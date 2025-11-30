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

  expected.push_back(lexer::create_token_t(lexer::tok_dyn, ""));
  expected.push_back(lexer::create_token_t(lexer::tok_id, "half"));
  expected.push_back(lexer::create_token_t(lexer::tok_lparen, ""));
  expected.push_back(lexer::create_token_t(lexer::tok_dyn, ""));
  expected.push_back(lexer::create_token_t(lexer::tok_id, "number"));
  expected.push_back(lexer::create_token_t(lexer::tok_rparen, ""));
  expected.push_back(lexer::create_token_t(lexer::tok_lbrace, ""));
  expected.push_back(lexer::create_token_t(lexer::tok_let, ""));
  expected.push_back(lexer::create_token_t(lexer::tok_id, "result"));
  expected.push_back(lexer::create_token_t(lexer::tok_assign, ""));
  expected.push_back(lexer::create_token_t(lexer::tok_id, "number"));
  expected.push_back(lexer::create_token_t(lexer::tok_slash, ""));
  expected.push_back(lexer::create_token_t(lexer::tok_number, "2"));
  expected.push_back(lexer::create_token_t(lexer::tok_semicolon, ""));
  expected.push_back(lexer::create_token_t(
      lexer::tok_comment, "// type: can be short, int, long, float, double"));
  expected.push_back(lexer::create_token_t(lexer::tok_return, ""));
  expected.push_back(lexer::create_token_t(lexer::tok_id, "result"));
  expected.push_back(lexer::create_token_t(lexer::tok_semicolon, ""));
  expected.push_back(lexer::create_token_t(lexer::tok_rbrace, ""));
  expected.push_back(lexer::create_token_t(lexer::tok_int, ""));
  expected.push_back(lexer::create_token_t(lexer::tok_id, "main"));
  expected.push_back(lexer::create_token_t(lexer::tok_lparen, ""));
  expected.push_back(lexer::create_token_t(lexer::tok_int, ""));
  expected.push_back(lexer::create_token_t(lexer::tok_id, "argc"));
  expected.push_back(lexer::create_token_t(lexer::tok_comma, ""));
  expected.push_back(lexer::create_token_t(lexer::tok_char, ""));
  expected.push_back(lexer::create_token_t(lexer::tok_star, ""));
  expected.push_back(lexer::create_token_t(lexer::tok_id, "argv"));
  expected.push_back(lexer::create_token_t(lexer::tok_lbracket, ""));
  expected.push_back(lexer::create_token_t(lexer::tok_rbracket, ""));
  expected.push_back(lexer::create_token_t(lexer::tok_rparen, ""));
  expected.push_back(lexer::create_token_t(lexer::tok_lbrace, ""));
  expected.push_back(lexer::create_token_t(lexer::tok_let, ""));
  expected.push_back(lexer::create_token_t(lexer::tok_id, "f"));
  expected.push_back(lexer::create_token_t(lexer::tok_assign, ""));
  expected.push_back(lexer::create_token_t(lexer::tok_number, "1.2"));
  expected.push_back(lexer::create_token_t(lexer::tok_semicolon, ""));
  expected.push_back(lexer::create_token_t(lexer::tok_let, ""));
  expected.push_back(lexer::create_token_t(lexer::tok_id, "i"));
  expected.push_back(lexer::create_token_t(lexer::tok_assign, ""));
  expected.push_back(lexer::create_token_t(lexer::tok_number, "5"));
  expected.push_back(lexer::create_token_t(lexer::tok_semicolon, ""));
  expected.push_back(lexer::create_token_t(lexer::tok_let, ""));
  expected.push_back(lexer::create_token_t(lexer::tok_id, "i2"));
  expected.push_back(lexer::create_token_t(lexer::tok_assign, ""));
  expected.push_back(lexer::create_token_t(lexer::tok_number, "4"));
  expected.push_back(lexer::create_token_t(lexer::tok_semicolon, ""));
  expected.push_back(lexer::create_token_t(lexer::tok_id, "printf"));
  expected.push_back(lexer::create_token_t(lexer::tok_lparen, ""));
  expected.push_back(lexer::create_token_t(lexer::tok_id, "f"));
  expected.push_back(
      lexer::create_token_t(lexer::tok_string, "\"{f}/2={half(f)}\\n\""));
  expected.push_back(lexer::create_token_t(lexer::tok_rparen, ""));
  expected.push_back(lexer::create_token_t(lexer::tok_semicolon, ""));
  expected.push_back(lexer::create_token_t(lexer::tok_id, "printf"));
  expected.push_back(lexer::create_token_t(lexer::tok_lparen, ""));
  expected.push_back(lexer::create_token_t(lexer::tok_id, "f"));
  expected.push_back(
      lexer::create_token_t(lexer::tok_string, "\"{i}/2={half(i)}\\n\""));
  expected.push_back(lexer::create_token_t(lexer::tok_rparen, ""));
  expected.push_back(lexer::create_token_t(lexer::tok_semicolon, ""));
  expected.push_back(lexer::create_token_t(lexer::tok_id, "printf"));
  expected.push_back(lexer::create_token_t(lexer::tok_lparen, ""));
  expected.push_back(lexer::create_token_t(lexer::tok_id, "f"));
  expected.push_back(
      lexer::create_token_t(lexer::tok_string, "\"{i2}/2={half(i2)}\\n\""));
  expected.push_back(lexer::create_token_t(lexer::tok_rparen, ""));
  expected.push_back(lexer::create_token_t(lexer::tok_semicolon, ""));
  expected.push_back(lexer::create_token_t(lexer::tok_return, ""));
  expected.push_back(lexer::create_token_t(lexer::tok_number, "0"));
  expected.push_back(lexer::create_token_t(lexer::tok_semicolon, ""));
  expected.push_back(lexer::create_token_t(lexer::tok_rbrace, ""));
  expected.push_back(
      lexer::create_token_t(lexer::tok_comment, "// comment at end of file"));
  expected.push_back(lexer::create_token_t(lexer::tok_eof, ""));

  // execute test
  std::vector<lexer::token_t *> tokens = this->tp_lexer->lex(source);

  // test
  ASSERT_EQ(expected.size(), tokens.size());

  while (expected.size() > 0) {
    lexer::token_t *exp = expected[expected.size() - 1];
    expected.pop_back();
    lexer::token_t *tok = tokens[tokens.size() - 1];
    tokens.pop_back();

    std::cout << exp->type_name() << " " << tok->type_name() << " "
              << exp->t_val << " " << tok->t_val << std::endl;

    EXPECT_EQ(tok->e_tok_type, exp->e_tok_type);
    EXPECT_EQ(tok->t_val, exp->t_val);
  }
}
