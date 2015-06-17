#include "GroupNode.hxx"
#include "NodeContext.hxx"

namespace arclaunch {

// GroupNode class
GroupNode::GroupNode(NodeContext& ctx, const group_t& groupElem) {
  // Emplace the nodes in the group
  for(group_t::node_const_iterator it = groupElem.node().begin();
    it != groupElem.node().end(); ++it) {
    if(Node* newNode = &ctx.execute(*it))
      nodes[it->name()] = newNode;
    else
      throw NodeConstructionError();
  }
}

GroupNode::~GroupNode() {
}

Node& GroupNode::getNode(std::string name) {
  return *nodes[name];
}

// Node Construction Error Exception
const char* NodeConstructionError::msg("Failed to find proper node constructor");

NodeConstructionError::NodeConstructionError() noexcept {
}

NodeConstructionError::~NodeConstructionError() {
}

const char* NodeConstructionError::what() const noexcept {
  return msg;
}

}
