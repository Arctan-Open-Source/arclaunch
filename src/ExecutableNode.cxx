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

std::map<pid_t, ExecutableNode*> ExecutableNode::running_nodes;

// ExecutableNode
void ExecutableNode::reaper(int snum, siginfo_t* info, void* uc) {
  if(ExecutableNode::running_nodes.find(info->si_pid) != ExecutableNode::running_nodes.end()) {
    Node* reaped_node = ExecutableNode::running_nodes[info->si_pid];
    // remove the reaped node from the list of running_nodes
    ExecutableNode::running_nodes.erase(info->si_pid);
    int status;
    waitpid(info->si_pid, &status, 0);
    reaped_node->onDeath(WEXITSTATUS(status), reaped_node, reaped_node->deathData);
  }
}

ExecutableNode::ExecutableNode(NodeContext& ctx, const executable_t& elem) {
  pid = 0;
  pathSeq = elem.path();
  argSeq = elem.arg();
  envSeq = elem.env();
}

// Due to major changes, executable nodes should be reusable
void ExecutableNode::startup() {
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
  running_nodes[pid] = this;
  // close the pipes
  closeFds();
}

bool ExecutableNode::isRunning() const {
  if(pid == 0)
    return false;
  else
  {
    // TODO: vary by operating system
    int status;
    pid_t exited = waitpid(pid, &status, WNOHANG);
    // returns 0 if no child has stopped which casts to false as a bool
    return exited;
  }
}

pid_t ExecutableNode::getPid() const {
  return pid;
}

void ExecutableNode::waitFor() {
  int status;
  if(pid != 0)
    waitpid(pid, &status, 0);
    // TODO: use macros to determine the circumstances of the exit
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
