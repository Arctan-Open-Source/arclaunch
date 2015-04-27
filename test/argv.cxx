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
  virtual void SetUp();
  virtual void TearDown();
};

void NodeTest::SetUp() {
  argv_file = launch(ARGV_LAUNCH);
  elem = dynamic_cast<arclaunch::LaunchNode*>(&arclaunch::context().execute(*argv_file));
  ASSERT_TRUE(NULL != elem);
  elem->startup();
  subElem = &elem->getNode("argv");
}

void NodeTest::TearDown() {
}

TEST_F(NodeTest, run_argv) {
  // The major issue right now is my inability to access subprocesses
  // The solution is to add subprocess access to the LaunchNode class
  // Access a subprocess
  // Let the process complete
  subElem->waitFor();
  std::string empty("");
  std::string err(drainFd(subElem->getStderr()));
  std::string out(drainFd(subElem->getStdout()));
  EXPECT_EQ(empty, err);
  EXPECT_NE(empty, out);
}
