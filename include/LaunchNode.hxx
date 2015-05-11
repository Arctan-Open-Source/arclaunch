#include "Node.hxx"

#ifndef _LAUNCH_NODE_HXX_
#define _LAUNCH_NODE_HXX_
namespace arclaunch {

class LaunchNode : public Node {
  typedef std::map<std::string, Node*>::iterator node_iterator;
  typedef std::map<std::string, Node*>::const_iterator const_node_iterator;
private:
  launch_t* interior;
protected:
  launch_t::linkage_sequence links;
  std::map<std::string, Node*> nodes;
public:
  LaunchNode(NodeContext& ctx, const launch_t& launchElem);
  virtual ~LaunchNode();
  virtual void startup();
  virtual bool isRunning() const;
  virtual void waitFor();
  // Very strange contortions can occur from here
  virtual void linkFd(int fd, int extFd);
  Node& getNode(std::string name);
};

}
#endif
