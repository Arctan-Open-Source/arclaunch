#include "ExecutableNode.hxx"
#include <sys/wait.h>

namespace arclaunch {
// ExecutableNode
ExecutableNode::ExecutableNode(NodeContext& ctx, executable_t* elem, executable_t::arg_sequence& args, executable_t::env_sequence& envs) {
  // This is where fork-exec happens
  pid = fork();
  if(pid == 0) {
    // forked thread
    // construct the argument list
    std::vector<std::vector<char> > argData(argSequenceToArgData(elem->arg()));
    std::vector<char*> args;
    args.push_back(NULL);
    for(std::vector<std::vector<char> >::iterator it = argData.begin(); 
      it != argData.end(); it++)
      args.emplace_back(it->data());
    args.push_back(NULL);
    // construct the environment list
    std::vector<std::vector<char> > envData(envSequenceToEnvData(elem->env()));
    std::vector<char*> envs;
    for(std::vector<std::vector<char> >::iterator it = envData.begin();
      it != envData.end(); it++)
      envs.emplace_back(it->data());
    envs.push_back(NULL);
    // Deduce the proper path
    for(executable_t::path_iterator it = elem->path().begin(); 
      it != elem->path().end(); it++) {
      // Check if it's the correct OS
      if(*(it->os()) != OS_STRING)
        continue;
      // Construct the path
      std::vector<char> path(pathElemToPathData(*it));
      // Place the path as argv[0]
      args[0] = path.data();
      execve(path.data(), args.data(), envs.data());
    }
  }
  // main thread
}

bool ExecutableNode::isRunning() const {
  if(pid == 0)
    return false;
  else
  {
    // TODO: vary by operating system
    int status;
    waitpid(pid, &status, WNOHANG);
  }
}

pid_t ExecutableNode::getPid() const {
  return pid;
}

void ExecutableNode::waitFor() const {
  int status;
  waitpid(pid, &status, 0);
}

int ExecutableNode::stdin_fd() const {
  return in_fd;
}

int ExecutableNode::stdout_fd() const {
  return out_fd;
}

int ExecutableNode::stderr_fd() const {
  return err_fd;
}

ExecutableNode::~ExecutableNode() {}

}
