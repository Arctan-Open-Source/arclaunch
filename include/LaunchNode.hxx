#include "GroupNode.hxx"

#ifndef _LAUNCH_NODE_HXX_
#define _LAUNCH_NODE_HXX_
namespace arclaunch {

class LaunchNode : public GroupNode {
private:
  class LaunchInstance {
  public:
    LaunchNode* owner;
    int instNum;
    std::map<Node*, int> nodeInstances;
  };
  std::map<int, LaunchInstance> instances;
  static void onSubNodeDeath(int retval, Node* subNode, void* launchInst, int instNum);
  launch_t::linkage_sequence links;
  launch_t::externalize_sequence externals;
protected:
  virtual void startInstance(int instNum);
public:
  LaunchNode(NodeContext& ctx, const launch_t& launchElem);
  virtual ~LaunchNode();
  virtual void waitForInstance(int instNum);
};

}
#endif
