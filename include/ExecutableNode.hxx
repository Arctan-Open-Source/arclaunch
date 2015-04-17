#include "Node.hxx"
#ifndef _EXECUTABLE_NODE_HXX_
#define _EXECUTABLE_NODE_HXX_

namespace arclaunch {
// Derive from the mapped types
class ExecutableNode : public Node {
private:
  pid_t pid;
  int inFd;
  int outFd;
  int errFd;
public:
  ExecutableNode(NodeContext& ctx, executable_t* elem, executable_t::arg_sequence& args, executable_t::env_sequence& envs);
  virtual ~ExecutableNode();
  virtual bool isRunning() const;
  virtual pid_t getPid() const;
  virtual void waitFor() const;
  virtual int stdinFd() const;
  virtual int stdoutFd() const;
  virtual int stderrFd() const;
};

}
#endif
