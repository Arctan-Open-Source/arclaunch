#include "gtest/gtest.h"
#include "arclaunch.hxx"
#include "locations.hxx"
#include "interpret_error.h"
#include "drain_fd.h"

// This is a test class for Node.hxx
// ensures that executables do in-fact get started up

class NodeTest : public testing::Test {
protected:
  std::unique_ptr<launch_t> argv_file;
  arclaunch::LaunchNode* elem;
  arclaunch::Node* subElem;
  int outFd;
  int errFd;
  virtual void SetUp();
  virtual void TearDown();
};

void NodeTest::SetUp() {
  argv_file = launch(ARGV_LAUNCH);
  elem = dynamic_cast<arclaunch::LaunchNode*>(&arclaunch::context().execute(*argv_file));
  ASSERT_TRUE(NULL != elem);
  subElem = &elem->getNode("argv");
  errFd = dup(subElem->getStderr());
  outFd = dup(subElem->getStdout());
  fcntl(outFd, F_SETFD, FD_CLOEXEC);
  fcntl(errFd, F_SETFD, FD_CLOEXEC);
  elem->startup();
}

void NodeTest::TearDown() {
  close(errFd);
  close(outFd);
}

TEST_F(NodeTest, run_argv) {
  // The major issue right now is my inability to access subprocesses
  // The solution is to add subprocess access to the LaunchNode class
  // Access a subprocess
  // Let the process complete
  subElem->waitFor();
  std::string empty("");
  std::string err(drainFd(errFd));
  std::string out(drainFd(outFd));
  EXPECT_EQ(empty, err);
  EXPECT_NE(empty, out);
}
