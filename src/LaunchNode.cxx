#include "LaunchNode.hxx"
#include "NodeContext.hxx"
#include <exception>

#include <iostream>

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
      std::cerr << "Failed to find proper node constructor" << std::endl;
      throw NodeConstructionError();
    }
  }
  // Configure linkage between nodes
  for(launch_t::linkage_const_iterator it = launchElem.linkage().begin();
    it < launchElem.linkage().end(); it++) {
    // Can only link files and programs from the same launch node
    int fd = getNode(it->from()).getStdout();
    getNode(it->to()).linkStdin(fd);
  }
}

LaunchNode::~LaunchNode() {}

void LaunchNode::startup() {
  // TODO: configure pipes from the launch node's stdin and stdout

  // Start all of the sub-nodes
  for(node_iterator it = nodes.begin(); it != nodes.end(); it++) {
    it->second->startup();
  }
}

bool LaunchNode::isRunning() const {
  for(const_node_iterator it = nodes.begin(); it != nodes.end(); it++) {
    if(it->second->isRunning()) {
      return true;
    }
  }
  return false;
}

void LaunchNode::waitFor() const {
  for(const_node_iterator it = nodes.begin(); it != nodes.end(); it++) {
    it->second->waitFor();
  }
}

void LaunchNode::linkStdin(int fd) {
  // TODO: setup a working connection
}

int LaunchNode::getStdout() {
  // TODO: setup stdout aggregation
  return 0;
}

int LaunchNode::getStderr() {
  // TODO: setup stderr aggregation
  return 0;
}

Node& LaunchNode::getNode(std::string name) {
  return *nodes[name];
}

}
