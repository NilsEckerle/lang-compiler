#include "tokens.h"
#include <gtest/gtest.h>
#include <string>

using namespace compiler;

class tokens_unit_test : public ::testing::Test {
protected:
  void SetUp() override {}
  void TearDown() override {}
};

TEST_F(tokens_unit_test, debug_token_test) {
  token_t tok = token_t(tok_int, "int", 1);
  std::string result = debug_tok(&tok);
  std::string expected = "<int, (int)>";
  EXPECT_EQ(expected, result);

  tok = token_t(tok_number, "123", 1);
  result = debug_tok(&tok);
  expected = "<number, (123)>";
  EXPECT_EQ(expected, result);

  tok = token_t(tok_eof, "", 1);
  result = debug_tok(&tok);
  expected = "<eof, ()>";
  EXPECT_EQ(expected, result);
}
