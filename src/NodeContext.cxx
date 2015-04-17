#include "NodeContext.hxx"

namespace arclaunch {

// NodeContext
void NodeContext::recognize(NodeStorage* store) {
  storages.push_back(store);
}

// Assumes that notifications about the least specific elements are submitted first
Node* NodeContext::execute(executable_t* elem, executable_t::arg_sequence args, executable_t::env_sequence envs) {
  Node* node;
  for(std::vector<NodeStorage*>::reverse_iterator it = storages.rbegin();
    it != storages.rend(); it++)
  {
    if((node = (*it)->construct(*this, elem, args, envs)) != NULL)
    {
      // Store a reference to the created node
      nodes.push_back(node);
      break;
    }
  }
  // Return a reference to the created node or null
  return node;
}

NodeContext::~NodeContext() {}

}
