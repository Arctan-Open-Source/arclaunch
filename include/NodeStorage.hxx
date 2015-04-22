#include "Node.hxx"
#ifndef _NODE_STORAGE_HXX_
#define _NODE_STORAGE_HXX_

namespace arclaunch {
// An abstract class for Node storage
class NodeStorage {
public:
  virtual ~NodeStorage();
  virtual Node* construct(NodeContext& ctx, const file_t& gNode) = 0;
};

}
#endif
