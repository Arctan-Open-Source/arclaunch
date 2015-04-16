#include "gtest/gtest.h"
#include "Node.hxx"
#include "locations.hxx"

// This is a test class for Node.hxx
// ensures that executables do in-fact get started up

class NodeTest : public testing::Test {
protected:
  std::unique_ptr<launch_t> 
    argv_binary_file, 
    argv_python_file, 
    argv_r_file,
    argv_perl_file;
  virtual void SetUp();
  virtual void TearDown();
};

void NodeTest::SetUp() {
  argv_perl_file = launch(ARGV_PERL_LAUNCH);
}

void NodeTest::TearDown() {
}

TEST_F(NodeTest, load_argv_perl_launch) {
  // Ensure the structure of the launch file is correct
  
}

TEST_F(NodeTest, run_argv_perl) {
  // Check that the perl script runs correctly
  
}
