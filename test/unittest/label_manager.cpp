#include "code_generation/label_manager.h"
#include <gtest/gtest.h>
#include <string>
#include <vector>

using namespace compiler;

// Example unit test for tokens module
class label_manager_unit_test : public ::testing::Test {
public:
protected:
  void SetUp() override {
    // Setup code here
  }

  void TearDown() override {
    // Cleanup code here
  }
};

TEST_F(label_manager_unit_test, newlabel) {
  ASSERT_EQ("t1", label_manager_t::newlabel());
  ASSERT_EQ("t2", label_manager_t::newlabel());
  ASSERT_EQ("t3", label_manager_t::newlabel());
  ASSERT_EQ("t4", label_manager_t::newlabel());
  ASSERT_EQ("t5", label_manager_t::newlabel());
  ASSERT_EQ("t6", label_manager_t::newlabel());
}
