#include "SocketNode.hxx"
#include <exception>

#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>

namespace arclaunch {

SocketNode::SocketNode(NodeContext& ctx, const socket_node_t& elem) :
  LaunchNode(ctx, elem) {
  struct addrinfo* res;
  struct addrinfo hint;
  // No hint flags
  hint.ai_flags = 0;
  // Seek either IPv6 or IPv4
  hint.ai_family = AF_UNSPEC;
  // Use streams only
  hint.ai_socktype = SOCK_STREAM;
  // Use protocol 0
  hint.ai_protocol = 0;
  // Need to set the other fields to NULL
  hint.ai_addrlen = 0;
  hint.ai_addr = NULL;
  hint.ai_canonname = NULL;
  hint.ai_next = NULL;
  port = 0; // use ephemeral port unless set
  if(elem.port().present())
    port = elem.port().get();
  int err; // error code storage
  // TODO: add limits to the XSD file to limit the acceptable strings for address and service
  // check for standard solutions to this problem in the XSD specification
  if(elem.service().present()) {
    // Derive information from the address and service if present
    err = getaddrinfo(elem.address().c_str(), elem.service().get().c_str(), &hint, &res);
  } else {
    // Derive information just from the address
    err = getaddrinfo(elem.address().c_str(), NULL, &hint, &res);
  }
  if(err) {
    // getaddrinfo failed
  } else {
    freeaddrinfo(res);
  }
}

SocketNode::~SocketNode() {
}

void SocketNode::startup() {
  // Decipher the 
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
  // Create an accepting socket
  
  LaunchNode::startup();
}

void SocketNode::waitFor() const {
  // TODO: reimplement to receive socket connections and automatedly startup as connections are received until
  // given a command to stop
  // TODO: create sockets for each result from getaddrinfo and listen on all of them
  int accSock;
  LaunchNode::waitFor();
}

}
