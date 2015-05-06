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
  virtual void SetUp();
  virtual void TearDown();
};

void NodeTest::SetUp() {
  linkage_file = launch(LINKAGE_LAUNCH);
  arclaunch::Node* node = &arclaunch::context().execute(*linkage_file);
  elem = dynamic_cast<arclaunch::LaunchNode*>(node);
  ASSERT_TRUE(NULL != elem);
  subElemA = &elem->getNode("from");
  subElemB = &elem->getNode("to");
  ASSERT_TRUE(NULL != subElemA);
  ASSERT_TRUE(NULL != subElemB);
  pipe2(outLinkB, O_CLOEXEC);
  pipe2(errLinkB, O_CLOEXEC);
  subElemB->linkStdout(outLinkB[1]);
  subElemB->linkStderr(errLinkB[1]);
  close(outLinkB[1]);
  close(errLinkB[1]);
  // Start the processes
  elem->startup();
}

void NodeTest::TearDown() {
  close(outLinkB[0]);
  close(errLinkB[0]);
}

TEST_F(NodeTest, run_linkage) {
  // Let one process complete
  subElemA->waitFor();
  std::cout << "subElemA completes" << std::endl;
  subElemB->waitFor();
  std::cout << "subElemB completes" << std::endl;
  // Let the processes complete
  // elem->waitFor();
  // Ensure that stdin works
  std::string empty("");
  std::string outB(drainFd(outLinkB[0]));
  std::string errB(drainFd(errLinkB[0]));
  EXPECT_EQ("LINKAGE TEST", outB);
  EXPECT_EQ(empty, errB);
}
