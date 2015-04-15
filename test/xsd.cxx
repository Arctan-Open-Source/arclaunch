#include "gtest/gtest.h"
#include "Node.hxx"
#include "FileLocations.hxx"

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
#ifdef ARGV_BINARY_LAUNCH
  argv_binary_file = launch(ARGV_BINARY_LAUNCH);
#endif
#ifdef ARGV_PYTHON_LAUNCH
  argv_python_file = launch(ARGV_PYTHON_LAUNCH);
#endif
#ifdef ARGV_R_LAUNCH
  argv_r_file = launch(ARGV_R_LAUNCH);
#endif
#ifdef ARGV_PERL_LAUNCH
  argv_perl_file = launch(ARGV_PERL_LAUNCH);
#endif
}

void NodeTest::TearDown() {
}


#ifdef ARGV_BINARY
TEST_F(NodeTest, load_argv_binary_launch) {
  // Ensure the structure of the launch file is correct
  
}

TEST_F(NodeTest, run_argv_binary) {
  
}
#endif

#ifdef ARGV_PYTHON
TEST_F(NodeTest, load_argv_python_launch) {
  // Ensure the structure of the launch file is correct

}

TEST_F(NodeTest, run_argv_python) {
  // Check that the python script runs correctly

}
#endif

#ifdef ARGV_R
TEST_F(NodeTest, load_argv_r_launch) {
  // Ensure the structure of the launch file is correct
  
}

TEST_F(NodeTest, run_argv_r) {
  // Check that the R script runs correctly

}
#endif

#ifdef ARGV_PERL
TEST_F(NodeTest, load_argv_perl_launch) {
  // Ensure the structure of the launch file is correct
  
}

TEST_F(NodeTest, run_argv_perl) {
  // Check that the perl script runs correctly
  
}
#endif
