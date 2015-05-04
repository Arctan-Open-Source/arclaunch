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
  launch_t::linkage_const_iterator opening = launchElem.linkage().begin();
  launch_t::linkage_const_iterator closing = launchElem.linkage().end();
  // Construct a "linkage map"
  for(launch_t::linkage_const_iterator it = launchElem.linkage().begin();
    it < launchElem.linkage().end(); it++) {
    // Can only link files and programs from the same launch node
    int fd = getNode(it->from()).getStdout();
    getNode(it->to()).linkStdin(fd);
  }
}

LaunchNode::~LaunchNode() {}

void LaunchNode::startup() {
  // TODO: note that pipe configuration must precede startup
  // Setup pipes
  int stdinPipes[2];
  int stdoutPipes[2];
  int stderrPipes[2];
  if(pipe(stdinPipes) ||
    pipe(stdoutPipes) ||
    pipe(stderrPipes)) {
    // TODO: throw a more descriptive error
    throw std::exception();
  }
  // transmitting end of stdin
  // receiving end of stdout
  // receiving end of stderr

  // Start all of the sub-nodes
  for(node_iterator it = nodes.begin(); it != nodes.end(); it++) {
    it->second->startup();
  }
}

pid_t LaunchNode::getPid() const {
  return 0;
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
