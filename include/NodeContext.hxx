#include "Node.hxx"
#include "NodeStorage.hxx"
#include <list>

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
  void recognize(NodeStorage& store);
  // Executes a node based on its dynamic type
  Node& execute(const file_t& executableElem);
};

// Use a templatized notify function
template<class Derived, class Contained>
void notify(NodeContext& ctx) {
  // Create a local class
  class Storage : public NodeStorage {
  private:
    // Need to use a "list" instead of "vector" to prevent reallocation
    // Could use forward list or some other container since access past use of emplace_back and destruction isn't really necessary
    // Cleaning up finished nodes
    // TODO: clean up finished nodes by some mechanism
    // TODO: a reference counting pointer might be an okay mechanism in this case
    // TODO: a pointer-wrapper object could be used to track that
    // TODO: this centralized storage would not even be necessary with good garbage collection
    std::list<Derived> nodes;
  public:
    virtual ~Storage() {};
    Node* construct(NodeContext& ctx, const file_t& gNode) {
      if(const Contained* sNode = dynamic_cast<const Contained*>(&gNode)) {
        nodes.emplace_back(ctx, *sNode);
        return &nodes.back();
      }
      return NULL;
    }
  };
  // This is where memory for all the nodes are allocated
  static Storage nodeStorage;
  ctx.recognize(nodeStorage);
}

}
#endif
