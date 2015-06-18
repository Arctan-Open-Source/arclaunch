#ifndef _SEQUENCE_NODE_HXX_
#define _SEQUENCE_NODE_HXX_
#include "GroupNode.hxx"

// A sequence node takes a set of programs and runs them in serial

// Given that each program returns 0 instead of 1, the programs are run one after the other
// using the same file handles for each of the programs

// If one of the programs returns 1, then the chain breaks and the sequence of programs stops altogether
namespace arclaunch {

class SequenceNode : public GroupNode {
private:
  class RunningSequence : public Node {
  protected:
    virtual void startInstance(int instNum);
  public:
    SequenceNode* owner;
    // The instance number assigned by the owner
    int instNum;
    // The currently running node
    Node* currentNode;
    // The instance number for our instance of the current node
    int currentStageInstNum;
    // The index of the currently running node in the sequence
    int stage;
    RunningSequence(SequenceNode& possessor);
    RunningSequence(SequenceNode* possessor);
    RunningSequence();
    virtual ~RunningSequence();
    virtual void waitForInstance(int instNum);
    static void proceed(int retval, Node* node, void* data, int instNum);
  };
  friend class RunningSequence;
  // Uses the instance number as the index, that should ordinarily work
  std::map<int, RunningSequence> sequences;
  // pointers to RunningSequences need to remain valid
  std::vector<std::string> order;
  static void finishSequence(int retval, Node* node, void* data, int instNum);
protected:
  virtual void startInstance(int instNum);
public:
  SequenceNode(NodeContext& ctx, const sequence_node_t& sequenceElem);
  virtual ~SequenceNode();
  virtual void waitForInstance(int instNum);
};

}
#endif
