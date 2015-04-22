#include "ExecutableNode.hxx"
#ifndef _SCRIPT_NODE_HXX
#define _SCRIPT_NODE_HXX
namespace arclaunch {

class ScriptNode : public ExecutableNode {
private:
  Node* interpreter;
  script_t* appended;
public:
  ScriptNode(NodeContext& ctx, const script_t& scriptElem);
  virtual ~ScriptNode();
};

}
#endif
