#include "gtest/gtest.h"
#include "arclaunch.hxx"
#include "locations.hxx"
#include "drain_fd.h"

// This is a test class for SequenceNode.hxx
// Makes sure that the sequence passes correctly

class NodeTest : public testing::Test {
protected:
  std::unique_ptr<launch_t> sequence_file;
  arclaunch::LaunchNode* elem;
  arclaunch::Node* subElem;
  int outLink[2];
  int errLink[2];
  void connectPipes();
  virtual void SetUp();
  virtual void TearDown();
};

void NodeTest::SetUp() {
  sequence_file = launch(SEQUENCE_LAUNCH);
  elem = dynamic_cast<arclaunch::LaunchNode*>(&arclaunch::context().execute(*sequence_file));
  ASSERT_TRUE(NULL != elem);
  subElem = &elem->getNode("sequence");
}

void NodeTest::TearDown() {
  // close the read ends of the pipes
  close(outLink[0]);
  close(errLink[0]);
}

void NodeTest::connectPipes() {
  pipe2(outLink, O_CLOEXEC);
  pipe2(errLink, O_CLOEXEC);
  // pass in the write ends of the pipe
  subElem->linkStdout(outLink[1]);
  subElem->linkStderr(errLink[1]);
  // Close the now unneeded file descriptors
  close(outLink[1]);
  close(errLink[1]);
}

TEST_F(NodeTest, run_sequence) {
  connectPipes();
  elem->startup();
  elem->waitFor();
  std::string empty("");
  std::string out(drainFd(outLink[0]));
  std::string err(drainFd(errLink[0]));
  EXPECT_EQ(empty, err);
  // TODO: fix newlines to not be linux specific
  EXPECT_EQ("first node\nsecond node\n", out);
}

