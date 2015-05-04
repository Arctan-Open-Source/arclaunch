#include "gtest/gtest.h"
#include "arclaunch.hxx"
#include "locations.hxx"
#include "interpret_error.h"
#include "drain_fd.h"

// This is a test class for Node.hxx
// ensures that executables do in-fact get started up

class NodeTest : public testing::Test {
protected:
  std::unique_ptr<launch_t> linkage_file;
  arclaunch::LaunchNode* elem;
  arclaunch::Node *subElemA, *subElemB;
  int outFdA;
  int errFdA;
  int outFdB;
  int errFdB;
  virtual void SetUp();
  virtual void TearDown();
};

void NodeTest::SetUp() {
  linkage_file = launch(LINKAGE_LAUNCH);
  arclaunch::Node* node = &arclaunch::context().execute(*linkage_file);
  elem = dynamic_cast<arclaunch::LaunchNode*>(node);
  ASSERT_TRUE(NULL != elem);
  subElemA = &elem->getNode("from");
  subElemB = &elem->getNode("to");
  ASSERT_TRUE(NULL != subElemA);
  ASSERT_TRUE(NULL != subElemB);
  outFdA = dup(subElemA->getStdout());
  errFdA = dup(subElemA->getStderr());
  outFdB = dup(subElemB->getStdout());
  errFdB = dup(subElemB->getStderr());
  fcntl(outFdA, F_SETFD, FD_CLOEXEC);
  fcntl(errFdA, F_SETFD, FD_CLOEXEC);
  fcntl(outFdB, F_SETFD, FD_CLOEXEC);
  fcntl(errFdB, F_SETFD, FD_CLOEXEC);
  // Start the processes
  elem->startup();
}

void NodeTest::TearDown() {
  close(outFdA);
  close(errFdA);
  close(outFdB);
  close(errFdB);
}

TEST_F(NodeTest, run_linkage) {
  // Let the processes complete
  elem->waitFor();
  // Ensure that stdin works
  std::string empty("");
  std::string errA(drainFd(errFdA));
  std::string outA(drainFd(outFdA));
  std::string errB(drainFd(errFdB));
  std::string outB(drainFd(outFdB));
  EXPECT_EQ(empty, errA);
  EXPECT_EQ(empty, errB);
  EXPECT_EQ(empty, outA);
  EXPECT_EQ("LINKAGE TEST", outB);
}
