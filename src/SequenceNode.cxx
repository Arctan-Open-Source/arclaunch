#include "SequenceNode.hxx"

namespace arclaunch {

SequenceNode::SequenceNode(NodeContext& ctx, const sequence_node_t& sequenceElem) :
  GroupNode(ctx, sequenceElem), 
  stage(-1) {
  // set onComplete for each node
  for(sequence_node_t::node_const_iterator it = sequenceElem.node().begin();
    it != sequenceElem.node().end(); ++it) {
    getNode(it->name()).addCompletionHandler(SequenceNode::proceed, this);
    order.push_back(it->name());
  }
}

SequenceNode::~SequenceNode() {
  
}

void SequenceNode::startup() {
  proceed(0, NULL, this);
}

// static
void SequenceNode::proceed(int retval, Node* node, void* data) {
  SequenceNode* self = static_cast<SequenceNode*>(data);
  self->stage++;
  if(retval || self->stage >= self->order.size()) { 
    // non-0 exit code or finished, stop everything
    // call onDeath
    for(std::vector<CompletionHandler>::iterator it = self->onDeath.begin(); 
      it != self->onDeath.end(); ++it) {
      (it->callback)(retval, self, it->data);
    }
    self->stage = -1;
    self->closeFds();
    return;
  }
  // proceed to the next stage
  // Connect all the pipes
  Node* nextNode = &self->getNode(self->order[self->stage]);
  for(std::map<int, int>::iterator it = self->fdMap.begin(); it != self->fdMap.end(); 
    ++it) {
    nextNode->linkFd(it->second, it->first);
  }
  nextNode->startup();
}

bool SequenceNode::isRunning() const {
  return stage != -1;
}

void SequenceNode::waitFor() {
  // This is a more or less brute force way to handle it
  // so long as a signal is used for termination and isRunning is changed by the handler, this should work
  int *nlptr = NULL;
  while(isRunning())
    wait(nlptr);
}

}

