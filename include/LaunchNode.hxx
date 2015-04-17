#include "Node.hxx"

#ifndef _LAUNCH_NODE_HXX_
#define _LAUNCH_NODE_HXX_
namespace arclaunch {

class LaunchNode : public Node {
  typedef std::vector<Node*>::iterator node_iterator;
  typedef std::vector<Node*>::const_iterator const_node_iterator;
private:
  launch_t* interior;
  int in_fd;
  int out_fd;
  int err_fd;
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
  virtual int stdin_fd() const;
  virtual int stdout_fd() const;
  virtual int stderr_fd() const;
};

}
#endif
