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
  void connectPipes();
  virtual void SetUp();
  virtual void TearDown();
};

void NodeTest::SetUp() {
  argv_file = launch(ARGV_LAUNCH);
  elem = dynamic_cast<arclaunch::LaunchNode*>(&arclaunch::context().execute(*argv_file));
  ASSERT_TRUE(NULL != elem);
  subElem = &elem->getNode("argv");
}

void NodeTest::TearDown() {
  // close the read ends of the pipes
  close(outLink[0]);
  close(errLink[0]);
}

void NodeTest::connectPipes() {
  pipe2(outLink, O_CLOEXEC);
  pipe2(errLink, O_CLOEXEC);
  // Pass in the write ends of the pipe
  subElem->linkStdout(outLink[1]);
  subElem->linkStderr(errLink[1]);
  // Close the now unnecessary write descriptors
  close(outLink[1]);
  close(errLink[1]);
}

TEST_F(NodeTest, run_argv) {
  // Let the process complete
  connectPipes();
  elem->startup();
  elem->waitFor();
  std::string empty("");
  std::string out(drainFd(outLink[0]));
  std::string err(drainFd(errLink[0]));
  EXPECT_EQ(empty, err);
  EXPECT_NE(empty, out);
}

TEST_F(NodeTest, repeat_argv) {
  std::string empty("");
  // Let the process complete
  connectPipes();
  elem->startup();
  elem->waitFor();
  std::string out(drainFd(outLink[0]));
  std::string err(drainFd(errLink[0]));
  EXPECT_EQ(empty, err);
  EXPECT_NE(empty, out);
  // repeat everything
  connectPipes();
  elem->startup();
  elem->waitFor();
  out = drainFd(outLink[0]);
  err = drainFd(errLink[0]);
  EXPECT_EQ(empty, err);
  EXPECT_NE(empty, out);
}
