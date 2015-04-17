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
  Node* execute(executable_t* executableElem, 
    executable_t::arg_sequence args = executable_t::arg_sequence(), 
    executable_t::env_sequence envs = executable_t::env_sequence());
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
    Node* construct(NodeContext& ctx, executable_t* gNode, executable_t::arg_sequence& args, executable_t::env_sequence& envs) {
      if(Contained* sNode = dynamic_cast<Contained*>(gNode)) {
        nodes.emplace_back(ctx, sNode, args, envs);
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
