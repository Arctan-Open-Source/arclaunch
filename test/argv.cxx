#include "gtest/gtest.h"
#include "arclaunch.hxx"
#include "locations.hxx"
#include "interpret_error.h"

// This is a test class for Node.hxx
// ensures that executables do in-fact get started up

class NodeTest : public testing::Test {
protected:
  std::unique_ptr<launch_t> argv_file;
  arclaunch::LaunchNode* elem;
  arclaunch::Node* subElem;
  virtual void SetUp();
  virtual void TearDown();
};

void NodeTest::SetUp() {
  argv_file = launch(ARGV_LAUNCH);
  elem = dynamic_cast<arclaunch::LaunchNode*>(&arclaunch::context().execute(*argv_file));
  ASSERT_TRUE(NULL != elem);
  elem->startup();
  subElem = &elem->getNode("argv");
}

void NodeTest::TearDown() {
}

TEST_F(NodeTest, run_argv) {
  // The major issue right now is my inability to access subprocesses
  // The solution is to add subprocess access to the LaunchNode class
  // Access a subprocess
  // Let the process complete
  subElem->waitFor();
  // should not read any bytes but shouldn't give an error
  char data[50];
  ssize_t numread;
  ssize_t cumread;
  int limit;

  // Ensure that nothing is read from stderr
  cumread = 0;
  limit = 5;
  do {
    numread = read(subElem->getStderr(), data, 49);
    data[numread] = '\0';
    std::cout << data;
    limit--;
    cumread += numread;
  } while(numread > 0 && limit > 0);
  EXPECT_EQ(0, cumread);
  
  // Ensure that something is read from stdout
  cumread = 0;
  limit = 5;
  do {
    numread = read(subElem->getStdout(), data, 49);
    data[numread] = '\0';
    std::cout << data;
    limit--;
    cumread += numread;
  } while(numread > 0 && limit > 0);
  EXPECT_NE(0, cumread);
}
