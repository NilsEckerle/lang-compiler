#include "parser/parser.h"
#include "parser/abstract_syntax_tree.h"
#include "tokens.h"
#include <gtest/gtest.h>
#include <string>
#include <vector>

using namespace compiler;

// Example unit test for tokens module
class parser_unit_test : public ::testing::Test {
public:
protected:
  void SetUp() override {
    // Setup code here
  }

  void TearDown() override {
    // Cleanup code here
  }
};

TEST_F(parser_unit_test, parse_tokens_global_expression) {
  std::vector<token_t *> tokens; // int number = 3 + 4 * 5;
  tokens.push_back(create_token_t(tok_int, "int", 1));
  tokens.push_back(create_token_t(tok_id, "main", 1));
  tokens.push_back(create_token_t(tok_lparen, "(", 1));
  tokens.push_back(create_token_t(tok_rparen, ")", 1));
  tokens.push_back(create_token_t(tok_lbrace, "{", 1));
  tokens.push_back(create_token_t(tok_int, "int", 1));
  tokens.push_back(create_token_t(tok_id, "number", 1));
  tokens.push_back(create_token_t(tok_assign, "assign", 1));
  tokens.push_back(create_token_t(tok_number, "3", 1));
  tokens.push_back(create_token_t(tok_plus, "plus", 1));
  tokens.push_back(create_token_t(tok_number, "4", 1));
  tokens.push_back(create_token_t(tok_star, "star", 1));
  tokens.push_back(create_token_t(tok_number, "5", 1));
  tokens.push_back(create_token_t(tok_semicolon, "semicolon", 1));
  tokens.push_back(create_token_t(tok_rbrace, "}", 1));

  std::string expected = "(plus (number 3) (star (number 4) (number 5)))";
  std::vector<parser::ast::abstract_syntax_tree_t *> *trees =
      parser::parse_tokens(tokens);

  ASSERT_EQ(trees->size(), 1);

  // Assert that p_head is actually a variable_t
  parser::ast::node::variable_t *p_var =
      dynamic_cast<parser::ast::node::variable_t *>(trees->back()->p_head);
  ASSERT_NE(nullptr, p_var) << "Expected p_head to be a variable_t";

  ASSERT_EQ(tok_int, p_var->type);
  ASSERT_EQ(0, p_var->pointer_level);
  ASSERT_EQ("number", p_var->identifier);
  ASSERT_EQ(expected, p_var->p_expr->to_prefix_notation());
}

// Test simple variable declaration with literal
TEST_F(parser_unit_test, parse_simple_variable_declaration) {
  std::vector<token_t *> tokens; // int x = 42;
  tokens.push_back(create_token_t(tok_int, "int", 1));
  tokens.push_back(create_token_t(tok_id, "x", 1));
  tokens.push_back(create_token_t(tok_assign, "=", 1));
  tokens.push_back(create_token_t(tok_number, "42", 1));
  tokens.push_back(create_token_t(tok_semicolon, ";", 1));

  std::vector<parser::ast::abstract_syntax_tree_t *> *trees =
      parser::parse_tokens(tokens);

  ASSERT_EQ(trees->size(), 1);
  parser::ast::node::variable_t *p_var =
      dynamic_cast<parser::ast::node::variable_t *>(trees->back()->p_head);
  ASSERT_NE(nullptr, p_var);
  ASSERT_EQ(tok_int, p_var->type);
  ASSERT_EQ("x", p_var->identifier);
  ASSERT_EQ("(number 42)", p_var->p_expr->to_prefix_notation());
}

// Test addition precedence
TEST_F(parser_unit_test, parse_addition_left_to_right) {
  std::vector<token_t *> tokens; // int result = 1 + 2 + 3;
  tokens.push_back(create_token_t(tok_int, "int", 1));
  tokens.push_back(create_token_t(tok_id, "result", 1));
  tokens.push_back(create_token_t(tok_assign, "=", 1));
  tokens.push_back(create_token_t(tok_number, "1", 1));
  tokens.push_back(create_token_t(tok_plus, "+", 1));
  tokens.push_back(create_token_t(tok_number, "2", 1));
  tokens.push_back(create_token_t(tok_plus, "+", 1));
  tokens.push_back(create_token_t(tok_number, "3", 1));
  tokens.push_back(create_token_t(tok_semicolon, ";", 1));

  std::string expected = "(plus (plus (number 1) (number 2)) (number 3))";
  std::vector<parser::ast::abstract_syntax_tree_t *> *trees =
      parser::parse_tokens(tokens);

  ASSERT_EQ(trees->size(), 1);
  parser::ast::node::variable_t *p_var =
      dynamic_cast<parser::ast::node::variable_t *>(trees->back()->p_head);
  ASSERT_NE(nullptr, p_var);
  ASSERT_EQ(expected, p_var->p_expr->to_prefix_notation());
}

