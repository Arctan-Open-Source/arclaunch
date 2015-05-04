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

class AlreadyRunningError : std::exception {
private:
  static const char* msg;
public:
  virtual ~AlreadyRunningError();
  virtual const char* what() const noexcept;
};

const char* AlreadyRunningError::msg("Cannot start-up an already running node");

AlreadyRunningError::~AlreadyRunningError() {
}

const char* AlreadyRunningError::what() const noexcept {
  return msg;
}

// ExecutableNode
ExecutableNode::ExecutableNode(NodeContext& ctx, const executable_t& elem) {
  // 
  pid = 0;
  pathSeq = elem.path();
  argSeq = elem.arg();
  envSeq = elem.env();
  inFd = 0;
  outFdRead = 1;
  outFdWrite = 1;
  errFdRead = 2;
  errFdWrite = 2;
}

bool ExecutableNode::outPipeIsOpen() const {
  return outFdRead != 1 && outFdWrite != 1;
}

bool ExecutableNode::errPipeIsOpen() const {
  return errFdRead != 2 && errFdWrite != 2;
}

void ExecutableNode::openOutPipe() {
  closeOutPipe();
  int stdoutPipes[2];
  if(pipe2(stdoutPipes, O_CLOEXEC)) {
    throw PipeOpenError();
  }
  // Store the pipe to reroute stdout
  outFdRead = stdoutPipes[0];
  outFdWrite = stdoutPipes[1];
}

void ExecutableNode::openErrPipe() {
  closeErrPipe();
  int stderrPipes[2];
  if(pipe2(stderrPipes, O_CLOEXEC)) {
    throw PipeOpenError();
  }
  // Store the pipe to reroute stderr
  errFdRead = stderrPipes[0];
  errFdWrite = stderrPipes[1];
}

void ExecutableNode::closeErrPipe() {
  if(!errPipeIsOpen())
    return;
  close(errFdRead);
  close(errFdWrite);
}

void ExecutableNode::closeOutPipe() {
  if(!outPipeIsOpen())
    return;
  close(outFdRead);
  close(outFdWrite);
}

void ExecutableNode::closePipes() {
  if(inFd != 0)
    close(inFd);
  closeOutPipe();
  closeErrPipe();
}

// Due to major changes, executable nodes should be reusable
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
    // Overwrite stdin in the forked process
    if(inFd && dup2(inFd, STDIN_FILENO)) {
      const char* err = "Failed to overwrite standard in\n";
      write(STDERR_FILENO, err, 32);
      exit(EXIT_FAILURE);
    }
    // re-route stdin, stdout, and stderr
    int ndout;
    int nderr;
    // re-route stdout and stderr
    if(outPipeIsOpen() && (ndout = dup2(outFdWrite, STDOUT_FILENO)) == -1) {
      const char* err = "Failed to overwrite standard out pipe\n";
      write(STDERR_FILENO, err, 39);
      exit(EXIT_FAILURE);
    }
    if(errPipeIsOpen() && (nderr = dup2(errFdWrite, STDERR_FILENO)) == -1) {
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
    // Failed to fork
    throw ForkError();
  }
  // close the pipes
  closePipes();
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

void ExecutableNode::waitFor() const {
  int status;
  if(pid != 0) {
    waitpid(pid, &status, 0);
    // TODO: use macros to determine the circumstances of the exit
  }
}

void ExecutableNode::linkStdin(int fd) {
  // TODO: If there is a multithreaded process that forks at the same time this is called, the child process
  // TODO: may get a non-CLOEXEC copy of the generated file descriptor
  // TODO: fix this race condition, possibly by use of dup3 or by adding a global lock to this method
  // duplicate the file descriptor so the file descriptor passed in can be closed and recycled
  inFd = dup(fd);
  // the duplicate still needs to be closed-on-exec to prevent superfluous file descriptors in the child processes
  fcntl(inFd, F_SETFD, FD_CLOEXEC);
}

int ExecutableNode::getStdout() {
  // open pipes if necessary
  if(!outPipeIsOpen())
    openOutPipe();
  return outFdRead;
}

int ExecutableNode::getStderr() {
  // open pipes if necessary
  if(!errPipeIsOpen())
    openErrPipe();
  return errFdRead;
}

void ExecutableNode::appendArguments(const executable_t::arg_sequence& args) {
  argSeq.insert(argSeq.end(), args.begin(), args.end());
}

void ExecutableNode::appendEnvironment(const executable_t::env_sequence& envs) {
  envSeq.insert(envSeq.end(), envs.begin(), envs.end());
}

ExecutableNode::~ExecutableNode() {
  // Close any unclosed pipe file descriptors
  closePipes();
}

}
