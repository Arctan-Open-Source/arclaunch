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
public:
  ExecutableNode(NodeContext& ctx, const executable_t& elem);
  virtual ~ExecutableNode();
  virtual void startup();
  virtual bool isRunning() const;
  virtual pid_t getPid() const;
  virtual void waitFor() const;
  virtual void linkStdin(int fd);
  virtual int getStdout();
  virtual int getStderr();
  virtual void appendArguments(const executable_t::arg_sequence& args);
  virtual void appendEnvironment(const executable_t::env_sequence& env);
};

}
#endif
