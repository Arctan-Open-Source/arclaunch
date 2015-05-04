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
  int outFd;
  int errFd;
  virtual void SetUp();
  virtual void TearDown();
};

void NodeTest::SetUp() {
  env_file = launch(ENV_LAUNCH);
  elem = dynamic_cast<arclaunch::LaunchNode*>(&arclaunch::context().execute(*env_file));
  ASSERT_TRUE(NULL != elem);
  subElem = &elem->getNode("env");
  // needs to dup because the descriptors are closed on startup
  outFd = dup(subElem->getStdout());
  errFd = dup(subElem->getStderr());
  fcntl(outFd, F_SETFD, FD_CLOEXEC);
  fcntl(errFd, F_SETFD, FD_CLOEXEC);
  elem->startup();
}

void NodeTest::TearDown() {
}

TEST_F(NodeTest, run_env) {
  // The major issue right now is my inability to access subprocesses
  // The solution is to add subprocess access to the LaunchNode class
  // Access a subprocess
  // Let the process complete
  subElem->waitFor();
  // Ensure that nothing is read from stderr
  std::string empty("");
  std::string err(drainFd(errFd));
  EXPECT_EQ(empty, err);
  // Ensure that something is read from stdout
  std::string out(drainFd(outFd));
  EXPECT_EQ("test environment variable", out);
  close(outFd);
  close(errFd);
}
