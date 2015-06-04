#include "SequenceNode.hxx"

namespace arclaunch {

SequenceNode::SequenceNode(NodeContext& ctx, const sequence_node_t& sequenceElem) {
  // Emplace nodes
  for(sequence_node_t::node_const_iterator it = sequenceElem.node().begin();
    it != sequenceElem.node().end(); ++it) {
    Node* newNode = &ctx.execute(*it);
    if(newNode)
      nodes[it->name()] = newNode;
    else
      throw NodeConstructionError();
  }
}

SequenceNode::~SequenceNode() {
  
}

void SequenceNode::startup() {
}

bool SequenceNode::isRunning() {
}

void SequenceNode::waitFor() {
}

}

