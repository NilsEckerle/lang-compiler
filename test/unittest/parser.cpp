#include "parser/parser.h"
#include "parser/syntax_tree.h"
#include "tokens.h"
#include <gtest/gtest.h>
#include <iostream>
#include <ostream>
#include <string>
#include <vector>

// Example unit test for tokens module
class parser_unit_test : public ::testing::Test {
public:
  parser::parser_t *p_parser;

protected:
  void SetUp() override {
    // Setup code here
    this->p_parser = new parser::parser_t();
  }

  void TearDown() override {
    // Cleanup code here
    free(this->p_parser);
  }
};

TEST_F(parser_unit_test, parse_expression) {
  std::vector<lexer::token_t *> tokens;
  tokens.push_back(lexer::create_token_t(lexer::tok_number, "3", 1));
  tokens.push_back(lexer::create_token_t(lexer::tok_plus, "", 1));
  tokens.push_back(lexer::create_token_t(lexer::tok_number, "4", 1));
  tokens.push_back(lexer::create_token_t(lexer::tok_star, "", 1));
  tokens.push_back(lexer::create_token_t(lexer::tok_number, "5", 1));

  std::string expected = "(+ 3 (* 4 5))";

  parser::ast::syntax_tree_t *tree = parser::parse_tokens(tokens);

  ASSERT_EQ(expected, tree->to_prefix_notation());
}

TEST_F(parser_unit_test, parse_tree_1) {
  std::vector<lexer::token_t *> tokens;
  tokens.push_back(lexer::create_token_t(lexer::tok_id, "num", 1));
  tokens.push_back(lexer::create_token_t(lexer::tok_assign, "", 1));
  tokens.push_back(lexer::create_token_t(lexer::tok_number, "3", 1));
  tokens.push_back(lexer::create_token_t(lexer::tok_plus, "", 1));
  tokens.push_back(lexer::create_token_t(lexer::tok_number, "4", 1));
  tokens.push_back(lexer::create_token_t(lexer::tok_star, "", 1));
  tokens.push_back(lexer::create_token_t(lexer::tok_number, "5", 1));
  tokens.push_back(lexer::create_token_t(lexer::tok_semicolon, "", 1));

  std::string expected = "(= num (+ 3 (* 4 5)))";

  parser::ast::syntax_tree_t *tree = parser::parse_tokens(tokens);

  ASSERT_EQ(expected, tree->to_prefix_notation());
}
