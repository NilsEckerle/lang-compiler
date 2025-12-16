#include "parser/expression.h"
#include "lexer/lexer.h"
#include "parser/abstract_syntax_tree.h"
// #include <spdlog/fmt/fmt.h>
#include <string>
#include <variant>

namespace compiler {
namespace parser {
namespace expression {

std::string to_prefix_notation(const ast::node::expression_t &expression) {

  // if (std::holds_alternative<atom_t>(expression)) {
  //   const auto &atom = std::get<atom_t>(expression);
  //   return fmt::format("({})", debug_tok(atom.p_token));
  // } else if (std::holds_alternative<std::unique_ptr<unary_expr_t>>(
  //                expression)) {
  //   const auto &unary = std::get<std::unique_ptr<unary_expr_t>>(expression);
  //   return fmt::format("({} {})", debug_tok(unary->p_operator),
  //                      to_prefix_notation(*unary->p_expression));
  // } else if (std::holds_alternative<std::unique_ptr<binary_expr_t>>(
  //                expression)) {
  //   const auto &binary =
  //   std::get<std::unique_ptr<binary_expr_t>>(expression); return
  //   fmt::format("({} {} {})", debug_tok(binary->p_operator),
  //                      to_prefix_notation(*binary->left),
  //                      to_prefix_notation(*binary->right));
  // }
  (void)expression;
  return ""; // unreachable if variant is well-defined
}

unary_expr_t::unary_expr_t(token_t *p_operator, expression_t *p_expr) {
  this->p_operator = p_operator;
  this->p_expression = p_expr;
}
} // namespace expression
} // namespace parser
} // namespace compiler
