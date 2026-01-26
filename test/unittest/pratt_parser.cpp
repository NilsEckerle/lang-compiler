#include "parser/pratt_parser.h"
#include "exceptions.h"
#include "parser/abstract_syntax_tree.h"
#include "tokens.h"
#include <gtest/gtest.h>
#include <string>
#include <vector>

using namespace compiler;

// Example unit test for tokens module
class pratt_parser_unit_test : public ::testing::Test {
public:
protected:
  void SetUp() override {
    // Setup code here
  }

  void TearDown() override {
    // Cleanup code here
  }
};

TEST_F(pratt_parser_unit_test, parse_expression_eof) {
  std::vector<token_t *> tokens;
  tokens.push_back(create_token_t(tok_number, "3", 1));
  tokens.push_back(create_token_t(tok_plus, "", 1));
  tokens.push_back(create_token_t(tok_number, "4", 1));
  tokens.push_back(create_token_t(tok_star, "", 1));
  tokens.push_back(create_token_t(tok_number, "5", 1));
  tokens.push_back(create_token_t(tok_eof, "", 1));
  symbol_table::symbol_table_t st;

  std::string expected = "(plus (number 3) (star (number 4) (number 5)))";

  // CRITICAL: Reverse the tokens since parse_expression expects reversed order
  std::reverse(tokens.begin(), tokens.end());
  parser::ast::node::expression_t *ex =
      parser::pratt_parser::parse_expression(tokens, &st);

  ASSERT_EQ(expected, ex->to_prefix_notation());
}

TEST_F(pratt_parser_unit_test, parse_expression_semicolon) {
  std::vector<token_t *> tokens;
  tokens.push_back(create_token_t(tok_number, "1.2", 1));
  tokens.push_back(create_token_t(tok_plus, "", 1));
  tokens.push_back(create_token_t(tok_number, "4", 1));
  tokens.push_back(create_token_t(tok_star, "", 1));
  tokens.push_back(create_token_t(tok_minus, "", 1));
  tokens.push_back(create_token_t(tok_number, "5", 1));
  tokens.push_back(create_token_t(tok_semicolon, "", 1));
  symbol_table::symbol_table_t st;

  std::string expected =
      "(plus (number 1.2) (star (number 4) (minus (number 5))))";

  // CRITICAL: Reverse the tokens since parse_expression expects reversed order
  std::reverse(tokens.begin(), tokens.end());
  parser::ast::node::expression_t *ex =
      parser::pratt_parser::parse_expression(tokens, &st);

  ASSERT_EQ(expected, ex->to_prefix_notation());
}

TEST_F(pratt_parser_unit_test, parse_expression_parentesies) {
  std::vector<token_t *> tokens;
  tokens.push_back(create_token_t(tok_lparen, "(", 1));
  tokens.push_back(create_token_t(tok_number, "1.2", 1));
  tokens.push_back(create_token_t(tok_plus, "", 1));
  tokens.push_back(create_token_t(tok_number, "4", 1));
  tokens.push_back(create_token_t(tok_rparen, ")", 1));
  tokens.push_back(create_token_t(tok_star, "", 1));
  tokens.push_back(create_token_t(tok_minus, "", 1));
  tokens.push_back(create_token_t(tok_number, "5", 1));
  tokens.push_back(create_token_t(tok_semicolon, "", 1));
  symbol_table::symbol_table_t st;

  std::string expected =
      "(star (plus (number 1.2) (number 4)) (minus (number 5)))";

  // CRITICAL: Reverse the tokens since parse_expression expects reversed order
  std::reverse(tokens.begin(), tokens.end());
  parser::ast::node::expression_t *ex =
      parser::pratt_parser::parse_expression(tokens, &st);

  ASSERT_EQ(expected, ex->to_prefix_notation());
}

TEST_F(pratt_parser_unit_test, parse_expression_variable) {
  std::vector<token_t *> tokens;
  tokens.push_back(create_token_t(tok_id, "a", 1));
  tokens.push_back(create_token_t(tok_plus, "+", 1));
  tokens.push_back(create_token_t(tok_number, "5", 1));
  tokens.push_back(create_token_t(tok_semicolon, ";", 1));

  symbol_table::symbol_table_t st;
  parser::ast::node::literal_expr_t *expr =
      new parser::ast::node::literal_expr_t(tok_number, "5");
  parser::ast::node::variable_t var =
      parser::ast::node::variable_t(tok_int, 0, "a", expr);
  st.add(&var);

  std::string expected = "(plus (id a) (number 5))";

  // CRITICAL: Reverse the tokens since parse_expression expects reversed order
  std::reverse(tokens.begin(), tokens.end());
  parser::ast::node::expression_t *ex =
      parser::pratt_parser::parse_expression(tokens, &st);

  ASSERT_EQ(expected, ex->to_prefix_notation());
}

