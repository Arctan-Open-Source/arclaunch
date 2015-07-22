#include "LaunchNode.hxx"
#include "NodeContext.hxx"
#include <exception>

namespace arclaunch {

// private methods
void LaunchNode::onSubNodeDeath(int retval, Node* subNode, 
  void* launchInst, int instNum) {
  LaunchInstance* self = static_cast<LaunchInstance*>(launchInst);
  self->nodeInstances.erase(subNode);
  if(self->nodeInstances.empty()) {
    // dispatch finishInstance
    self->owner->finishInstance(self->instNum, 0);
    self->owner->instances.erase(self->instNum);
  }
}

// protected methods
void LaunchNode::startInstance(int instNum) {
  // Starting up repeatedly without issue should now be possible
  
  // Configure linkage between nodes
  for(launch_t::linkage_const_iterator it = links.begin();
    it < links.end(); ++it) {
    int link[2];
    pipe2(link, O_CLOEXEC);
    getNode(it->to()).linkFd(it->to_fd(), link[0], true);
    getNode(it->from()).linkFd(it->from_fd(), link[1], false);
    close(link[0]);
    close(link[1]);
  }

  // Configure externalization
  for(launch_t::externalize_const_iterator it = externals.begin();
    it != externals.end(); ++it)
    passFd(getNode(it->node()), it->external(), it->internal());

  instances.emplace(
    std::piecewise_construct, 
    std::forward_as_tuple(instNum), 
    std::forward_as_tuple());
  LaunchInstance* inst = &instances[instNum];
  inst->owner = this;
  inst->instNum = instNum;
  // Start all of the sub-nodes
  for(node_iterator it = nodes.begin(); it != nodes.end(); it++) {
    // Hold onto the instance numbers for each node
    // Add a callback to the instance to make detecting closure easier
    it->second->addInstanceCompletionHandler(onSubNodeDeath, inst);
    inst->nodeInstances[it->second] = it->second->startup();
  }
  // Special case of no subNodes immediately closes out
  if(inst->nodeInstances.empty())
  {
    finishInstance(instNum, 0);
    instances.erase(instNum);
  }
}

// public methods
// LaunchNode
LaunchNode::LaunchNode(NodeContext& ctx, const launch_t& launchElem) :
  GroupNode(ctx, launchElem) {
  // copy the linkage information from the launch element
  links = launchElem.linkage();
  externals = launchElem.externalize();
}

LaunchNode::~LaunchNode() {}


void LaunchNode::waitForInstance(int instNum) {
  while(instanceIsRunning(instNum)) {
    std::map<Node*, int>::iterator subInst = 
      instances[instNum].nodeInstances.begin();
    subInst->first->waitForInstance(subInst->second);
  }
}

}
