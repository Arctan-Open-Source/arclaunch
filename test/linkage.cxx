#include "gtest/gtest.h"
#include "arclaunch.hxx"
#include "locations.hxx"
#include "interpret_error.h"
#include "drain_fd.h"

// This is a test class for Node.hxx
// ensures that executables do in-fact get started up

class NodeTest : public testing::Test {
protected:
  std::unique_ptr<launch_t> linkage_file;
  arclaunch::LaunchNode* elem;
  arclaunch::Node *subElemA, *subElemB;
  int outLinkB[2];
  int errLinkB[2];
  void connectPipes();
  virtual void SetUp();
  virtual void TearDown();
};

void NodeTest::SetUp() {
  linkage_file = launch(LINKAGE_LAUNCH);
  arclaunch::Node* node = &arclaunch::context().execute(*linkage_file);
  elem = dynamic_cast<arclaunch::LaunchNode*>(node);
  subElemA = &elem->getNode("from");
  subElemB = &elem->getNode("to");
  // Start the processes
}

void NodeTest::TearDown() {
  close(outLinkB[0]);
  close(errLinkB[0]);
}

void NodeTest::connectPipes() {
  pipe2(outLinkB, O_CLOEXEC);
  pipe2(errLinkB, O_CLOEXEC);
  subElemB->linkStdout(outLinkB[1]);
  subElemB->linkStderr(errLinkB[1]);
  close(outLinkB[1]);
  close(errLinkB[1]);
}

TEST_F(NodeTest, run_linkage) {
  std::string empty("");
  // 
  connectPipes();
  elem->startup();
  elem->waitFor();
  // Ensure that stdin works
  std::string outB(drainFd(outLinkB[0]));
  std::string errB(drainFd(errLinkB[0]));
  EXPECT_EQ("LINKAGE TEST", outB);
  EXPECT_EQ(empty, errB);
  // repeat
  connectPipes();
  elem->startup();
  elem->waitFor();
  outB = drainFd(outLinkB[0]);
  errB = drainFd(errLinkB[0]);
  EXPECT_EQ("LINKAGE TEST", outB);
  EXPECT_EQ(empty, errB);
}
