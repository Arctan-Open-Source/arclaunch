#include "gtest/gtest.h"
#include "arclaunch.hxx"
#include "locations.hxx"
#include "interpret_error.h"
#include "drain_fd.h"

// This is a test class for Node.hxx
// ensures that executables do in-fact get started up

class NodeTest : public testing::Test {
protected:
  std::unique_ptr<launch_t> stdout_file;
  arclaunch::LaunchNode* elem;
  arclaunch::Node* subElem;
  int outFd;
  int errFd;
  virtual void SetUp();
  virtual void TearDown();
};

void NodeTest::SetUp() {
  stdout_file = launch(STDOUT_LAUNCH);
  elem = dynamic_cast<arclaunch::LaunchNode*>(&arclaunch::context().execute(*stdout_file));
  ASSERT_TRUE(NULL != elem);
  subElem = &elem->getNode("stdout");
  errFd = dup(subElem->getStderr());
  outFd = dup(subElem->getStdout());
  fcntl(errFd, F_SETFD, FD_CLOEXEC);
  fcntl(outFd, F_SETFD, FD_CLOEXEC);
  elem->startup();
}

void NodeTest::TearDown() {
  close(errFd);
  close(outFd);
}

TEST_F(NodeTest, run_stdout) {
  // Let the process complete
  subElem->waitFor();
  // Ensure that stdout works
  std::string empty("");
  std::string err(drainFd(errFd));
  std::string out(drainFd(outFd));
  EXPECT_EQ(empty, err);
  EXPECT_EQ("STDOUT TEST", out);
}
