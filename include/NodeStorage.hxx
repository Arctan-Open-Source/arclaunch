#include "Node.hxx"
#ifndef _NODE_STORAGE_HXX_
#define _NODE_STORAGE_HXX_

namespace arclaunch {
// An abstract class for Node storage
class NodeStorage {
public:
  virtual ~NodeStorage();
  virtual Node* construct(NodeContext& ctx, executable_t* gNode, executable_t::arg_sequence& args, executable_t::env_sequence& envs) = 0;
};

}
#endif
