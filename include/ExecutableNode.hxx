#include "Node.hxx"
#include <map>
#include <signal.h>
#ifndef _EXECUTABLE_NODE_HXX_
#define _EXECUTABLE_NODE_HXX_

namespace arclaunch {
// Derive from the mapped types
class ExecutableNode : public Node {
private:
  executable_t::path_sequence pathSeq;
  executable_t::arg_sequence argSeq;
  executable_t::env_sequence envSeq;
  unsigned int defer;
  // Used for capturing SIGCHLD gracefully and proper reaping
  static std::map<pid_t, ExecutableNode*> running_nodes;
  
  // used to indicate if reaper has been set as the SIGCHLD handler
  static bool reaping;
  static struct sigaction reap;
  // The SIGCHLD callback
  // used to track instances
  static void reaper(int snum, siginfo_t* info, void* uc);
  std::map<pid_t, int> pidInstance;
  void reapProcess(pid_t reaped, int retval);
protected:
  virtual void startInstance(int instNum);
  virtual void appendArguments(const executable_t::arg_sequence& args);
  virtual void appendEnvironment(const executable_t::env_sequence& env);
public:
  ExecutableNode(NodeContext& ctx, const executable_t& elem);
  virtual ~ExecutableNode();
  virtual void waitForInstance(int instNum);
  // the newly created descriptor will be closed on exec
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

}
#endif
