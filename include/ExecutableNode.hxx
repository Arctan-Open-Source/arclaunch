#include "Node.hxx"
#ifndef _EXECUTABLE_NODE_HXX_
#define _EXECUTABLE_NODE_HXX_

namespace arclaunch {
// Derive from the mapped types
class ExecutableNode : public Node {
private:
  pid_t pid;
  int inFd;
  int outFdRead;
  int outFdWrite;
  int errFdRead;
  int errFdWrite;
  executable_t::path_sequence pathSeq;
  executable_t::arg_sequence argSeq;
  executable_t::env_sequence envSeq;
  bool outPipeIsOpen() const;
  bool errPipeIsOpen() const;
  void openOutPipe();
  void openErrPipe();
  void closeOutPipe();
  void closeErrPipe();
  void closePipes();
public:
  // During construction file descriptors are created that are used the first time the node is started up
  // 
  ExecutableNode(NodeContext& ctx, const executable_t& elem);
  virtual ~ExecutableNode();
  // When startup is called the stdin, stdout, and stderr file descriptors are closed in the parent process
  virtual void startup();
  virtual bool isRunning() const;
  virtual pid_t getPid() const;
  virtual void waitFor() const;
  // The file descriptor given to the process is "dup"ed so that even if the provided descriptor is closed
  // the newly created descriptor will be closed on exec
  virtual void linkStdin(int fd);
  // Gets the read end of the stdout pipe for the next process started
  // Will generate a new stdout pipe if it doesn't already exist
  // Can't get the stdout pipe after the process is started, the file descriptors have been closed
  virtual int getStdout();
  // Gets the read end of the stderr pipe for the next process started
  // Will generate a new stderr pipe if it doesn't already exist
  // Can't get the stderr pipe after the process is started, the file descriptors have been closed
  virtual int getStderr();
  virtual void appendArguments(const executable_t::arg_sequence& args);
  virtual void appendEnvironment(const executable_t::env_sequence& env);
};

}
#endif
