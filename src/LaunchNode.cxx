#include "LaunchNode.hxx"
#include "NodeContext.hxx"
#include <exception>

namespace arclaunch {

// Used when a node fails construction
class NodeConstructionError : std::exception {
private:
  static const char* msg;
public:
  NodeConstructionError() noexcept;
  virtual ~NodeConstructionError();
  const char* what() const noexcept;
};

const char* NodeConstructionError::msg("Failed to find proper node constructor");

NodeConstructionError::NodeConstructionError() noexcept {
}

NodeConstructionError::~NodeConstructionError() {
}

const char* NodeConstructionError::what() const noexcept {
  return msg;
}


// LaunchNode
LaunchNode::LaunchNode(NodeContext& ctx, const launch_t& launchElem) {
  // open pipes for stdin, stdout, and stderr mapping
  int success;
  // Emplace nodes based on the launch element's children
  for(launch_t::node_const_iterator it = launchElem.node().begin(); 
    it != launchElem.node().end(); it++) {
    const node_t* oc = &(*it);
    Node* points = &ctx.execute(*it);
    if(points)
      nodes[it->name()] = points;
    else {
      throw NodeConstructionError();
    }
  }
  // copy the linkage information from the launch element
  links = launchElem.linkage();
}

LaunchNode::~LaunchNode() {}

void LaunchNode::startup() {
  // Starting up repeatedly without issue should now be possible
  
  // Configure linkage between nodes
  for(launch_t::linkage_const_iterator it = links.begin();
    it < links.end(); it++) {
    // Can only link files and programs from the same launch node
    int link[2];
    pipe2(link, O_CLOEXEC);
    getNode(it->from()).linkFd(it->from_fd(), link[1]);
    getNode(it->to()).linkFd(it->to_fd(), link[0]);
    close(link[0]);
    close(link[1]);
  }

  // Start all of the sub-nodes
  for(node_iterator it = nodes.begin(); it != nodes.end(); it++) {
    it->second->startup();
  }
  // linkage is cleared by startup
  closeFds();
}

bool LaunchNode::isRunning() const {
  for(const_node_iterator it = nodes.begin(); it != nodes.end(); it++) {
    if(it->second->isRunning()) {
      return true;
    }
  }
  return false;
}

void LaunchNode::waitFor() {
  for(const_node_iterator it = nodes.begin(); it != nodes.end(); it++) {
    it->second->waitFor();
  }
}

Node& LaunchNode::getNode(std::string name) {
  return *nodes[name];
}

}
