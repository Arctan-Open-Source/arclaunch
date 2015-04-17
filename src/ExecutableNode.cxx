#include "ExecutableNode.hxx"
#include <sys/wait.h>

namespace arclaunch {
// ExecutableNode
ExecutableNode::ExecutableNode(NodeContext& ctx, executable_t* elem, executable_t::arg_sequence& args, executable_t::env_sequence& envs) {
  // This is where fork-exec happens
  // Create three pipes
  int stdinPipes[2];
  int stdoutPipes[2];
  int stderrPipes[2];
  // create the three necessary pipes
  // TODO: throw an error if opening pipes fails
  int success;
  success = pipe(stdinPipes);
  success = pipe(stdoutPipes);
  success = pipe(stderrPipes);
  // Store the transmitting end of stdin
  inFd = stdinPipes[1];
  // Store the receiving end of stdout
  outFd = stdoutPipes[0];
  // Store the receiving end of stderr
  errFd = stderrPipes[0];
  pid = fork();
  if(pid == 0) {
    // re-route stdin, stdout, and stderr
    // TODO: throw an error if redirecting the pipes fails
    success = dup2(stdinPipes[0], STDIN_FILENO);
    success = dup2(stdoutPipes[1], STDOUT_FILENO);
    success = dup2(stderrPipes[1], STDERR_FILENO);
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
      int failure;
      failure = execve(path.data(), args.data(), envs.data());
      // Starting the process failed somehow
      // TODO: throw an error
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

int ExecutableNode::stdinFd() const {
  return inFd;
}

int ExecutableNode::stdoutFd() const {
  return outFd;
}

int ExecutableNode::stderrFd() const {
  return errFd;
}

ExecutableNode::~ExecutableNode() {}

}
