#include "Node.hxx"
#include "ExecutableNode.hxx"
#ifndef _SEQUENCE_NODE_HXX_
#define _SEQUENCE_NODE_HXX_

// A sequence node takes a set of programs and runs them in serial

// Given that each program returns 0 instead of 1, the programs are run one after the other
// using the same file handles for each of the programs

// If one of the programs returns 1, then the chain breaks and the sequence of programs stops altogether
namespace arclaunch {

class SequenceNode : public Node {
private:
  std::map<std::string, Node*> nodes;
public:
  SequenceNode(NodeContext& ctx, const sequence_node_t& sequenceElem);
  virtual ~SequenceNode();
  virtual void startup();
  virtual bool isRunning() const;
  virtual void waitFor();
};

}
#endif
