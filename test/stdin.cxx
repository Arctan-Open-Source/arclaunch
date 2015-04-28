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
}

TEST_F(NodeTest, run_stdin) {
  int p[2];
  pipe(p);
  // setup close on exec for the write end
  fcntl(p[1], F_SETFD, FD_CLOEXEC);
  // The data being sent in
  std::string xch("STDIN TEST");
  // Set the read end as stdin
  subElem->linkStdin(p[0]);
  // Start the process
  elem->startup();
  close(p[0]);
  // Write to stdin
  ::write(p[1], xch.c_str(), xch.size());
  // close the remaining end of this end of the pipe
  close(p[1]);
  // Let the process complete
  subElem->waitFor();
  // Ensure that stdin works
  std::string empty("");
  std::string err(drainFd(subElem->getStderr()));
  std::string out(drainFd(subElem->getStdout()));
  EXPECT_EQ(empty, err);
  EXPECT_EQ(xch, out);
}
