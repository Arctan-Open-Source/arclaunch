#include "Node.hxx"
#include <vector>
#include <map>
#include <signal.h>
#ifndef _EXECUTABLE_NODE_HXX_
#define _EXECUTABLE_NODE_HXX_

namespace arclaunch {
// Derive from the mapped types
class ExecutableNode : public Node {
private:
  std::set<pid_t> pids;
  executable_t::path_sequence pathSeq;
  executable_t::arg_sequence argSeq;
  executable_t::env_sequence envSeq;
  // Used for capturing SIGCHLD gracefully and proper reaping
  static std::map<pid_t, ExecutableNode*> running_nodes;
  // used to indicate if reaper has been set as the SIGCHLD handler
  static bool reaping;
  static struct sigaction reap;
  // The SIGCHLD callback
  static void reaper(int snum, siginfo_t* info, void* uc);
public:
  // During construction file descriptors are created that are used the first time the node is started up
  // 
  ExecutableNode(NodeContext& ctx, const executable_t& elem);
  virtual ~ExecutableNode();
  // When startup is called the passed in file descriptors are closed in the parent process
  virtual void startup();
  virtual bool isRunning() const;
  virtual void waitFor();
  // The file descriptors given to the process is "dup"ed so that even if the provided descriptor is closed
  // the newly created descriptor will be closed on exec
  virtual void appendArguments(const executable_t::arg_sequence& args);
  virtual void appendEnvironment(const executable_t::env_sequence& env);
  void reapProcess(pid_t reaped, int retval);
};

// Exception Classes
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

class AlreadyRunningError : std::exception {
private:
  static const char* msg;
public:
  virtual ~AlreadyRunningError();
  virtual const char* what() const noexcept;
};

}
#endif
