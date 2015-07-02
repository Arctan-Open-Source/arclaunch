#include "gtest/gtest.h"
#include "arclaunch.hxx"
#include <iostream>

#define EMPTY_LAUNCH "xml/empty_launch.xml"

class NodeTest : public testing::Test {
protected:
  std::unique_ptr<launch_t> empty_file;
  arclaunch::LaunchNode* elem;
};

TEST_F(NodeTest, run_empty) {
  empty_file = launch(EMPTY_LAUNCH);
  std::cout << "BEFORE DYNAMIC CAST" << std::endl;
  ASSERT_NO_THROW(elem = dynamic_cast<arclaunch::LaunchNode*>(
    &arclaunch::context().execute(*empty_file)));
  std::cout << "BEFORE STARTUP" << std::endl;
  ASSERT_NO_THROW(elem->startup());
  std::cout << "BEFORE WAITFOR" << std::endl;
  ASSERT_NO_THROW(elem->waitFor());
}

