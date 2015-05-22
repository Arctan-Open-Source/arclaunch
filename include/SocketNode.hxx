#include "LaunchNode.hxx"
#include <thread>
#include <atomic>

#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>

#ifndef SOCKET_NODE_H_
#define SOCKET_NODE_H_

// Mostly behaves like a LaunchNode with a node generated
namespace arclaunch {
class SocketNode : public LaunchNode {
  typedef struct ::addrinfo Addr;
private:
  socket_node_t::socket_sequence seq;
  unsigned int port;
  int domain;
  int type;
  int protocol;
  addrinfo* res;
  void acceptConnections(Addr* addr);
  // the threads used for accepting connections
  std::vector<std::thread> thrs;
  std::atomic<bool> keep;
public:
  SocketNode(NodeContext& ctx, const socket_node_t& elem);
  virtual ~SocketNode();
  virtual void startup();
  // need to choose a port and an interface, domain name, or an address
  virtual void waitFor();
};

}
#endif