TEST_F(pratt_parser_unit_test, parse_expression_variable_not_declared) {
  std::vector<token_t *> tokens;
  tokens.push_back(create_token_t(tok_id, "a", 1));
  tokens.push_back(create_token_t(tok_plus, "+", 1));
  tokens.push_back(create_token_t(tok_number, "5", 1));
  tokens.push_back(create_token_t(tok_semicolon, ";", 1));

  symbol_table::symbol_table_t st;

  std::string expected = "(plus (id a) (number 5))";

  // CRITICAL: Reverse the tokens since parse_expression expects reversed order
  std::reverse(tokens.begin(), tokens.end());
  ASSERT_THROW(parser::pratt_parser::parse_expression(tokens, &st),
               exceptions::variable_not_declared_error);
}

TEST_F(pratt_parser_unit_test, parse_expression_function_call_no_args) {
  std::vector<token_t *> tokens;
  tokens.push_back(create_token_t(tok_id, "foo", 1));
  tokens.push_back(create_token_t(tok_lparen, "(", 1));
  tokens.push_back(create_token_t(tok_rparen, ")", 1));
  tokens.push_back(create_token_t(tok_semicolon, ";", 1));

  symbol_table::symbol_table_t st;
  // Declare function 'foo' with no parameters
  parser::ast::node::function_t func =
      parser::ast::node::function_t(tok_void, 0, "foo", {}, nullptr);
  st.add(&func);

  std::string expected = "(call foo)";

  std::reverse(tokens.begin(), tokens.end());
  parser::ast::node::expression_t *ex =
      parser::pratt_parser::parse_expression(tokens, &st);

  ASSERT_EQ(expected, ex->to_prefix_notation());
}

