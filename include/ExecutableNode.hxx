#include "Node.hxx"
#ifndef _EXECUTABLE_NODE_HXX_
#define _EXECUTABLE_NODE_HXX_

namespace arclaunch {
// Derive from the mapped types
class ExecutableNode : public Node {
private:
  pid_t pid;
  std::map<int, int> fdMap;
  executable_t::path_sequence pathSeq;
  executable_t::arg_sequence argSeq;
  executable_t::env_sequence envSeq;
  void closeFds();
public:
  // During construction file descriptors are created that are used the first time the node is started up
  // 
  ExecutableNode(NodeContext& ctx, const executable_t& elem);
  virtual ~ExecutableNode();
  // When startup is called the stdin, stdout, and stderr file descriptors are closed in the parent process
  virtual void startup();
  virtual bool isRunning() const;
  virtual pid_t getPid() const;
  virtual void waitFor();
  // The file descriptors given to the process is "dup"ed so that even if the provided descriptor is closed
  // the newly created descriptor will be closed on exec
  virtual void linkFd(int fd, int extFd);
  virtual void appendArguments(const executable_t::arg_sequence& args);
  virtual void appendEnvironment(const executable_t::env_sequence& env);
};

}
#endif
