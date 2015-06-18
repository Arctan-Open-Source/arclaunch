#include "SequenceNode.hxx"
#include <sys/types.h>
#include <sys/wait.h>

namespace arclaunch {

// RunningSequence

SequenceNode::RunningSequence::RunningSequence(SequenceNode& possessor) :
  stage(-1), owner(&possessor) {
}

SequenceNode::RunningSequence::RunningSequence(SequenceNode* possessor) :
  stage(-1), owner(possessor) {
}

SequenceNode::RunningSequence::RunningSequence() :
  stage(-1) {
  owner = NULL;
}

SequenceNode::RunningSequence::~RunningSequence() {
}

void SequenceNode::RunningSequence::waitForInstance(int instNum) {
  // A RunningSequence is technically never actually started
}

void SequenceNode::RunningSequence::startInstance(int instNum) {
  // This actually needs to never startup in the conventional way
  // startup would cause the file descriptors to be closed
}

void SequenceNode::RunningSequence::proceed(int retval, Node* finishingNode, 
  void* data, int instNum) {
  // The instNum provided to proceed is irrelevant
  RunningSequence* self = static_cast<RunningSequence*>(data);
  SequenceNode* seq = self->owner;
  self->stage++;
  if(retval || self->stage >= seq->order.size()) { 
    // non-0 exit code or finished, stop everything
    // call onDeath for the SequenceNode
    self->stage = -1;
    seq->sequences.erase(self->instNum);
    seq->finishInstance(self->instNum, retval);
    return;
  }
  // Start the next node
  self->currentNode = &self->owner->getNode(self->owner->order[self->stage]);
  self->currentNode->addInstanceCompletionHandler(proceed, data);
  self->passFds(*self->currentNode);
  self->currentStageInstNum = self->currentNode->startup();
}

SequenceNode::SequenceNode(NodeContext& ctx, 
  const sequence_node_t& sequenceElem) :
  GroupNode(ctx, sequenceElem)
  {
  for(sequence_node_t::node_const_iterator it = sequenceElem.node().begin();
    it != sequenceElem.node().end(); ++it) {
    order.push_back(it->name());
  }
}

SequenceNode::~SequenceNode() {
  
}

void SequenceNode::startInstance(int instNum) {
  // create a new sequence
  // TODO, sort out proper emplacement of RunningSequences in a map
  sequences.emplace(
    std::piecewise_construct, 
    std::make_tuple(instNum), 
    std::make_tuple(this));
  RunningSequence *s = &sequences[instNum];
  // Devolve the file descriptors to the sequence
  passFds(*s);
  s->instNum = instNum;
  RunningSequence::proceed(0, NULL, s, instNum);
}


void SequenceNode::waitForInstance(int instNum) {
  /* The core issue is that waitFor is done but instanceIsRunning is still true */
  while(instanceIsRunning(instNum)) {
    sequences[instNum].currentNode->waitForInstance(
      sequences[instNum].currentStageInstNum);
  }
}

}

