#include "SocketNode.hxx"
#include <exception>
#include <sys/select.h>

namespace arclaunch {

SocketNode::SocketNode(NodeContext& ctx, const socket_node_t& elem) :
  LaunchNode(ctx, elem) {
  // The largest numbered accepting socket needs to start at 0
  maxfd = 0;
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
  int err; // error code storage
  // TODO: add limits to the XSD file to limit the acceptable strings for address and service
  // check for standard solutions to this problem in the XSD specification
  if(elem.service().present()) // Derive information from the address and service if present
    err = getaddrinfo(elem.address().c_str(), elem.service().get().c_str(), &hint, &res);
  else // Derive information just from the address
    err = getaddrinfo(elem.address().c_str(), NULL, &hint, &res);
  
  if(err) {
    // getaddrinfo failed
    // TODO throw a more descriptive exception
    throw std::exception();
  }
}

SocketNode::~SocketNode() {
  waitFor();
  freeaddrinfo(res);
}

// Can be called multiple times to accept on even more linked lists of addresses
void SocketNode::prepareAccept(Addr* addr) {
  Addr* anAddr;
  anAddr = addr;
  do {
    int fd;
    sockaddr_in* s4;
    sockaddr_in6* s6;
    // TODO: SOCK_CLOEXEC is linux specific
    fd = socket(anAddr->ai_family, anAddr->ai_socktype | SOCK_CLOEXEC, anAddr->ai_protocol);
    if(fd > maxfd)
      maxfd = fd;
    bind(fd, anAddr->ai_addr, anAddr->ai_addrlen);
    // uses backlog of 20
    listen(fd, 20);
    fds.push_back(fd);
  } while(anAddr = anAddr->ai_next);
}

void SocketNode::acceptConnections() {
  fd_set read_set;
  // start accepting connections
  // the accepted address
  int numAccSock = 0;
  // close the file descriptor on exec
  struct timeval tout = {0, 500};
  do {
    for(size_t c = 0; c < fds.size(); c++) {
      if(numAccSock == 0)
        break;
      int accSock = fds[c];
      if(!FD_ISSET(accSock, &read_set))
        continue;
      numAccSock--;
      sockaddr accAddr;
      socklen_t addrlen;
      int sockFd = accept4(accSock, &accAddr, &addrlen, SOCK_CLOEXEC);
      // Configure linkage between the socket and the nodes
      for(socket_node_t::socket_iterator it = seq.begin(); it != seq.end(); ++it) {
        if(it->from() == "socket")
          getNode(it->to()).linkFd(it->from_fd(), sockFd);
        else if(it->to() == "socket")
          getNode(it->from()).linkFd(it->to_fd(), sockFd);
      }
      for(std::vector<int>::iterator it = fds.begin(); it != fds.end(); ++it)
        FD_SET(*it, &read_set);
      // Use LaunchNode version of startup
      LaunchNode::startup();
    }
  } while(keep && (numAccSock = select(maxfd + 1, &read_set, NULL, NULL, &tout)));
}

void SocketNode::startup() {
  // Start the accepting sockets
  // Create an accepting socket
  prepareAccept(res);
  // start the thread
  thrs.emplace_back(&SocketNode::acceptConnections, this);
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
