#include "gtest/gtest.h"
#include "arclaunch.hxx"
#include "locations.hxx"
#include "interpret_error.h"
#include "drain_fd.h"

// This is a test class for Node.hxx
// ensures that executables do in-fact get started up

class NodeTest : public testing::Test {
protected:
  std::unique_ptr<launch_t> env_file;
  arclaunch::LaunchNode* elem;
  arclaunch::Node* subElem;
  int linkStdout[2];
  int linkStderr[2];
  void connectPipes();
  virtual void SetUp();
  virtual void TearDown();
};

void NodeTest::SetUp() {
  env_file = launch(ENV_LAUNCH);
  elem = dynamic_cast<arclaunch::LaunchNode*>(&arclaunch::context().execute(*env_file));
  ASSERT_TRUE(NULL != elem);
  subElem = &elem->getNode("env");
}

void NodeTest::TearDown() {
  close(linkStdout[0]);
  close(linkStderr[0]);
}

void NodeTest::connectPipes() {
  pipe2(linkStdout, O_CLOEXEC);
  pipe2(linkStderr, O_CLOEXEC);
  subElem->linkStdout(linkStdout[1]);
  subElem->linkStderr(linkStderr[1]);
  close(linkStdout[1]);
  close(linkStderr[1]);
}

TEST_F(NodeTest, run_env) {
  connectPipes();
  // Let the process complete
  elem->startup();
  subElem->waitFor();
  // Ensure that nothing is read from stderr
  std::string empty("");
  std::string err(drainFd(linkStderr[0]));
  EXPECT_EQ(empty, err);
  std::string out(drainFd(linkStdout[0]));
  EXPECT_EQ("test environment variable", out);
}

TEST_F(NodeTest, repeat_env) {
  std::string empty("");
  connectPipes();
  elem->startup();
  elem->waitFor();
  std::string err(drainFd(linkStderr[0]));
  EXPECT_EQ(empty, err);
  std::string out(drainFd(linkStdout[0]));
  EXPECT_EQ("test environment variable", out);
  connectPipes();
  elem->startup();
  elem->waitFor();
  err = drainFd(linkStderr[0]);
  EXPECT_EQ(empty, err);
  out = drainFd(linkStdout[0]);
  EXPECT_EQ("test environment variable", out);
}