// Test multiplication has higher precedence than addition
TEST_F(parser_unit_test, parse_mixed_precedence) {
  std::vector<token_t *> tokens; // int val = 2 * 3 + 4;
  tokens.push_back(create_token_t(tok_int, "int", 1));
  tokens.push_back(create_token_t(tok_id, "val", 1));
  tokens.push_back(create_token_t(tok_assign, "=", 1));
  tokens.push_back(create_token_t(tok_number, "2", 1));
  tokens.push_back(create_token_t(tok_star, "*", 1));
  tokens.push_back(create_token_t(tok_number, "3", 1));
  tokens.push_back(create_token_t(tok_plus, "+", 1));
  tokens.push_back(create_token_t(tok_number, "4", 1));
  tokens.push_back(create_token_t(tok_semicolon, ";", 1));

  std::string expected = "(plus (star (number 2) (number 3)) (number 4))";
  std::vector<parser::ast::abstract_syntax_tree_t *> *trees =
      parser::parse_tokens(tokens);

  ASSERT_EQ(trees->size(), 1);
  parser::ast::node::variable_t *p_var =
      dynamic_cast<parser::ast::node::variable_t *>(trees->back()->p_head);
  ASSERT_NE(nullptr, p_var);
  ASSERT_EQ(expected, p_var->p_expr->to_prefix_notation());
}

// Test parentheses override precedence
TEST_F(parser_unit_test, parse_with_parentheses) {
  std::vector<token_t *> tokens; // int val = (2 + 3) * 4;
  tokens.push_back(create_token_t(tok_int, "int", 1));
  tokens.push_back(create_token_t(tok_id, "val", 1));
  tokens.push_back(create_token_t(tok_assign, "=", 1));
  tokens.push_back(create_token_t(tok_lparen, "(", 1));
  tokens.push_back(create_token_t(tok_number, "2", 1));
  tokens.push_back(create_token_t(tok_plus, "+", 1));
  tokens.push_back(create_token_t(tok_number, "3", 1));
  tokens.push_back(create_token_t(tok_rparen, ")", 1));
  tokens.push_back(create_token_t(tok_star, "*", 1));
  tokens.push_back(create_token_t(tok_number, "4", 1));
  tokens.push_back(create_token_t(tok_semicolon, ";", 1));

  std::string expected = "(star (plus (number 2) (number 3)) (number 4))";
  std::vector<parser::ast::abstract_syntax_tree_t *> *trees =
      parser::parse_tokens(tokens);

  ASSERT_EQ(trees->size(), 1);
  parser::ast::node::variable_t *p_var =
      dynamic_cast<parser::ast::node::variable_t *>(trees->back()->p_head);
  ASSERT_NE(nullptr, p_var);
  ASSERT_EQ(expected, p_var->p_expr->to_prefix_notation());
}

// Test unary negation
TEST_F(parser_unit_test, parse_unary_negation) {
  std::vector<token_t *> tokens; // int x = -5;
  tokens.push_back(create_token_t(tok_int, "int", 1));
  tokens.push_back(create_token_t(tok_id, "x", 1));
  tokens.push_back(create_token_t(tok_assign, "=", 1));
  tokens.push_back(create_token_t(tok_minus, "-", 1));
  tokens.push_back(create_token_t(tok_number, "5", 1));
  tokens.push_back(create_token_t(tok_semicolon, ";", 1));

  std::string expected = "(minus (number 5))";
  std::vector<parser::ast::abstract_syntax_tree_t *> *trees =
      parser::parse_tokens(tokens);

  ASSERT_EQ(trees->size(), 1);
  parser::ast::node::variable_t *p_var =
      dynamic_cast<parser::ast::node::variable_t *>(trees->back()->p_head);
  ASSERT_NE(nullptr, p_var);
  ASSERT_EQ(expected, p_var->p_expr->to_prefix_notation());
}

