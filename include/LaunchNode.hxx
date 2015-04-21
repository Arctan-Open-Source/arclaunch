#include "Node.hxx"

#ifndef _LAUNCH_NODE_HXX_
#define _LAUNCH_NODE_HXX_
namespace arclaunch {

class LaunchNode : public Node {
  typedef std::map<std::string, Node*>::iterator node_iterator;
  typedef std::map<std::string, Node*>::const_iterator const_node_iterator;
private:
  launch_t* interior;
  std::map<std::string, Node*> nodes;
  // file descriptors
  std::map<std::string, int> files;
public:
  LaunchNode(NodeContext& ctx, launch_t* launchElem);
  virtual ~LaunchNode();
  virtual void startup();
  virtual bool isRunning() const;
  virtual pid_t getPid() const;
  virtual void waitFor() const;
  // Very strange contortions can occur from here
  virtual void linkStdin(int fd);
  virtual int getStdout();
  virtual int getStderr();
};

}
#endif
