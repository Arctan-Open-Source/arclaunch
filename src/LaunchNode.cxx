#include "LaunchNode.hxx"
#include "NodeContext.hxx"

namespace arclaunch {
// LaunchNode
LaunchNode::LaunchNode(NodeContext& ctx, launch_t* launchElem, executable_t::arg_sequence& args, executable_t::env_sequence& envs) {
  // Emplace nodes based on the launch element's children
  for(launch_t::executable_iterator it = launchElem->executable().begin(); 
    it < launchElem->executable().end(); it++) {
    nodes.push_back(ctx.execute(&(*it), args, envs));
  }
}

LaunchNode::~LaunchNode() {}

pid_t LaunchNode::getPid() const {
  return 0;
}

bool LaunchNode::isRunning() const {
  for(const_node_iterator it = nodes.begin(); it != nodes.end(); it++) {
    if((*it)->isRunning()) {
      return true;
    }
  }
  return false;
}

void LaunchNode::waitFor() const {
  for(const_node_iterator it = nodes.begin(); it != nodes.end(); it++) {
    (*it)->waitFor();
  }
}

int LaunchNode::stdin_fd() const {
  return in_fd;
}

int LaunchNode::stdout_fd() const {
  return out_fd;
}

int LaunchNode::stderr_fd() const {
  return err_fd;
}

}
