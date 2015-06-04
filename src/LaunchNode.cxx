#include "LaunchNode.hxx"
#include "NodeContext.hxx"
#include <exception>

namespace arclaunch {

// LaunchNode
LaunchNode::LaunchNode(NodeContext& ctx, const launch_t& launchElem) :
  GroupNode(ctx, launchElem) {
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
    getNode(it->from()).linkStdout(link[1]);
    getNode(it->to()).linkStdin(link[0]);
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

}
