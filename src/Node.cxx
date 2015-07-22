#include "Node.hxx"

// TODO: vary by operating system
// TODO: catch the terminate signal and pass it on to the children
#include <sys/types.h>
#include <sys/select.h>
#include <exception>

namespace arclaunch {

// CompletionHandler definition
Node::CompletionHandler::CompletionHandler(Node::CompletionCallback call, void* deathData) {
  callback = call;
  data = deathData;
}

// private functions
void Node::closeFds() {
  for(std::map<int, int>::iterator it = fdMap.begin(); it != fdMap.end(); it++)
    close(it->first);
  fdMap.clear();
}

// protected functions
void Node::finishInstance(int instNum, int retval) {
  if(!instanceIsRunning(instNum))
    return;
  // Call generalized onDeath callbacks
  for(std::vector<CompletionHandler>::iterator it = onDeath.begin();
    it != onDeath.end(); ++it)
    it->callback(retval, this, it->data, instNum);
  // Call instance based onDeath callbacks
  for(std::vector<CompletionHandler>::iterator it = instances[instNum].begin();
    it != instances[instNum].end(); ++it)
    it->callback(retval, this, it->data, instNum);
  instances.erase(instNum);
}

void Node::overwriteFds() {
  for(std::map<int, int>::iterator it = fdMap.begin(); it != fdMap.end(); 
    ++it) {
    if(dup2(it->first, it->second) == -1) {
      const char* err = "Failed to overwrite pipe\n";
      write(STDERR_FILENO, err, 27);
      exit(EXIT_FAILURE);
    }
  }
}

void Node::passFds(Node& child) {
  for(std::map<int, int>::iterator it = fdMap.begin(); it != fdMap.end();
    ++it) {
    bool input = false;
    child.linkFd(it->second, it->first, isInput(it->first));
  }
}

bool Node::deferReadFds(unsigned int nbytes) {
  fd_set fds;
  int maxfd = 0, nfds;
  std::vector<int> nread(readFds.size(), 0);
  std::vector<bool> closedEarly(readFds.size(), false);
  std::vector<char> buffer(nbytes);
  FD_ZERO(&fds);
  do {
    for(int c = 0; c < readFds.size(); c++) {
      int fd = readFds[c], bytesRead;
      if(nread[c] >= nbytes || closedEarly[c] || !FD_ISSET(readFds[c], &fds))
        continue;
      bytesRead = read(fd, buffer.data(), nbytes - nread[c]);
      nread[c] += bytesRead;
      // 0 
      if(bytesRead == 0 && nread[c] < nbytes) { 
        // The incoming file descriptor has exited early
        closedEarly[c] = true;
      }
    }
    FD_ZERO(&fds);
    for(int c = 0; c < readFds.size(); ++c) {
      int fd = readFds[c];
      if(nread[c] >= nbytes)
        continue;
      if(maxfd < fd)
        maxfd = fd;
      FD_SET(fd, &fds);
    }
    ++maxfd;
  } while(nfds = select(maxfd, &fds, NULL, NULL, NULL));
}

bool Node::isInput(int fd) {
  for(std::vector<int>::iterator it = readFds.begin(); 
    it != readFds.end(); ++it) {
    if(fd == *it)
      return true;
  }
  return false;
}

void Node::passFd(Node& child, int fd, int childFd) {
  std::map<int, int>::iterator it;
  for(it = fdMap.begin(); 
    it != fdMap.end(); ++it) {
    if(it->second == fd) {
     child.linkFd(childFd, it->first, isInput(it->first));
     break;
    }
  }
}

// public functions
// Node

Node::Node() {
}

Node::~Node() {
  // close any unclosed file descriptors
  closeFds();
}

int Node::startup() {
  int newInstNum = 0;
  while(instanceIsRunning(newInstNum))
    newInstNum++;
  // allows startInstance to add more death handlers
  // needs to preceed startInstance to allow closing
  // while starting in cases of no subNodes
  instances[newInstNum] = onInstanceDeath;
  onInstanceDeath.clear();
  startInstance(newInstNum);
  closeFds();
  return newInstNum;
}

bool Node::instanceIsRunning(int instNum) const {
  return instances.find(instNum) != instances.end();
}

bool Node::isRunning() const {
  return !instances.empty();
}

int Node::numInstancesRunning() const {
  return instances.size();
}

void Node::waitFor() {
  // Can't use iteraters because they could become invalidated
  while(!instances.empty()) {
    int num = instances.begin()->first;
    waitForInstance(num);
  }
}

void Node::addCompletionHandler(CompletionCallback handle, void* data) {
  onDeath.emplace_back(handle, data);
}

void Node::addInstanceCompletionHandler(CompletionCallback handle, void* data) {
  onInstanceDeath.emplace_back(handle, data);
}

void Node::linkFd(int fd, int extFd, bool input) {
  // fd should be kept small
  // By ensuring nFd is greater than fd, the mapping will be performed such that nFd is never closed by accident
  // because each nFd must be greater than the previous, which is greater than the previous fds
  int nFd = fd + 1;
  int result = -1;
  while(result == -1) {
    // Check that nFd is not currently an open file descriptor
    if(fcntl(nFd, F_GETFD) != -1) {
      nFd++;
      continue;
    }
    result = dup3(extFd, nFd, O_CLOEXEC);
    if(result == -1) {
      if(errno == EBUSY) { // keep trying
        nFd++;
        continue;
      }
      // TODO throw a more descriptive exception
      throw std::exception();
    }
  }
  if(input)
    readFds.push_back(nFd);
  fdMap[nFd] = fd;
}

void Node::linkStdin(int fd) {
  // check that the descriptor is readable
  if(isReadable(fd))
    linkFd(STDIN_FILENO, fd, true);
  else
    throw std::exception();
}

void Node::linkStdout(int fd) {
  // check that the descriptor is writable
  if(isWritable(fd))
    linkFd(STDOUT_FILENO, fd, false);
  else
    throw std::exception();
}

void Node::linkStderr(int fd) {
  // check that the descriptor is writable
  if(isWritable(fd))
    linkFd(STDERR_FILENO, fd, false);
  else
    throw std::exception();
}

// Static methods of Node
bool Node::isReadable(int fd) {
  int fileFlags = fcntl(fd, F_GETFL) & (O_RDONLY | O_WRONLY | O_RDWR);
  return fileFlags == O_RDONLY || fileFlags == O_RDWR;
}

bool Node::isWritable(int fd) {
  int fileFlags = fcntl(fd, F_GETFL) & (O_RDONLY | O_WRONLY | O_RDWR);
  return fileFlags == O_WRONLY || fileFlags == O_RDWR;
}


// public static functions
std::vector<std::vector<char> > Node::argSequenceToArgData(executable_t::arg_sequence& args) {
  std::vector<std::vector<char> > argData;
  for(executable_t::arg_iterator it = args.begin();
    it != args.end(); it++) {
    argData.emplace_back(it->begin(), it->end());
    argData.back().push_back('\0');
  }
  return argData;
}

std::vector<std::vector<char> > Node::envSequenceToEnvData(executable_t::env_sequence& envs) {
  std::vector<std::vector<char> > envData;
  for(executable_t::env_iterator it = envs.begin();
    it != envs.end(); it++) {
    std::string envstr(*(it->key()) + "=" + *(it->value()));
    envData.emplace_back(envstr.begin(), envstr.end());
    envData.back().push_back('\0');
  }
  return envData;
}

executable_t::arg_sequence Node::fuseArgSequence(executable_t::arg_sequence& args1, executable_t::arg_sequence& args2) {
  executable_t::arg_sequence seq(args1);
  seq.insert(seq.end(), args2.begin(), args2.end());
  return seq;
}

executable_t::env_sequence Node::fuseEnvSequence(executable_t::env_sequence& envs1, executable_t::env_sequence& envs2) {
  executable_t::env_sequence seq(envs1);
  seq.insert(seq.end(), envs2.begin(), envs2.end());
  return seq;
}

std::vector<char> Node::pathElemToPathData(const path_t& pathElem) {
  std::vector<char> path;
  bool add_seperator = pathElem.absolute();
  for(path_t::directory_const_iterator it = pathElem.directory().begin(); 
    it != pathElem.directory().end(); it++)
  {
    if(add_seperator)
      path.insert(path.end(), DIR_SEPERATOR);
    add_seperator = true;
    path.insert(path.end(), it->begin(), it->end());
  }
  path.push_back('\0');
  return path;
}

}
