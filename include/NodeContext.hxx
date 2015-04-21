#include "Node.hxx"
#include "NodeStorage.hxx"

#ifndef _NODE_CONTEXT_HXX_
#define _NODE_CONTEXT_HXX_

namespace arclaunch {
class NodeContext {
private:
  // These are used as memory containers for the different types of nodes
  std::vector<NodeStorage*> storages;
  std::vector<Node*> nodes;
public:
  virtual ~NodeContext();
  // Registers a type of node storage
  void recognize(NodeStorage* store);
  // Executes a node based on its dynamic type
  Node* execute(file_t* executableElem);
};

// Use a templatized notify function
template<class Derived, class Contained>
void notify(NodeContext& ctx) {
  // Create a local class
  class Storage : public NodeStorage {
  private:
    std::vector<Derived> nodes;
  public:
    virtual ~Storage() {};
    Node* construct(NodeContext& ctx, file_t* gNode) {
      if(Contained* sNode = dynamic_cast<Contained*>(gNode)) {
        nodes.emplace_back(ctx, sNode);
        return &nodes.back();
      }
      return NULL;
    }
  };
  // This is where memory for all the nodes are allocated
  static Storage nodeStorage;
  ctx.recognize(&nodeStorage);
}

}
#endif
