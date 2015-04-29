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
  virtual void SetUp();
  virtual void TearDown();
};

void NodeTest::SetUp() {
  linkage_file = launch(LINKAGE_LAUNCH);
  elem = dynamic_cast<arclaunch::LaunchNode*>(&arclaunch::context().execute(*linkage_file));
  ASSERT_TRUE(NULL != elem);
  subElemA = &elem->getNode("from");
  subElemB = &elem->getNode("to");
  // Start the processes
  elem->startup();
}

void NodeTest::TearDown() {
}

TEST_F(NodeTest, run_linkage) {
  // Let the processes complete
  elem->waitFor();
  // Ensure that stdin works
  std::string empty("");
  std::string errA(drainFd(subElemA->getStderr()));
  std::string outA(drainFd(subElemA->getStdout()));
  std::string errB(drainFd(subElemB->getStderr()));
  std::string outB(drainFd(subElemB->getStdout()));
  EXPECT_EQ(empty, errA);
  EXPECT_EQ(empty, errB);
  EXPECT_EQ(empty, outA);
  EXPECT_EQ("LINKAGE TEST", outB);
}
