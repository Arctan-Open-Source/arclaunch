#include "gtest/gtest.h"
#include "arclaunch.hxx"
#include "locations.hxx"
#include "drain_fd.h"

#include <iostream>

// This is a test class for SequenceNode.hxx
// Makes sure that the sequence passes correctly

class NodeTest : public testing::Test {
protected:
  std::unique_ptr<launch_t> sequence_file;
  arclaunch::LaunchNode* elem;
  arclaunch::Node* subElem;
  int outLink[2];
  int errLink[2];
  int outLink2[2];
  int errLink2[2];
  void connectPipes(int *out, int *err);
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
  close(outLink2[0]);
  close(errLink2[0]);
}

void NodeTest::connectPipes(int* out, int* err) {
  pipe2(out, O_CLOEXEC);
  pipe2(err, O_CLOEXEC);
  // pass in the write ends of the pipe
  subElem->linkStdout(out[1]);
  subElem->linkStderr(err[1]);
  // Close the now unneeded file descriptors
  close(out[1]);
  close(err[1]);
}

TEST_F(NodeTest, run_sequence) {
  connectPipes(outLink, errLink);
  elem->startup();
  elem->waitFor();
  std::string empty("");
  std::string out(drainFd(outLink[0]));
  std::string err(drainFd(errLink[0]));
  // TODO: fix newlines to not be linux specific
  EXPECT_EQ(empty, err);
  EXPECT_EQ("first node\nsecond node\n", out);
}
TEST_F(NodeTest, series_sequence) {
  
  connectPipes(outLink, errLink);
  elem->startup();
  elem->waitFor();
  
  connectPipes(outLink2, errLink2);
  elem->startup();
  elem->waitFor();
  
  // wait for both running cases to stop
  std::string empty("");
  std::string out(drainFd(outLink[0]));
  std::string err(drainFd(errLink[0]));
  std::string out2(drainFd(outLink2[0]));
  std::string err2(drainFd(errLink2[0]));
  // TODO: fix newlines to not be linux specific
  EXPECT_EQ(empty, err);
  EXPECT_EQ("first node\nsecond node\n", out);
  EXPECT_EQ(empty, err2);
  EXPECT_EQ("first node\nsecond node\n", out2);
}

TEST_F(NodeTest, parrallel_sequence) {
  
  connectPipes(outLink, errLink);
  elem->startup();
  connectPipes(outLink2, errLink2);
  elem->startup();
  elem->waitFor();
  std::cout << "PAST WAIT FOR" << std::endl;
  
  // wait for both running cases to stop
  std::string empty("");
  std::string out(drainFd(outLink[0]));
  std::string err(drainFd(errLink[0]));
  std::string out2(drainFd(outLink2[0]));
  std::string err2(drainFd(errLink2[0]));
  // TODO: fix newlines to not be linux specific
  EXPECT_EQ(empty, err);
  EXPECT_EQ("first node\nsecond node\n", out);
  EXPECT_EQ(empty, err2);
  EXPECT_EQ("first node\nsecond node\n", out2);
}
