#include "ExecutableNode.hxx"
#include <sys/wait.h>
#include <exception>
#include <errno.h>

namespace arclaunch {

// Error in case of failed pipe opening
class PipeOpenError : std::exception {
private:
  static const char* msg;
  int num;
public:
  PipeOpenError() noexcept;
  virtual ~PipeOpenError();
  virtual const char* what() const noexcept;
};

const char* PipeOpenError::msg("Failed to open pipe");

PipeOpenError::PipeOpenError() noexcept {
  num = errno;
}

PipeOpenError::~PipeOpenError() {
}

const char* PipeOpenError::what() const noexcept {
  return msg;
}

// Error in case of failed forking
class ForkError : std::exception {
private:
  static const char* msg;
  int num;
public:
  ForkError() noexcept;
  virtual ~ForkError();
  virtual const char* what() const noexcept;
};

const char* ForkError::msg("Failed to fork process");

ForkError::ForkError() noexcept {
  num = errno;
}

ForkError::~ForkError() {
}

const char* ForkError::what() const noexcept {
  return msg;
}


// ExecutableNode
ExecutableNode::ExecutableNode(NodeContext& ctx, const executable_t& elem) {
  // 
  inFd = 0;
  pathSeq = elem.path();
  argSeq = elem.arg();
  envSeq = elem.env();
  
  // This is where fork-exec happens
  // Create three pipes
  int stdoutPipes[2];
  int stderrPipes[2];
  // create the necessary pipes
  // set close-on-exec so that the desciptors aren't open
  // in the child process
  if(pipe2(stdoutPipes, O_CLOEXEC) || 
    pipe2(stderrPipes, O_CLOEXEC)) {
    // Failed to open pipes
    throw PipeOpenError();
  }
  // Store the pipe to reroute stdout
  // read end of pipe
  outFdRead = stdoutPipes[0];
  // write end of pipe
  outFdWrite = stdoutPipes[1];
  // Store the pipe to reroute stderr
  errFdRead = stderrPipes[0];
  errFdWrite = stderrPipes[1];
}

void ExecutableNode::startup() {
  // construct the argument list
  std::vector<std::vector<char> > argData(argSequenceToArgData(argSeq));
  std::vector<char*> argList;
  argList.push_back(NULL);
  for(std::vector<std::vector<char> >::iterator it = argData.begin(); 
    it != argData.end(); it++)
    argList.emplace_back(it->data());
  argList.push_back(NULL);
  // construct the environment list
  std::vector<std::vector<char> > envData(envSequenceToEnvData(envSeq));
  std::vector<char*> envList;
  for(std::vector<std::vector<char> >::iterator it = envData.begin();
    it != envData.end(); it++)
    envList.emplace_back(it->data());
  envList.push_back(NULL);
  if((pid = fork()) == 0) {
    // close the pipes you don't use in the forked process
    if(inFd && dup2(inFd, STDIN_FILENO)) {
      const char* err = "Failed to overwrite standard in\n";
      write(STDERR_FILENO, err, 32);
      exit(EXIT_FAILURE);
    }
    // re-route stdin, stdout, and stderr
    int ndout;
    int nderr;
    if((ndout = dup2(outFdWrite, STDOUT_FILENO)) == -1) {
      const char* err = "Failed to overwrite standard out pipe\n";
      write(STDERR_FILENO, err, 39);
      exit(EXIT_FAILURE);
    }
    if((nderr = dup2(errFdWrite, STDERR_FILENO)) == -1) {
      const char* err = "Failed to overwrite standard err pipe\n";
      write(STDERR_FILENO, err, 39);
      exit(EXIT_FAILURE);
    }
    // forked thread
    // Deduce the proper path
    for(executable_t::path_iterator it = pathSeq.begin(); 
      it != pathSeq.end(); it++) {
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
    // TODO: use a more descriptive exception
    // Failed to fork
    throw ForkError();
  }
  // close the write end of the out and error pipes
  close(outFdWrite);
  close(errFdWrite);
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

void ExecutableNode::linkStdin(int fd) {
  inFd = fd;
}

int ExecutableNode::getStdout() {
  return outFdRead;
}

int ExecutableNode::getStderr() {
  return errFdRead;
}

void ExecutableNode::appendArguments(const executable_t::arg_sequence& args) {
  argSeq.insert(argSeq.end(), args.begin(), args.end());
}

void ExecutableNode::appendEnvironment(const executable_t::env_sequence& envs) {
  envSeq.insert(envSeq.end(), envs.begin(), envs.end());
}

ExecutableNode::~ExecutableNode() {}

}
