#include "GroupNode.hxx"

#ifndef _LAUNCH_NODE_HXX_
#define _LAUNCH_NODE_HXX_
namespace arclaunch {

class LaunchNode : public GroupNode {
protected:
  launch_t::linkage_sequence links;
public:
  LaunchNode(NodeContext& ctx, const launch_t& launchElem);
  virtual ~LaunchNode();
  virtual void startup();
  virtual bool isRunning() const;
  virtual void waitFor();
};

}
#endif
