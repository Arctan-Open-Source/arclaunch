#include "LaunchNode.hxx"

#ifndef SOCKET_NODE_H_
#define SOCKET_NODE_H_

// Mostly behaves like a LaunchNode with a node generated
namespace arclaunch {
class SocketNode : public LaunchNode {
private:
  socket_node_t::socket_sequence seq;
  char addr[4];
  unsigned int port;
  int sockFd;

  int domain;
  int type;
  int protocol;
public:
  SocketNode(NodeContext& ctx, const socket_node_t& elem);
  virtual ~SocketNode();
  virtual void startup();
  // need to choose a port and an interface, domain name, or an address
  virtual void waitFor() const;
};

}
#endif
