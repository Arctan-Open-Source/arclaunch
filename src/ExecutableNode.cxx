#include "ExecutableNode.hxx"
#include <sys/wait.h>
#include <exception>
#include <errno.h>

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

const char* AlreadyRunningError::msg("Cannot start-up an already running node");

AlreadyRunningError::~AlreadyRunningError() {
}

const char* AlreadyRunningError::what() const noexcept {
  return msg;
}

bool ExecutableNode::reaping(false);

struct sigaction ExecutableNode::reap;

std::map<pid_t, ExecutableNode*> ExecutableNode::running_nodes;

// ExecutableNode
void ExecutableNode::reaper(int snum, siginfo_t* info, void* uc) {
  int status;
  pid_t reaped = wait(&status);
  if(ExecutableNode::running_nodes.find(reaped) != ExecutableNode::running_nodes.end()) {
    ExecutableNode::running_nodes[reaped]->reapProcess(reaped, WEXITSTATUS(status));
  }
}

void ExecutableNode::reapProcess(pid_t reaped, int retval) {
  running_nodes.erase(reaped);
  pids.erase(reaped);
  for(std::vector<Node::CompletionHandler>::iterator it = onDeath.begin(); 
    it != onDeath.end(); ++it) {
    (it->callback)(retval, this, it->data);
  }
}

ExecutableNode::ExecutableNode(NodeContext& ctx, const executable_t& elem) {
  pathSeq = elem.path();
  argSeq = elem.arg();
  envSeq = elem.env();
}

// Due to major changes, executable nodes should be reusable
void ExecutableNode::startup() {
  if(!reaping) {
    reaping = true;
    reap.sa_sigaction = reaper;
    reap.sa_flags = SA_NOCLDSTOP;
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
    // Map the file descriptors to pass to the forked process
    for(std::map<int, int>::iterator it = fdMap.begin(); it != fdMap.end(); ++it) {
      if(dup2(it->first, it->second) != it->second) {
        const char* err = "Failed to overwrite pipe\n";
        write(STDERR_FILENO, err, 27);
        exit(EXIT_FAILURE);
      }
    }
    // forked thread
    // Deduce the proper path
    for(executable_t::path_iterator it = pathSeq.begin(); 
      it != pathSeq.end(); ++it) {
      // Check if it's the correct OS
      if(*(it->os()) != OS_STRING)
        continue;
      // Construct the path
      std::vector<char> path(pathElemToPathData(*it));
      // Place the path as argv[0]
      argList[0] = path.data();
      int failure;
      failure = execve(path.data(), argList.data(), envList.data());
    }
    // Starting the process failed somehow
    // exit 1, 
    const char* err = "Failed to find viable executable path";
    write(STDERR_FILENO, err, 39);
    exit(EXIT_FAILURE);
  } else if(pid < 0) {
    // Failed to fork
    throw ForkError();
  }
  pids.insert(pid);
  running_nodes[pid] = this;
  // close the pipes
  closeFds();
}

bool ExecutableNode::isRunning() const {
  return !pids.empty();
}

void ExecutableNode::waitFor() {
  int status;
  pid_t pid;
  while(isRunning()) {
    pid = waitpid(*pids.begin(), &status, 0);
    if(pid != -1)
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
