#include "Node.hxx"

#ifndef GROUP_NODE_HXX_
#define GROUP_NODE_HXX_

namespace arclaunch {

// An abstract node for nodes that use groups of nodes
class GroupNode : public Node {
protected:
  typedef std::map<std::string, Node*>::iterator node_iterator;
  typedef std::map<std::string, Node*>::const_iterator const_node_iterator;
  std::map<std::string, Node*> nodes;
public:
  GroupNode(NodeContext& ctx, const group_t& groupElem);
  virtual ~GroupNode();
  virtual Node& getNode(std::string name);
};
// Used when a node fails construction
class NodeConstructionError : std::exception {
private:
  static const char* msg;
public:
  NodeConstructionError() noexcept;
  virtual ~NodeConstructionError();
  const char* what() const noexcept;
};

}

#endif
