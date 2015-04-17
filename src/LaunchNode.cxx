#include "LaunchNode.hxx"
#include "NodeContext.hxx"

namespace arclaunch {
// LaunchNode
LaunchNode::LaunchNode(NodeContext& ctx, launch_t* launchElem, executable_t::arg_sequence& args, executable_t::env_sequence& envs) {
  // open pipes for stdin, stdout, and stderr mapping
  int success;
  int stdinPipes[2];
  int stdoutPipes[2];
  int stderrPipes[2];
  // TODO: throw an error if opening a pipe fails
  success = pipe(stdinPipes);
  success = pipe(stdoutPipes);
  success = pipe(stderrPipes);
  // transmitting end of stdin
  inFd = stdinPipes[1];
  // receiving end of stdout
  outFd = stdoutPipes[0];
  // receiving end of stderr
  errFd = stderrPipes[0];
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

int LaunchNode::stdinFd() const {
  return inFd;
}

int LaunchNode::stdoutFd() const {
  return outFd;
}

int LaunchNode::stderrFd() const {
  return errFd;
}

}