// Test unary logical NOT
TEST_F(parser_unit_test, parse_unary_not) {
  std::vector<token_t *> tokens; // bool flag = !true;
  tokens.push_back(create_token_t(tok_bool, "bool", 1));
  tokens.push_back(create_token_t(tok_id, "flag", 1));
  tokens.push_back(create_token_t(tok_assign, "=", 1));
  tokens.push_back(create_token_t(tok_exclaimationmark, "!", 1));
  tokens.push_back(create_token_t(tok_number, "0", 1));
  tokens.push_back(create_token_t(tok_semicolon, ";", 1));

  std::string expected = "(exclaimationmark (number 0))";
  std::vector<parser::ast::abstract_syntax_tree_t *> *trees =
      parser::parse_tokens(tokens);

  ASSERT_EQ(trees->size(), 1);
  parser::ast::node::variable_t *p_var =
      dynamic_cast<parser::ast::node::variable_t *>(trees->back()->p_head);
  ASSERT_NE(nullptr, p_var);
  ASSERT_EQ(expected, p_var->p_expr->to_prefix_notation());
}

// Test comparison operators
TEST_F(parser_unit_test, parse_comparison_less_than) {
  std::vector<token_t *> tokens; // bool check = x < 10;
  tokens.push_back(create_token_t(tok_int, "int", 1));
  tokens.push_back(create_token_t(tok_id, "x", 1));
  tokens.push_back(create_token_t(tok_assign, "=", 1));
  tokens.push_back(create_token_t(tok_number, "5", 1));
  tokens.push_back(create_token_t(tok_semicolon, ";", 1));

  tokens.push_back(create_token_t(tok_bool, "bool", 1));
  tokens.push_back(create_token_t(tok_id, "check", 1));
  tokens.push_back(create_token_t(tok_assign, "=", 1));
  tokens.push_back(create_token_t(tok_id, "x", 1));
  tokens.push_back(create_token_t(tok_lt, "<", 1));
  tokens.push_back(create_token_t(tok_number, "10", 1));
  tokens.push_back(create_token_t(tok_semicolon, ";", 1));

  std::string expected = "(lt (id x) (number 10))";
  std::vector<parser::ast::abstract_syntax_tree_t *> *trees =
      parser::parse_tokens(tokens);

  ASSERT_EQ(trees->size(), 2);
  parser::ast::node::variable_t *p_var =
      dynamic_cast<parser::ast::node::variable_t *>(trees->back()->p_head);
  ASSERT_NE(nullptr, p_var);
  ASSERT_EQ(expected, p_var->p_expr->to_prefix_notation());
}

// Test equality operator
TEST_F(parser_unit_test, parse_equality) {
  std::vector<token_t *> tokens; // bool eq = a == b;
  tokens.push_back(create_token_t(tok_int, "int", 1));
  tokens.push_back(create_token_t(tok_id, "a", 1));
  tokens.push_back(create_token_t(tok_assign, "=", 1));
  tokens.push_back(create_token_t(tok_number, "5", 1));
  tokens.push_back(create_token_t(tok_semicolon, ";", 1));
  tokens.push_back(create_token_t(tok_int, "int", 1));
  tokens.push_back(create_token_t(tok_id, "b", 1));
  tokens.push_back(create_token_t(tok_assign, "=", 1));
  tokens.push_back(create_token_t(tok_number, "6", 1));
  tokens.push_back(create_token_t(tok_semicolon, ";", 1));
  tokens.push_back(create_token_t(tok_bool, "bool", 1));
  tokens.push_back(create_token_t(tok_id, "eq", 1));
  tokens.push_back(create_token_t(tok_assign, "=", 1));
  tokens.push_back(create_token_t(tok_id, "a", 1));
  tokens.push_back(create_token_t(tok_eq, "==", 1));
  tokens.push_back(create_token_t(tok_id, "b", 1));
  tokens.push_back(create_token_t(tok_semicolon, ";", 1));

  std::string expected = "(eq (id a) (id b))";
  std::vector<parser::ast::abstract_syntax_tree_t *> *trees =
      parser::parse_tokens(tokens);

  ASSERT_EQ(trees->size(), 3);
  parser::ast::node::variable_t *p_var =
      dynamic_cast<parser::ast::node::variable_t *>(trees->back()->p_head);
  ASSERT_NE(nullptr, p_var);
  ASSERT_EQ(expected, p_var->p_expr->to_prefix_notation());
}

