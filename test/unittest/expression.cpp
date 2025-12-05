#include "parser/expression.h"
#include "tokens.h"
#include <gtest/gtest.h>
#include <memory>
#include <string>

using namespace parser::expression;

class expression_unit_test : public ::testing::Test {
protected:
  void SetUp() override {}
  void TearDown() override {}
};

TEST_F(expression_unit_test, expression_to_string) {
  // Build from innermost to outermost
  
  // (-4) : unary minus on 4
  auto unary_neg_4 = std::make_unique<unary_expr_t>(unary_expr_t{
      lexer::create_token_t(lexer::tok_minus, "", 1),
    atom_t{lexer::create_token_t(lexer::tok_number, "4", 1)}
  });

  // (* (-4) 5) : multiply
  auto mul_expr = std::make_unique<binary_expr_t>(binary_expr_t{
      lexer::create_token_t(lexer::tok_star, "", 1),
    std::move(unary_neg_4),
    atom_t{lexer::create_token_t(lexer::tok_number, "5", 1)}
  });

  // (+ 3 (* (-4) 5)) : add
  expression_t expression = std::make_unique<binary_expr_t>(binary_expr_t{
      lexer::create_token_t(lexer::tok_plus, "", 1),
    atom_t{lexer::create_token_t(lexer::tok_number, "3", 1)},
    std::move(mul_expr)
  });

  std::string expected = "(<plus> (<number, (3)>) (<star> (<minus> (<number, (4)>)) (<number, (5)>)))";
  std::string result = debug::expression_to_string(expression);
  EXPECT_EQ(expected, result);
}
