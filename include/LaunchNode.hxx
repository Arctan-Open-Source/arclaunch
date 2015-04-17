#include "Node.hxx"

#ifndef _LAUNCH_NODE_HXX_
#define _LAUNCH_NODE_HXX_
namespace arclaunch {

class LaunchNode : public Node {
  typedef std::vector<Node*>::iterator node_iterator;
  typedef std::vector<Node*>::const_iterator const_node_iterator;
private:
  launch_t* interior;
  int inFd;
  int outFd;
  int errFd;
  std::vector<Node*> nodes;
public:
  LaunchNode(NodeContext& ctx, launch_t* launchElem, executable_t::arg_sequence& args, executable_t::env_sequence& envs);
  virtual ~LaunchNode();
  virtual bool isRunning() const;
  virtual pid_t getPid() const;
  virtual void waitFor() const;
  // Multiplexes stdout and stderr
  // Repeats stdin to all sub-nodes
  // Really, a very ugly answer
  virtual int stdinFd() const;
  virtual int stdoutFd() const;
  virtual int stderrFd() const;
};

}
#endif
