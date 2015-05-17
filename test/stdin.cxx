#include "gtest/gtest.h"
#include "arclaunch.hxx"
#include "locations.hxx"
#include "interpret_error.h"
#include "drain_fd.h"

// This is a test class for Node.hxx
// ensures that executables do in-fact get started up

class NodeTest : public testing::Test {
protected:
  std::unique_ptr<launch_t> stdin_file;
  arclaunch::LaunchNode* elem;
  arclaunch::Node* subElem;
  int inLink[2];
  int outLink[2];
  int errLink[2];
  void connectPipes();
  virtual void SetUp();
  virtual void TearDown();
};

void NodeTest::SetUp() {
  stdin_file = launch(STDIN_LAUNCH);
  elem = dynamic_cast<arclaunch::LaunchNode*>(&arclaunch::context().execute(*stdin_file));
  ASSERT_TRUE(NULL != elem);
  subElem = &elem->getNode("stdin");
}

void NodeTest::TearDown() {
  close(outLink[0]);
  close(errLink[0]);
}

void NodeTest::connectPipes() {
  pipe2(inLink, O_CLOEXEC);
  pipe2(outLink, O_CLOEXEC);
  pipe2(errLink, O_CLOEXEC);
  int flags = fcntl(inLink[0], F_GETFL) & 3;
  flags = fcntl(inLink[0], F_GETFL) & 3;
  subElem->linkStdin(inLink[0]);
  subElem->linkStdout(outLink[1]);
  subElem->linkStderr(errLink[1]);
  close(inLink[0]);
  close(outLink[1]);
  close(errLink[1]);
}

TEST_F(NodeTest, run_stdin) {
  // The data being sent in
  std::string xch("STDIN TEST");
  // Start the process
  connectPipes();
  elem->startup();
  // Write to stdin
  ::write(inLink[1], xch.c_str(), xch.size());
  // Needs to close so the process can finish
  close(inLink[1]);
  // Let the process complete
  subElem->waitFor();
  // Ensure that stdin works
  std::string empty("");
  std::string err(drainFd(errLink[0]));
  std::string out(drainFd(outLink[0]));
  EXPECT_EQ(empty, err);
  EXPECT_EQ(xch, out);
}

TEST_F(NodeTest, repeat_stdin) {
  // The data being sent in
  std::string xch("STDIN TEST");
  // Start the process
  connectPipes();
  elem->startup();
  // Write to stdin
  ::write(inLink[1], xch.c_str(), xch.size());
  // Needs to close so the process can finish
  close(inLink[1]);
  // Let the process complete
  subElem->waitFor();
  // Ensure that stdin works
  std::string empty("");
  std::string err(drainFd(errLink[0]));
  std::string out(drainFd(outLink[0]));
  EXPECT_EQ(empty, err);
  EXPECT_EQ(xch, out);
  // Repeat the process
  connectPipes();
  elem->startup();
  ::write(inLink[1], xch.c_str(), xch.size());
  close(inLink[1]);
  // Let the process complete
  subElem->waitFor();
  // Ensure that stdin works
  err = drainFd(errLink[0]);
  out = drainFd(outLink[0]);
  EXPECT_EQ(empty, err);
  EXPECT_EQ(xch, out);
}
