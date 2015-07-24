#include "gtest/gtest.h"
#include "arclaunch.hxx"
#include "locations.hxx"
#include "drain_fd.h"

#include <iostream>

class NodeTest : public testing::Test {
protected:
  std::unique_ptr<launch_t> external_file;
  arclaunch::LaunchNode* elem;
  int linkStdin[2];
  int linkStdout[2];
  int linkStderr[2];
  void connectPipes();
  virtual void SetUp();
  virtual void TearDown();
};

void NodeTest::connectPipes() {
  pipe2(linkStdin, O_CLOEXEC);
  pipe2(linkStdout, O_CLOEXEC);
  pipe2(linkStderr, O_CLOEXEC);
  elem->linkStdin(linkStdin[0]);
  elem->linkStdout(linkStdout[1]);
  elem->linkStderr(linkStderr[1]);
  close(linkStdin[0]);
  close(linkStdout[1]);
  close(linkStderr[1]);
}

void NodeTest::SetUp() {
  external_file = launch(EXTERNAL_LAUNCH);
  elem = dynamic_cast<arclaunch::LaunchNode*>(&arclaunch::context().execute(*external_file));
  ASSERT_TRUE(NULL != elem);
}

void NodeTest::TearDown() {
  close(linkStdout[0]);
  close(linkStderr[0]);
}

TEST_F(NodeTest, run_external) {
  connectPipes();
  elem->startup();
  std::string msg("External test input");
  std::string empty("");
  write(linkStdin[1], msg.data(), msg.size());
  close(linkStdin[1]);
  elem->waitFor();
  std::string out(drainFd(linkStdout[0]));
  std::string err(drainFd(linkStderr[0]));
  EXPECT_EQ(empty, err);
  EXPECT_EQ(msg, out);
}

