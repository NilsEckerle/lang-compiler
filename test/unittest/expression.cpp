#include "parser/expression.h"
#include "tokens.h"
#include <gtest/gtest.h>
#include <iostream>
#include <memory>
#include <ostream>
#include <string>

using namespace compiler;
using namespace compiler::parser::expression;

class expression_unit_test : public ::testing::Test {
protected:
  void SetUp() override {}
  void TearDown() override {}
};

TEST_F(expression_unit_test, to_prefix_notation) {
  // Build from innermost to outermost

  // Create the atom for 4
  auto four_expr = new expression_t(atom_t{create_token_t(tok_number, "4", 1)});

  // (-4) : unary minus on 4
  auto unary_neg_4_expr = new expression_t(std::make_unique<unary_expr_t>(
      create_token_t(tok_minus, "-", 1), four_expr));

  // Create the atom for 5
  auto five_expr = new expression_t(atom_t{create_token_t(tok_number, "5", 1)});

  // (* (-4) 5) : multiply
  auto mul_expr = new expression_t(std::make_unique<binary_expr_t>(
      unary_neg_4_expr, create_token_t(tok_star, "*", 1), five_expr));

  // Create the atom for 3
  auto three_expr =
      new expression_t(atom_t{create_token_t(tok_number, "3", 1)});

  // (+ 3 (* (-4) 5)) : add
  expression_t expression = std::make_unique<binary_expr_t>(
      three_expr, create_token_t(tok_plus, "+", 1), mul_expr);

  const auto &binary = std::get<std::unique_ptr<binary_expr_t>>(expression);
  const auto &left = std::get<atom_t>(*binary->left);
  const auto &right = std::get<std::unique_ptr<binary_expr_t>>(*binary->right);
  std::cout << debug_tok(binary->p_operator) << debug_tok(left.p_token)
            << debug_tok(right->p_operator) << std::endl;
  exit(1);

  std::string expected = "(<plus, (+)> (<number, (3)>) (<star, (*)> (<minus, "
                         "(-)> (<number, (4)>)) (<number, (5)>)))";
  std::string result = to_prefix_notation(expression);
  EXPECT_EQ(expected, result);
}
