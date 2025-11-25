#include "lexer/lexer.h"
#include "tokens.h"
#include <cmath>
#include <gtest/gtest.h>
#include <string>
#include <vector>

// Example unit test for tokens module
class lexer_unit_test : public ::testing::Test {
public:
  lexer::lexer_t *tp_lexer;

protected:
  void SetUp() override {
    // Setup code here
    this->tp_lexer = new lexer::lexer_t();
  }

  void TearDown() override {
    // Cleanup code here
  }
};

// test for the case that a return is tokenized as a id
// (happens when token_e order is wrong)
TEST_F(lexer_unit_test, lex_return_order) {
  std::vector<lexer::token_t *> expected;
  expected.push_back(lexer::create_token_t(lexer::tok_return, ""));
  expected.push_back(lexer::create_token_t(lexer::tok_number, "0"));
  expected.push_back(lexer::create_token_t(lexer::tok_semicolon, ""));
  expected.push_back(lexer::create_token_t(lexer::tok_eof, ""));

  std::vector<lexer::token_t *> tokens = this->tp_lexer->lex("return 0;");

  ASSERT_EQ(expected.size(), tokens.size());
  EXPECT_EQ(expected[0]->e_tok_type, tokens[0]->e_tok_type);
  EXPECT_EQ(expected[0]->t_val, tokens[0]->t_val);
  EXPECT_EQ(expected[1]->e_tok_type, tokens[1]->e_tok_type);
  EXPECT_EQ(expected[1]->t_val, tokens[1]->t_val);
  EXPECT_EQ(expected[2]->e_tok_type, tokens[2]->e_tok_type);
  EXPECT_EQ(expected[2]->t_val, tokens[2]->t_val);
  EXPECT_EQ(expected[3]->e_tok_type, tokens[3]->e_tok_type);
  EXPECT_EQ(expected[3]->t_val, tokens[3]->t_val);
}
