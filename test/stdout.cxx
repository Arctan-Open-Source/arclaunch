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
  virtual void SetUp();
  virtual void TearDown();
};

void NodeTest::SetUp() {
  stdout_file = launch(STDOUT_LAUNCH);
  elem = dynamic_cast<arclaunch::LaunchNode*>(&arclaunch::context().execute(*stdout_file));
  ASSERT_TRUE(NULL != elem);
  subElem = &elem->getNode("stdout");
  pipe2(outLink, O_CLOEXEC);
  pipe2(errLink, O_CLOEXEC);
  subElem->linkStdout(outLink[1]);
  subElem->linkStderr(errLink[1]);
  close(outLink[1]);
  close(errLink[1]);
  elem->startup();
}

void NodeTest::TearDown() {
  close(outLink[0]);
  close(errLink[0]);
}

TEST_F(NodeTest, run_stdout) {
  // Let the process complete
  subElem->waitFor();
  // Ensure that stdout works
  std::string empty("");
  std::string out(drainFd(outLink[0]));
  std::string err(drainFd(errLink[0]));
  EXPECT_EQ(empty, err);
  EXPECT_EQ("STDOUT TEST", out);
}