// Test array subscript
// TEST_F(parser_unit_test, parse_array_subscript) {
//   std::vector<token_t *> tokens; // int elem = arr[5];
//   tokens.push_back(create_token_t(tok_int, "int", 1));
//   tokens.push_back(create_token_t(tok_id, "arr", 1));
//   tokens.push_back(create_token_t(tok_assign, "=", 1));
//   tokens.push_back(create_token_t(tok_lbrace, "{", 1));
//   tokens.push_back(create_token_t(tok_number, "1", 1));
//   tokens.push_back(create_token_t(tok_comma, ",", 1));
//   tokens.push_back(create_token_t(tok_number, "2", 1));
//   tokens.push_back(create_token_t(tok_comma, ",", 1));
//   tokens.push_back(create_token_t(tok_number, "3", 1));
//   tokens.push_back(create_token_t(tok_comma, ",", 1));
//   tokens.push_back(create_token_t(tok_number, "4", 1));
//   tokens.push_back(create_token_t(tok_comma, ",", 1));
//   tokens.push_back(create_token_t(tok_number, "5", 1));
//   tokens.push_back(create_token_t(tok_rbrace, "}", 1));
//   tokens.push_back(create_token_t(tok_semicolon, ";", 1));
//
//   tokens.push_back(create_token_t(tok_int, "int", 1));
//   tokens.push_back(create_token_t(tok_id, "elem", 1));
//   tokens.push_back(create_token_t(tok_assign, "=", 1));
//   tokens.push_back(create_token_t(tok_id, "arr", 1));
//   tokens.push_back(create_token_t(tok_lbracket, "[", 1));
//   tokens.push_back(create_token_t(tok_number, "5", 1));
//   tokens.push_back(create_token_t(tok_rbracket, "]", 1));
//   tokens.push_back(create_token_t(tok_semicolon, ";", 1));
//
//   std::string expected = "([] arr 5)";
//   std::vector<parser::ast::abstract_syntax_tree_t *> *trees =
//       parser::parse_tokens(tokens);
//
//   ASSERT_EQ(trees->size(), 1);
//   parser::ast::node::variable_t *p_var =
//       dynamic_cast<parser::ast::node::variable_t *>(trees->back()->p_head);
//   ASSERT_NE(nullptr, p_var);
//   ASSERT_EQ(expected, p_var->p_expr->to_prefix_notation());
// }

// Test function call with no arguments
TEST_F(parser_unit_test, parse_function_call_no_args) {
  std::vector<token_t *> tokens; // int result = foo();
  tokens.push_back(create_token_t(tok_int, "int", 1));
  tokens.push_back(create_token_t(tok_id, "foo", 1));
  tokens.push_back(create_token_t(tok_lparen, "(", 1));
  tokens.push_back(create_token_t(tok_rparen, ")", 1));
  tokens.push_back(create_token_t(tok_lbrace, "{", 1));
  tokens.push_back(create_token_t(tok_return, "return", 1));
  tokens.push_back(create_token_t(tok_number, "5", 1));
  tokens.push_back(create_token_t(tok_semicolon, ";", 1));
  tokens.push_back(create_token_t(tok_rbrace, "}", 1));

  tokens.push_back(create_token_t(tok_int, "int", 1));
  tokens.push_back(create_token_t(tok_id, "result", 1));
  tokens.push_back(create_token_t(tok_assign, "=", 1));
  tokens.push_back(create_token_t(tok_id, "foo", 1));
  tokens.push_back(create_token_t(tok_lparen, "(", 1));
  tokens.push_back(create_token_t(tok_rparen, ")", 1));
  tokens.push_back(create_token_t(tok_semicolon, ";", 1));

  std::string expected = "(call foo)";
  std::vector<parser::ast::abstract_syntax_tree_t *> *trees =
      parser::parse_tokens(tokens);

  ASSERT_EQ(trees->size(), 2);
  parser::ast::node::variable_t *p_var =
      dynamic_cast<parser::ast::node::variable_t *>(trees->back()->p_head);
  ASSERT_NE(nullptr, p_var);
  ASSERT_EQ(expected, p_var->p_expr->to_prefix_notation());
}

