#include "lexer/lexer.h"
#include "tokens.h"
#include <cmath>
#include <gtest/gtest.h>
#include <iostream>
#include <ostream>
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
    free(this->tp_lexer);
  }
};

// test for the case that a return is tokenized as a id
// (happens when token_e order is wrong)
TEST_F(lexer_unit_test, lex_return_order) {
  std::vector<lexer::token_t *> expected;
  expected.push_back(lexer::create_token_t(lexer::tok_return, "", 1));
  expected.push_back(lexer::create_token_t(lexer::tok_number, "0", 1));
  expected.push_back(lexer::create_token_t(lexer::tok_semicolon, "", 1));
  expected.push_back(lexer::create_token_t(lexer::tok_eof, "", 1));

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

TEST_F(lexer_unit_test, lex) {
  // test source
  std::string source = "dyn half(dyn number) {\n"
                       "  let result = number / 2; // type: can be short, int, "
                       "long, float, double\n"
                       "\n"
                       "  return result;\n"
                       "}\n"
                       "\n"
                       "int main (int argc, char *argv[]) {\n"
                       "  let f = 1.2;\n"
                       "  let i = 5;\n"
                       "  let i2 = 4;\n"
                       "\n"
                       "  printf(f\"{f}/2={half(f)}\\n\");\n"
                       "  printf(f\"{i}/2={half(i)}\\n\");\n"
                       "  printf(f\"{i2}/2={half(i2)}\\n\");\n"
                       "\n"
                       "  return 0;\n"
                       "} // comment at end of file";

  // expected
  std::vector<lexer::token_t *> expected;

  expected.push_back(lexer::create_token_t(lexer::tok_dyn, "", 1));
  expected.push_back(lexer::create_token_t(lexer::tok_id, "half", 1));
  expected.push_back(lexer::create_token_t(lexer::tok_lparen, "", 1));
  expected.push_back(lexer::create_token_t(lexer::tok_dyn, "", 1));
  expected.push_back(lexer::create_token_t(lexer::tok_id, "number", 1));
  expected.push_back(lexer::create_token_t(lexer::tok_rparen, "", 1));
  expected.push_back(lexer::create_token_t(lexer::tok_lbrace, "", 1));
  expected.push_back(lexer::create_token_t(lexer::tok_let, "", 1));
  expected.push_back(lexer::create_token_t(lexer::tok_id, "result", 1));
  expected.push_back(lexer::create_token_t(lexer::tok_assign, "", 1));
  expected.push_back(lexer::create_token_t(lexer::tok_id, "number", 1));
  expected.push_back(lexer::create_token_t(lexer::tok_slash, "", 1));
  expected.push_back(lexer::create_token_t(lexer::tok_number, "2", 1));
  expected.push_back(lexer::create_token_t(lexer::tok_semicolon, "", 1));
  expected.push_back(lexer::create_token_t(
      lexer::tok_comment, "// type: can be short, int, long, float, double",
      1));
  expected.push_back(lexer::create_token_t(lexer::tok_return, "", 1));
  expected.push_back(lexer::create_token_t(lexer::tok_id, "result", 1));
  expected.push_back(lexer::create_token_t(lexer::tok_semicolon, "", 1));
  expected.push_back(lexer::create_token_t(lexer::tok_rbrace, "", 1));
  expected.push_back(lexer::create_token_t(lexer::tok_int, "", 1));
  expected.push_back(lexer::create_token_t(lexer::tok_id, "main", 1));
  expected.push_back(lexer::create_token_t(lexer::tok_lparen, "", 1));
  expected.push_back(lexer::create_token_t(lexer::tok_int, "", 1));
  expected.push_back(lexer::create_token_t(lexer::tok_id, "argc", 1));
  expected.push_back(lexer::create_token_t(lexer::tok_comma, "", 1));
  expected.push_back(lexer::create_token_t(lexer::tok_char, "", 1));
  expected.push_back(lexer::create_token_t(lexer::tok_star, "", 1));
  expected.push_back(lexer::create_token_t(lexer::tok_id, "argv", 1));
  expected.push_back(lexer::create_token_t(lexer::tok_lbracket, "", 1));
  expected.push_back(lexer::create_token_t(lexer::tok_rbracket, "", 1));
  expected.push_back(lexer::create_token_t(lexer::tok_rparen, "", 1));
  expected.push_back(lexer::create_token_t(lexer::tok_lbrace, "", 1));
  expected.push_back(lexer::create_token_t(lexer::tok_let, "", 1));
  expected.push_back(lexer::create_token_t(lexer::tok_id, "f", 1));
  expected.push_back(lexer::create_token_t(lexer::tok_assign, "", 1));
  expected.push_back(lexer::create_token_t(lexer::tok_number, "1.2", 1));
  expected.push_back(lexer::create_token_t(lexer::tok_semicolon, "", 1));
  expected.push_back(lexer::create_token_t(lexer::tok_let, "", 1));
  expected.push_back(lexer::create_token_t(lexer::tok_id, "i", 1));
  expected.push_back(lexer::create_token_t(lexer::tok_assign, "", 1));
  expected.push_back(lexer::create_token_t(lexer::tok_number, "5", 1));
  expected.push_back(lexer::create_token_t(lexer::tok_semicolon, "", 1));
  expected.push_back(lexer::create_token_t(lexer::tok_let, "", 1));
  expected.push_back(lexer::create_token_t(lexer::tok_id, "i2", 1));
  expected.push_back(lexer::create_token_t(lexer::tok_assign, "", 1));
  expected.push_back(lexer::create_token_t(lexer::tok_number, "4", 1));
  expected.push_back(lexer::create_token_t(lexer::tok_semicolon, "", 1));
  expected.push_back(lexer::create_token_t(lexer::tok_id, "printf", 1));
  expected.push_back(lexer::create_token_t(lexer::tok_lparen, "", 1));
  expected.push_back(lexer::create_token_t(lexer::tok_id, "f", 1));
  expected.push_back(
      lexer::create_token_t(lexer::tok_string, "\"{f}/2={half(f)}\\n\"", 1));
  expected.push_back(lexer::create_token_t(lexer::tok_rparen, "", 1));
  expected.push_back(lexer::create_token_t(lexer::tok_semicolon, "", 1));
  expected.push_back(lexer::create_token_t(lexer::tok_id, "printf", 1));
  expected.push_back(lexer::create_token_t(lexer::tok_lparen, "", 1));
  expected.push_back(lexer::create_token_t(lexer::tok_id, "f", 1));
  expected.push_back(
      lexer::create_token_t(lexer::tok_string, "\"{i}/2={half(i)}\\n\"", 1));
  expected.push_back(lexer::create_token_t(lexer::tok_rparen, "", 1));
  expected.push_back(lexer::create_token_t(lexer::tok_semicolon, "", 1));
  expected.push_back(lexer::create_token_t(lexer::tok_id, "printf", 1));
  expected.push_back(lexer::create_token_t(lexer::tok_lparen, "", 1));
  expected.push_back(lexer::create_token_t(lexer::tok_id, "f", 1));
  expected.push_back(
      lexer::create_token_t(lexer::tok_string, "\"{i2}/2={half(i2)}\\n\"", 1));
  expected.push_back(lexer::create_token_t(lexer::tok_rparen, "", 1));
  expected.push_back(lexer::create_token_t(lexer::tok_semicolon, "", 1));
  expected.push_back(lexer::create_token_t(lexer::tok_return, "", 1));
  expected.push_back(lexer::create_token_t(lexer::tok_number, "0", 1));
  expected.push_back(lexer::create_token_t(lexer::tok_semicolon, "", 1));
  expected.push_back(lexer::create_token_t(lexer::tok_rbrace, "", 1));
  expected.push_back(lexer::create_token_t(lexer::tok_comment,
                                           "// comment at end of file", 1));
  expected.push_back(lexer::create_token_t(lexer::tok_eof, "", 1));

  // execute test
  std::vector<lexer::token_t *> tokens = this->tp_lexer->lex(source);

  // test
  ASSERT_EQ(expected.size(), tokens.size());

  while (expected.size() > 0) {
    lexer::token_t *exp = expected[expected.size() - 1];
    expected.pop_back();
    lexer::token_t *tok = tokens[tokens.size() - 1];
    tokens.pop_back();

    std::cout << exp->type_name() << " " << tok->type_name() << std::endl;

    EXPECT_EQ(tok->e_tok_type, exp->e_tok_type);
    EXPECT_EQ(tok->t_val, exp->t_val);
  }
}
