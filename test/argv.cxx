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
  int outLink[2];
  int errLink[2];
  virtual void SetUp();
  virtual void TearDown();
};

void NodeTest::SetUp() {
  argv_file = launch(ARGV_LAUNCH);
  elem = dynamic_cast<arclaunch::LaunchNode*>(&arclaunch::context().execute(*argv_file));
  ASSERT_TRUE(NULL != elem);
  subElem = &elem->getNode("argv");
  pipe2(outLink, O_CLOEXEC);
  pipe2(errLink, O_CLOEXEC);
  // Pass in the write ends of the pipe
  subElem->linkStdout(outLink[1]);
  subElem->linkStderr(errLink[1]);
  // Close the now unnecessary write descriptors
  close(outLink[1]);
  close(errLink[1]);
  elem->startup();
}

void NodeTest::TearDown() {
  // close the read ends of the pipes
  close(outLink[0]);
  close(errLink[0]);
}

TEST_F(NodeTest, run_argv) {
  // The major issue right now is my inability to access subprocesses
  // The solution is to add subprocess access to the LaunchNode class
  // Access a subprocess
  // Let the process complete
  subElem->waitFor();
  std::string empty("");
  std::string out(drainFd(outLink[0]));
  std::string err(drainFd(errLink[0]));
  EXPECT_EQ(empty, err);
  EXPECT_NE(empty, out);
}
