#include "SequenceNode.hxx"

namespace arclaunch {

SequenceNode::SequenceNode(NodeContext& ctx, const sequence_node_t& sequenceElem) :
  GroupNode(ctx, sequenceElem) {
  // set onComplete for each node
  for(sequence_node_t::node_const_iterator it = sequenceElem.node().begin();
    it != sequenceElem.node().end(); ++it) {
    getNode(it->name()).onComplete(SequenceNode::proceed, this);
    order.push_back(it->name());
  }
}

SequenceNode::~SequenceNode() {
  
}

void SequenceNode::startup() {
  stage = -1;
  proceed(0, NULL, this);
}

// static
void SequenceNode::proceed(int retval, Node* node, void* data) {
  SequenceNode* self = static_cast<SequenceNode*>(data);
  if(retval) // non-0 exit code, stop everything
    return;
  // proceed to the next stage
  self->stage++;
  // Connect all the pipes
  Node* nextNode = &self->getNode(self->order[self->stage]);
  for(std::map<int, int>::iterator it = self->fdMap.begin(); it != self->fdMap.end(); 
    ++it) {
    nextNode->linkFd(it->second, it->first);
  }
  nextNode->startup();
}

bool SequenceNode::isRunning() const {
}

void SequenceNode::waitFor() {
}

}

