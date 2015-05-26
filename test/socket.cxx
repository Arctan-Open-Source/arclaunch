#include "gtest/gtest.h"
#include "arclaunch.hxx"
#include "locations.hxx"
#include "interpret_error.h"
#include "drain_fd.h"

#include <cstring>
#include <sys/types.h>
#include <sys/socket.h>

// This is a test class for Node.hxx
// tests whether socket nodes work or not

class NodeTest : public testing::Test {
protected:
  std::unique_ptr<launch_t> socket_file;
  arclaunch::LaunchNode* elem;
  arclaunch::Node* subElem;
  int transSock;
  virtual void SetUp();
  virtual void TearDown();
};

void NodeTest::SetUp() {
  socket_file = launch(SOCKET_LAUNCH);
  elem = dynamic_cast<arclaunch::LaunchNode*>(&arclaunch::context().execute(*socket_file));
  EXPECT_TRUE(elem != NULL);
  // startup accepting connections
  elem->startup();
}

void NodeTest::TearDown() {
  // cause the server to stop
  elem->waitFor();
}

TEST_F(NodeTest, run_socket) {
  // Create the socket
  int fd = socket(AF_INET, SOCK_STREAM | SOCK_CLOEXEC, 0);
  // connect to the loopback socket
  union {
    char char_data[4];
    uint32_t uint32;
  } addr4;
  addr4.char_data[0] = 127;
  addr4.char_data[1] = 0;
  addr4.char_data[2] = 0;
  addr4.char_data[3] = 1;
  sockaddr_in addr = {
    AF_INET,
    htons(8320), // port 8320
    {
      addr4.uint32
    }
  };
  int res = connect(fd, (sockaddr*) &addr, sizeof(sockaddr_in));
  if(res != 0) {
    switch(errno) {
    case EACCES:
      std::cout << "EACCES" << std::endl;
      break;
    case EPERM:
      std::cout << "EPERM" << std::endl;
      break;
    case EADDRINUSE:
      std::cout << "EADDRINUSE" << std::endl;
      break;
    case EAFNOSUPPORT:
      std::cout << "EAFNOSUPPORT" << std::endl;
      break;
    case EAGAIN:
      std::cout << "EAGAIN" << std::endl;
      break;
    case EALREADY:
      std::cout << "EALREADY" << std::endl;
      break;
    case EBADF:
      std::cout << "EBADF" << std::endl;
      break;
    case ECONNREFUSED:
      std::cout << "ECONNREFUSED" << std::endl;
      break;
    case EFAULT:
      std::cout << "EFAULT" << std::endl;
      break;
    case EINPROGRESS:
      std::cout << "EINPROGRESS" << std::endl;
      break;
    case EINTR:
      std::cout << "EINTR" << std::endl;
      break;
    case EISCONN:
      std::cout << "EISCONN" << std::endl;
      break;
    default:
      std::cout << "default" << std::endl;
      break;
    }
  }
  ASSERT_EQ(res, 0);
  // The server should echo back
  const char *write_buffer = "Hello World";
  write(fd, write_buffer, strlen(write_buffer));
  char read_buffer[128];
  read(fd, read_buffer, strlen(write_buffer));
  close(fd);
}