// TEST_F(pratt_parser_unit_test, parse_expression_function_call_one_arg) {
//   std::vector<token_t *> tokens;
//   tokens.push_back(create_token_t(tok_id, "foo", 1));
//   tokens.push_back(create_token_t(tok_lparen, "(", 1));
//   tokens.push_back(create_token_t(tok_number, "5", 1));
//   tokens.push_back(create_token_t(tok_rparen, ")", 1));
//   tokens.push_back(create_token_t(tok_semicolon, ";", 1));
//
//   symbol_table::symbol_table_t st;
//   parser::ast::node::function_t func =
//       parser::ast::node::function_t(tok_int, 0, "foo", {}, nullptr);
//   st.add(&func);
//
//   std::string expected = "(call foo (number 5))";
//
//   std::reverse(tokens.begin(), tokens.end());
//   parser::ast::node::expression_t *ex =
//       parser::pratt_parser::parse_expression(tokens, &st);
//
//   ASSERT_EQ(expected, ex->to_prefix_notation());
// }
//
// TEST_F(pratt_parser_unit_test, parse_expression_function_call_multiple_args)
// {
//   std::vector<token_t *> tokens;
//   tokens.push_back(create_token_t(tok_id, "add", 1));
//   tokens.push_back(create_token_t(tok_lparen, "(", 1));
//   tokens.push_back(create_token_t(tok_number, "3", 1));
//   tokens.push_back(create_token_t(tok_comma, ",", 1));
//   tokens.push_back(create_token_t(tok_number, "7", 1));
//   tokens.push_back(create_token_t(tok_comma, ",", 1));
//   tokens.push_back(create_token_t(tok_number, "10", 1));
//   tokens.push_back(create_token_t(tok_rparen, ")", 1));
//   tokens.push_back(create_token_t(tok_semicolon, ";", 1));
//
//   symbol_table::symbol_table_t st;
//   parser::ast::node::function_t func =
//       parser::ast::node::function_t(tok_int, 0, "add", {}, nullptr);
//   st.add(&func);
//
//   std::string expected = "(call add (number 3) (number 7) (number 10))";
//
//   std::reverse(tokens.begin(), tokens.end());
//   parser::ast::node::expression_t *ex =
//       parser::pratt_parser::parse_expression(tokens, &st);
//
//   ASSERT_EQ(expected, ex->to_prefix_notation());
// }
//
// TEST_F(pratt_parser_unit_test,
//        parse_expression_function_call_with_expression_args) {
//   std::vector<token_t *> tokens;
//   tokens.push_back(create_token_t(tok_id, "calculate", 1));
//   tokens.push_back(create_token_t(tok_lparen, "(", 1));
//   tokens.push_back(create_token_t(tok_number, "3", 1));
//   tokens.push_back(create_token_t(tok_plus, "+", 1));
//   tokens.push_back(create_token_t(tok_number, "2", 1));
//   tokens.push_back(create_token_t(tok_comma, ",", 1));
//   tokens.push_back(create_token_t(tok_number, "7", 1));
//   tokens.push_back(create_token_t(tok_star, "*", 1));
//   tokens.push_back(create_token_t(tok_number, "4", 1));
//   tokens.push_back(create_token_t(tok_rparen, ")", 1));
//   tokens.push_back(create_token_t(tok_semicolon, ";", 1));
//
//   symbol_table::symbol_table_t st;
//   parser::ast::node::function_t func =
//       parser::ast::node::function_t(tok_int, 0, "calculate", {}, nullptr);
//   st.add(&func);
//
//   std::string expected = "(call calculate (plus (number 3) (number 2)) (star
//   "
//                          "(number 7) (number 4)))";
//
//   std::reverse(tokens.begin(), tokens.end());
//   parser::ast::node::expression_t *ex =
//       parser::pratt_parser::parse_expression(tokens, &st);
//
//   ASSERT_EQ(expected, ex->to_prefix_notation());
// }
//
// TEST_F(pratt_parser_unit_test, parse_expression_function_not_declared) {
//   std::vector<token_t *> tokens;
//   tokens.push_back(create_token_t(tok_id, "undefined_func", 1));
//   tokens.push_back(create_token_t(tok_lparen, "(", 1));
//   tokens.push_back(create_token_t(tok_number, "5", 1));
//   tokens.push_back(create_token_t(tok_rparen, ")", 1));
//   tokens.push_back(create_token_t(tok_semicolon, ";", 1));
//
//   symbol_table::symbol_table_t st;
//
//   std::reverse(tokens.begin(), tokens.end());
//   ASSERT_THROW(parser::pratt_parser::parse_expression(tokens, &st),
//                exceptions::variable_not_declared_error);
// }
//
// TEST_F(pratt_parser_unit_test, parse_expression_array_access) {
//   std::vector<token_t *> tokens;
//   tokens.push_back(create_token_t(tok_id, "arr", 1));
//   tokens.push_back(create_token_t(tok_lbracket, "[", 1));
//   tokens.push_back(create_token_t(tok_number, "0", 1));
//   tokens.push_back(create_token_t(tok_rbracket, "]", 1));
//   tokens.push_back(create_token_t(tok_semicolon, ";", 1));
//
//   symbol_table::symbol_table_t st;
//   parser::ast::node::variable_t arr = parser::ast::node::variable_t(
//       tok_int, 0, "arr", nullptr); // TODO: implement size:  array of size 10
//   st.add(&arr);
//
//   std::string expected = "(array_access arr (number 0))";
//
//   std::reverse(tokens.begin(), tokens.end());
//   parser::ast::node::expression_t *ex =
//       parser::pratt_parser::parse_expression(tokens, &st);
//
//   ASSERT_EQ(expected, ex->to_prefix_notation());
// }
//
// TEST_F(pratt_parser_unit_test, parse_expression_array_access_with_expression)
// {
//   std::vector<token_t *> tokens;
//   tokens.push_back(create_token_t(tok_id, "arr", 1));
//   tokens.push_back(create_token_t(tok_lbracket, "[", 1));
//   tokens.push_back(create_token_t(tok_id, "i", 1));
//   tokens.push_back(create_token_t(tok_plus, "+", 1));
//   tokens.push_back(create_token_t(tok_number, "1", 1));
//   tokens.push_back(create_token_t(tok_rbracket, "]", 1));
//   tokens.push_back(create_token_t(tok_semicolon, ";", 1));
//
//   symbol_table::symbol_table_t st;
//   parser::ast::node::variable_t arr = parser::ast::node::variable_t(
//       tok_int, 0, "arr", nullptr); // TODO: array size
//   parser::ast::node::literal_expr_t *i_expr =
//       new parser::ast::node::literal_expr_t(tok_number, "0");
//   parser::ast::node::variable_t i_var =
//       parser::ast::node::variable_t(tok_int, 0, "i", i_expr);
//   st.add(&arr);
//   st.add(&i_var);
//
//   std::string expected = "(array_access arr (plus (id i) (number 1)))";
//
//   std::reverse(tokens.begin(), tokens.end());
//   parser::ast::node::expression_t *ex =
//       parser::pratt_parser::parse_expression(tokens, &st);
//
//   ASSERT_EQ(expected, ex->to_prefix_notation());
// }
//
// TEST_F(pratt_parser_unit_test, parse_expression_nested_array_access) {
//   std::vector<token_t *> tokens;
//   tokens.push_back(create_token_t(tok_id, "matrix", 1));
//   tokens.push_back(create_token_t(tok_lbracket, "[", 1));
//   tokens.push_back(create_token_t(tok_number, "0", 1));
//   tokens.push_back(create_token_t(tok_rbracket, "]", 1));
//   tokens.push_back(create_token_t(tok_lbracket, "[", 1));
//   tokens.push_back(create_token_t(tok_number, "1", 1));
//   tokens.push_back(create_token_t(tok_rbracket, "]", 1));
//   tokens.push_back(create_token_t(tok_semicolon, ";", 1));
//
//   symbol_table::symbol_table_t st;
//   parser::ast::node::variable_t matrix = parser::ast::node::variable_t(
//       tok_int, 0, "matrix", nullptr); // TODO: array size. 2D array
//   st.add(&matrix);
//
//   std::string expected =
//       "(array_access (array_access matrix (number 0)) (number 1))";
//
//   std::reverse(tokens.begin(), tokens.end());
//   parser::ast::node::expression_t *ex =
//       parser::pratt_parser::parse_expression(tokens, &st);
//
//   ASSERT_EQ(expected, ex->to_prefix_notation());
// }
//
// TEST_F(pratt_parser_unit_test, parse_expression_array_in_arithmetic) {
//   std::vector<token_t *> tokens;
//   tokens.push_back(create_token_t(tok_id, "arr", 1));
//   tokens.push_back(create_token_t(tok_lbracket, "[", 1));
//   tokens.push_back(create_token_t(tok_number, "0", 1));
//   tokens.push_back(create_token_t(tok_rbracket, "]", 1));
//   tokens.push_back(create_token_t(tok_plus, "+", 1));
//   tokens.push_back(create_token_t(tok_number, "5", 1));
//   tokens.push_back(create_token_t(tok_semicolon, ";", 1));
//
//   symbol_table::symbol_table_t st;
//   parser::ast::node::variable_t arr = parser::ast::node::variable_t(
//       tok_int, 0, "arr", nullptr); // TODO: array size.
//   st.add(&arr);
//
//   std::string expected = "(plus (array_access arr (number 0)) (number 5))";
//
//   std::reverse(tokens.begin(), tokens.end());
//   parser::ast::node::expression_t *ex =
//       parser::pratt_parser::parse_expression(tokens, &st);
//
//   ASSERT_EQ(expected, ex->to_prefix_notation());
// }
//
// TEST_F(pratt_parser_unit_test, parse_expression_function_call_in_arithmetic)
// {
//   std::vector<token_t *> tokens;
//   tokens.push_back(create_token_t(tok_id, "getValue", 1));
//   tokens.push_back(create_token_t(tok_lparen, "(", 1));
//   tokens.push_back(create_token_t(tok_rparen, ")", 1));
//   tokens.push_back(create_token_t(tok_star, "*", 1));
//   tokens.push_back(create_token_t(tok_number, "2", 1));
//   tokens.push_back(create_token_t(tok_semicolon, ";", 1));
//
//   symbol_table::symbol_table_t st;
//   parser::ast::node::function_t func =
//       parser::ast::node::function_t(tok_int, 0, "getValue", {}, nullptr);
//   st.add(&func);
//
//   std::string expected = "(star (call getValue) (number 2))";
//
//   std::reverse(tokens.begin(), tokens.end());
//   parser::ast::node::expression_t *ex =
//       parser::pratt_parser::parse_expression(tokens, &st);
//
//   ASSERT_EQ(expected, ex->to_prefix_notation());
// }
