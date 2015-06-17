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
  int outLink[2];
  int errLink[2];
  int outLink2[2];
  int errLink2[2];
  void connectPipes(int *out, int *err);
  virtual void SetUp();
  virtual void TearDown();
};

void NodeTest::SetUp() {
  stdout_file = launch(STDOUT_LAUNCH);
  elem = dynamic_cast<arclaunch::LaunchNode*>(&arclaunch::context().execute(*stdout_file));
  ASSERT_TRUE(NULL != elem);
  subElem = &elem->getNode("stdout");
}

void NodeTest::TearDown() {
  close(outLink[0]);
  close(errLink[0]);
}

void NodeTest::connectPipes(int *out, int *err) {
  pipe2(out, O_CLOEXEC);
  pipe2(err, O_CLOEXEC);
  subElem->linkStdout(out[1]);
  subElem->linkStderr(err[1]);
  close(out[1]);
  close(err[1]);
}

TEST_F(NodeTest, run_stdout) {
  connectPipes(outLink, errLink);
  elem->startup();
  // Let the process complete
  subElem->waitFor();
  // Ensure that stdout works
  std::string empty("");
  std::string out(drainFd(outLink[0]));
  std::string err(drainFd(errLink[0]));
  EXPECT_EQ(empty, err);
  EXPECT_EQ("STDOUT TEST", out);
}

// Test restarting the launch file
TEST_F(NodeTest, repeat_stdout) {
  std::string empty("");
  // Go through the whole process
  connectPipes(outLink, errLink);
  elem->startup();
  subElem->waitFor();
  // Ensure that stdout works
  std::string out(drainFd(outLink[0]));
  std::string err(drainFd(errLink[0]));
  EXPECT_EQ(empty, err);
  EXPECT_EQ("STDOUT TEST", out);
  // Repeat the process
  connectPipes(outLink, errLink);
  elem->startup();
  subElem->waitFor();
  out = drainFd(outLink[0]);
  err = drainFd(errLink[0]);
  EXPECT_EQ(empty, err);
  EXPECT_EQ("STDOUT TEST", out);
}

// Test running two instances of the launch file in parallel
TEST_F(NodeTest, parallel_stdout) {
  std::string empty("");
  // Start the process twice, linked to two different sets of pipes
  connectPipes(outLink, errLink);
  elem->startup();
  connectPipes(outLink2, errLink2);
  elem->startup();
  subElem->waitFor();
  std::string out(drainFd(outLink[0]));
  std::string err(drainFd(errLink[0]));
  std::string out2(drainFd(outLink2[0]));
  std::string err2(drainFd(errLink2[0]));
  EXPECT_EQ(empty, err);
  EXPECT_EQ("STDOUT TEST", out);
  EXPECT_EQ(empty, err2);
  EXPECT_EQ("STDOUT TEST", out2);
}
