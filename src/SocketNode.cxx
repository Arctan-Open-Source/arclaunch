#include "SocketNode.hxx"
#include <exception>


namespace arclaunch {

SocketNode::SocketNode(NodeContext& ctx, const socket_node_t& elem) :
  LaunchNode(ctx, elem) {
  Addr hint;
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
  if(elem.service().present()) // Derive information from the address and service if present
    err = getaddrinfo(elem.address().c_str(), elem.service().get().c_str(), &hint, &res);
  else // Derive information just from the address
    err = getaddrinfo(elem.address().c_str(), NULL, &hint, &res);
  
  if(err) {
    // getaddrinfo failed
  }
}

SocketNode::~SocketNode() {
  waitFor();
  freeaddrinfo(res);
}

void SocketNode::acceptConnections(Addr* addr) {
  int fd = socket(addr->ai_family, addr->ai_socktype, addr->ai_protocol);
  // start accepting connections
  int sockFd;
  bind(fd, addr->ai_addr, addr->ai_addrlen);
  // uses backlog of 20
  listen(fd, 20);
  // the accepted address
  sockaddr accAddr;
  socklen_t addrlen;
  // close the file descriptor on exec
  while(keep && (sockFd = accept4(sockFd, &accAddr, &addrlen, SOCK_CLOEXEC))) {
    // Uses a lock so that different threads won't interfere with each other
    std::unique_lock<std::mutex> forkLock(forkMtx);
    // Configure linkage between the socket and the nodes
    for(socket_node_t::socket_iterator it = seq.begin(); it != seq.end(); ++it) {
      // Configure linkage between the socket and the nodes
      if(it->from() == "socket")
        getNode(it->to()).linkStdin(sockFd);
      else if(it->to() == "socket")
        getNode(it->from()).linkStdout(sockFd);
    }
    // Use LaunchNode version of startup
    LaunchNode::startup();
  }
}

void SocketNode::startup() {
  // Start the accepting sockets
  // Create an accepting socket
  Addr* accAddr;
  accAddr = res;
  do {
    // address needs to be non-null
    if(!accAddr->ai_addr)
      break;
    // start the thread
    thrs.emplace_back(&SocketNode::acceptConnections, this, accAddr);
  } while(accAddr = accAddr->ai_next);
}

void SocketNode::waitFor() {
  // Setup the threads to stop
  keep = false;
  // Wait for the threads to stop cleanly
  for(std::vector<std::thread>::iterator it = thrs.begin(); it != thrs.end(); it++) {
    it->join();
  }
}

}
