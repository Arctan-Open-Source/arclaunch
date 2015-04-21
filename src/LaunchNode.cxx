#include "LaunchNode.hxx"
#include "NodeContext.hxx"
#include <exception>

namespace arclaunch {
// LaunchNode
LaunchNode::LaunchNode(NodeContext& ctx, launch_t* launchElem) {
  // open pipes for stdin, stdout, and stderr mapping
  int success;
  // Emplace nodes based on the launch element's children
  for(launch_t::node_iterator it = launchElem->node().begin(); 
    it < launchElem->node().end(); it++) {
    nodes[it->name()] = ctx.execute(&(*it));
  }
  // Construct a "linkage map"
  for(launch_t::linkage_iterator it = launchElem->linkage().begin();
    it < launchElem->linkage().end(); it++) {
    // TODO: implement linkages
    // Can only link files and programs in the same level
  }
}

LaunchNode::~LaunchNode() {}

void LaunchNode::startup() {
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

}