// Test function call with multiple arguments
// TEST_F(parser_unit_test, parse_function_call_multiple_args) {
//   std::vector<token_t *> tokens; // int sum = add(1, 2, 3);
//   tokens.push_back(create_token_t(tok_int, "int", 1));
//   tokens.push_back(create_token_t(tok_id, "add", 1));
//   tokens.push_back(create_token_t(tok_lparen, "(", 1));
//   tokens.push_back(create_token_t(tok_int, "int", 1));
//   tokens.push_back(create_token_t(tok_id, "a", 1));
//   tokens.push_back(create_token_t(tok_comma, ",", 1));
//   tokens.push_back(create_token_t(tok_int, "int", 1));
//   tokens.push_back(create_token_t(tok_id, "b", 1));
//   tokens.push_back(create_token_t(tok_comma, ",", 1));
//   tokens.push_back(create_token_t(tok_int, "int", 1));
//   tokens.push_back(create_token_t(tok_id, "c", 1));
//   tokens.push_back(create_token_t(tok_rparen, ")", 1));
//   tokens.push_back(create_token_t(tok_lbrace, "{", 1));
//   tokens.push_back(create_token_t(tok_return, "return", 1));
//   tokens.push_back(create_token_t(tok_int, "2", 1));
//   tokens.push_back(create_token_t(tok_semicolon, ";", 1));
//   tokens.push_back(create_token_t(tok_rbrace, "}", 1));
//
//   tokens.push_back(create_token_t(tok_int, "int", 1));
//   tokens.push_back(create_token_t(tok_id, "sum", 1));
//   tokens.push_back(create_token_t(tok_assign, "=", 1));
//   tokens.push_back(create_token_t(tok_id, "add", 1));
//   tokens.push_back(create_token_t(tok_lparen, "(", 1));
//   tokens.push_back(create_token_t(tok_number, "1", 1));
//   tokens.push_back(create_token_t(tok_comma, ",", 1));
//   tokens.push_back(create_token_t(tok_number, "2", 1));
//   tokens.push_back(create_token_t(tok_comma, ",", 1));
//   tokens.push_back(create_token_t(tok_number, "3", 1));
//   tokens.push_back(create_token_t(tok_rparen, ")", 1));
//   tokens.push_back(create_token_t(tok_semicolon, ";", 1));
//
//   std::string expected = "(add 1 2 3)";
//   std::vector<parser::ast::abstract_syntax_tree_t *> *trees =
//       parser::parse_tokens(tokens);
//
//   ASSERT_EQ(trees->size(), 2);
//   parser::ast::node::variable_t *p_var =
//       dynamic_cast<parser::ast::node::variable_t *>(trees->back()->p_head);
//   ASSERT_NE(nullptr, p_var);
//   ASSERT_EQ(expected, p_var->p_expr->to_prefix_notation());
// }

