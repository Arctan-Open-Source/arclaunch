#include "Node.hxx"

// TODO: vary by operating system
// TODO: catch the terminate signal and pass it on to the children
#include <sys/types.h>
#include <exception>

namespace arclaunch {

// Node
Node::~Node() {
  // close any unclosed file descriptors
  closeFds();
}

void Node::closeFds() {
  for(std::map<int, int>::iterator it = fdMap.begin(); it != fdMap.end(); it++)
    close(it->second);
  fdMap.clear();
}

void Node::linkFd(int fd, int extFd) {
  // TODO: fix possible race condition for fork occurring between dup and CLOEXEC
  if(fdMap.find(fd) != fdMap.end())
    close(fdMap[fd]);
  fdMap[fd] = dup(extFd);
  fcntl(fdMap[fd], F_SETFD, FD_CLOEXEC);
}

void Node::linkStdin(int fd) {
  // check that the descriptor is readable
  if(isReadable(fd))
    linkFd(STDIN_FILENO, fd);
  else
    throw std::exception();
}

void Node::linkStdout(int fd) {
  // check that the descriptor is writable
  if(isWritable(fd))
    linkFd(STDOUT_FILENO, fd);
  else
    throw std::exception();
}

void Node::linkStderr(int fd) {
  // check that the descriptor is writable
  if(isWritable(fd))
    linkFd(STDERR_FILENO, fd);
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

// Templatize these? Use functional features of C++11?
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
