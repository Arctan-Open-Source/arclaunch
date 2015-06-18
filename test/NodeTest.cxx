#include "Node.hxx"
#include "gtest/gtest.h"

class TestNode : public arclaunch::Node {
};

class NodeTest : public testing::Test {
  protected:
    virtual void SetUp();
    virtual void TearDown();
};

