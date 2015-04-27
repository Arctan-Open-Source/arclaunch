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
  virtual void SetUp();
  virtual void TearDown();
};

void NodeTest::SetUp() {
  stdout_file = launch(STDOUT_LAUNCH);
  elem = dynamic_cast<arclaunch::LaunchNode*>(&arclaunch::context().execute(*stdout_file));
  ASSERT_TRUE(NULL != elem);
  elem->startup();
  subElem = &elem->getNode("stdout");
}

void NodeTest::TearDown() {
}

TEST_F(NodeTest, run_stdout) {
  // Let the process complete
  subElem->waitFor();
  // Ensure that stdout works
  std::string empty("");
  std::string err(drainFd(subElem->getStderr()));
  std::string out(drainFd(subElem->getStdout()));
  EXPECT_EQ(empty, err);
  EXPECT_EQ("STDOUT TEST", out);
}
