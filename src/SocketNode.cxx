#include "SocketNode.hxx"
#include <exception>

namespace arclaunch {

SocketNode::SocketNode(NodeContext& ctx, const socket_node_t& elem) :
  LaunchNode(ctx, elem) {
}

SocketNode::~SocketNode() {
}

void SocketNode::startup() {
  for(socket_node_t::socket_iterator it = seq.begin(); it != seq.end(); ++it) {
    // Configure linkage between the socket and the nodes
    if(it->from() == "socket") {
      getNode(it->to()).linkStdin(sockFd);
    }
    else if(it->to() == "socket") {
      getNode(it->from()).linkStdout(sockFd);
    } else
      throw std::exception();
  }
  LaunchNode::startup();
}

void SocketNode::waitFor() const {
  // TODO: reimplement to receive socket connections and automatedly startup as connections are received until
  // given a command to stop
  LaunchNode::waitFor();
}

}