// Test complex nested expression
TEST_F(parser_unit_test, parse_complex_expression) {
  std::vector<token_t *> tokens; // int val = (a + b) * (c - d);
  tokens.push_back(create_token_t(tok_int, "int", 1));
  tokens.push_back(create_token_t(tok_id, "a", 1));
  tokens.push_back(create_token_t(tok_assign, "=", 1));
  tokens.push_back(create_token_t(tok_number, "2", 1));
  tokens.push_back(create_token_t(tok_semicolon, ";", 1));

  tokens.push_back(create_token_t(tok_int, "int", 1));
  tokens.push_back(create_token_t(tok_id, "b", 1));
  tokens.push_back(create_token_t(tok_assign, "=", 1));
  tokens.push_back(create_token_t(tok_number, "2", 1));
  tokens.push_back(create_token_t(tok_semicolon, ";", 1));

  tokens.push_back(create_token_t(tok_int, "int", 1));
  tokens.push_back(create_token_t(tok_id, "c", 1));
  tokens.push_back(create_token_t(tok_assign, "=", 1));
  tokens.push_back(create_token_t(tok_number, "2", 1));
  tokens.push_back(create_token_t(tok_semicolon, ";", 1));

  tokens.push_back(create_token_t(tok_int, "int", 1));
  tokens.push_back(create_token_t(tok_id, "d", 1));
  tokens.push_back(create_token_t(tok_assign, "=", 1));
  tokens.push_back(create_token_t(tok_number, "2", 1));
  tokens.push_back(create_token_t(tok_semicolon, ";", 1));

  tokens.push_back(create_token_t(tok_int, "int", 1));
  tokens.push_back(create_token_t(tok_id, "val", 1));
  tokens.push_back(create_token_t(tok_assign, "=", 1));
  tokens.push_back(create_token_t(tok_lparen, "(", 1));
  tokens.push_back(create_token_t(tok_id, "a", 1));
  tokens.push_back(create_token_t(tok_plus, "+", 1));
  tokens.push_back(create_token_t(tok_id, "b", 1));
  tokens.push_back(create_token_t(tok_rparen, ")", 1));
  tokens.push_back(create_token_t(tok_star, "*", 1));
  tokens.push_back(create_token_t(tok_lparen, "(", 1));
  tokens.push_back(create_token_t(tok_id, "c", 1));
  tokens.push_back(create_token_t(tok_minus, "-", 1));
  tokens.push_back(create_token_t(tok_id, "d", 1));
  tokens.push_back(create_token_t(tok_rparen, ")", 1));
  tokens.push_back(create_token_t(tok_semicolon, ";", 1));

  std::string expected = "(star (plus (id a) (id b)) (minus (id c) (id d)))";
  std::vector<parser::ast::abstract_syntax_tree_t *> *trees =
      parser::parse_tokens(tokens);

  ASSERT_EQ(trees->size(), 5);
  parser::ast::node::variable_t *p_var =
      dynamic_cast<parser::ast::node::variable_t *>(trees->back()->p_head);
  ASSERT_NE(nullptr, p_var);
  ASSERT_EQ(expected, p_var->p_expr->to_prefix_notation());
}

// Test different type declarations
TEST_F(parser_unit_test, parse_float_declaration) {
  std::vector<token_t *> tokens; // float pi = 3.14;
  tokens.push_back(create_token_t(tok_float, "float", 1));
  tokens.push_back(create_token_t(tok_id, "pi", 1));
  tokens.push_back(create_token_t(tok_assign, "=", 1));
  tokens.push_back(create_token_t(tok_number, "3.14", 1));
  tokens.push_back(create_token_t(tok_semicolon, ";", 1));

  std::vector<parser::ast::abstract_syntax_tree_t *> *trees =
      parser::parse_tokens(tokens);

  ASSERT_EQ(trees->size(), 1);
  parser::ast::node::variable_t *p_var =
      dynamic_cast<parser::ast::node::variable_t *>(trees->back()->p_head);
  ASSERT_NE(nullptr, p_var);
  ASSERT_EQ(tok_float, p_var->type);
  ASSERT_EQ("pi", p_var->identifier);
}

// Test char declaration
TEST_F(parser_unit_test, parse_char_declaration) {
  std::vector<token_t *> tokens; // char letter = 'A';
  tokens.push_back(create_token_t(tok_char, "char", 1));
  tokens.push_back(create_token_t(tok_id, "letter", 1));
  tokens.push_back(create_token_t(tok_assign, "=", 1));
  tokens.push_back(create_token_t(tok_char_literal, "'A'", 1));
  tokens.push_back(create_token_t(tok_semicolon, ";", 1));

  std::vector<parser::ast::abstract_syntax_tree_t *> *trees =
      parser::parse_tokens(tokens);

  ASSERT_EQ(trees->size(), 1);
  parser::ast::node::variable_t *p_var =
      dynamic_cast<parser::ast::node::variable_t *>(trees->back()->p_head);
  ASSERT_NE(nullptr, p_var);
  ASSERT_EQ(tok_char, p_var->type);
  ASSERT_EQ("letter", p_var->identifier);
}
