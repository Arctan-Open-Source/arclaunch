#include "ExecutableNode.hxx"
#include <sys/wait.h>
#include <exception>
#include <errno.h>
#include <sys/select.h>

namespace arclaunch {

const char* ForkError::msg("Failed to fork process");

ForkError::ForkError() noexcept {
  num = errno;
}

ForkError::~ForkError() {
}

const char* ForkError::what() const noexcept {
  return msg;
}

bool ExecutableNode::reaping(false);

struct sigaction ExecutableNode::reap;

std::map<pid_t, ExecutableNode*> ExecutableNode::running_nodes;

// ExecutableNode

// private methods
void ExecutableNode::reaper(int snum, siginfo_t* info, void* uc) {
  int status;
  pid_t reaped = wait(&status);
  if(running_nodes.find(reaped) != running_nodes.end()) {
    running_nodes[reaped]->reapProcess(reaped, WEXITSTATUS(status));
  }
}

void ExecutableNode::reapProcess(pid_t reaped, int retval) {
  running_nodes.erase(reaped);
  finishInstance(pidInstance[reaped], retval);
}


// protected methods

// public methods
ExecutableNode::ExecutableNode(NodeContext& ctx, const executable_t& elem) {
  pathSeq = elem.path();
  argSeq = elem.arg();
  envSeq = elem.env();
  defer = elem.defer();
}

// Due to major changes, executable nodes should be reusable
void ExecutableNode::startInstance(int instNum) {
  if(!reaping) {
    reaping = true;
    reap.sa_sigaction = reaper;
    reap.sa_flags = SA_NOCLDSTOP | SA_RESTART;
    sigaction(SIGCHLD, &reap, NULL);
  }
  // construct the argument list
  std::vector<std::vector<char> > argData(argSequenceToArgData(argSeq));
  std::vector<char*> argList;
  argList.push_back(NULL);
  for(std::vector<std::vector<char> >::iterator it = argData.begin(); 
    it != argData.end(); ++it)
    argList.emplace_back(it->data());
  argList.push_back(NULL);
  // construct the environment list
  std::vector<std::vector<char> > envData(envSequenceToEnvData(envSeq));
  std::vector<char*> envList;
  for(std::vector<std::vector<char> >::iterator it = envData.begin();
    it != envData.end(); ++it)
    envList.emplace_back(it->data());
  envList.push_back(NULL);
  
  pid_t pid;
  if((pid = fork()) == 0) {
    // forked thread
    // defer startup, possibly exiting if no data is ever passed in
    // relies on boolean short-circuiting
    if(defer && deferReadFds(defer))
      exit(EXIT_SUCCESS);
    // Map the file descriptors to pass to the forked process
    overwriteFds();
    // Deduce the proper path
    for(executable_t::path_iterator it = pathSeq.begin(); 
      it != pathSeq.end(); ++it) {
      // Check if it's the correct OS
      if(it->os() != OS_STRING)
        continue;
      // Construct the path
      std::vector<char> path(pathElemToPathData(*it));
      // Place the path as argv[0]
      argList[0] = path.data();
      int failure;
      failure = execve(path.data(), argList.data(), envList.data());
    }
    const char* err = "Failed to find viable executable path";
    write(STDERR_FILENO, err, 39);
    exit(EXIT_FAILURE);
  } else if(pid < 0) {
    // Failed to fork
    throw ForkError();
  }
  running_nodes[pid] = this;
  pidInstance[pid] = instNum;
  // close the pipes
}

void ExecutableNode::waitForInstance(int instNum) {
  int status;
  pid_t pid, result;
  // Find the pid, it's an inefficient action, but who cares we're burning
  // time anyway
  // TODO: implement efficiently
  for(std::map<pid_t, int>::iterator it = pidInstance.begin(); 
    it != pidInstance.end(); ++it) {
    if(it->second == instNum)
      pid = it->first;
  }
  while(instanceIsRunning(instNum)) {
    result = waitpid(pid, &status, 0);
    if(result != -1)
      reapProcess(pid, WEXITSTATUS(status));
  }
}

void ExecutableNode::appendArguments(const executable_t::arg_sequence& args) {
  argSeq.insert(argSeq.end(), args.begin(), args.end());
}

void ExecutableNode::appendEnvironment(const executable_t::env_sequence& envs) {
  envSeq.insert(envSeq.end(), envs.begin(), envs.end());
}

ExecutableNode::~ExecutableNode() {
}

}
