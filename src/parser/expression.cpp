
#include "parser/expression.h"
#include "lexer/lexer.h"
#include <spdlog/fmt/fmt.h>
#include <string>

namespace parser {
namespace expression {
namespace debug {

std::string expression_to_string(const expression_t &expression) {
  if (std::holds_alternative<atom_t>(expression)) {
    const auto &atom = std::get<atom_t>(expression);
    return fmt::format("({})", lexer::debug_tok(atom.p_token));
  } else if (std::holds_alternative<std::unique_ptr<unary_expr_t>>(
                 expression)) {
    const auto &unary = std::get<std::unique_ptr<unary_expr_t>>(expression);
    return fmt::format("({} {})", lexer::debug_tok(unary->p_operator),
                       expression_to_string(unary->operand));
  } else if (std::holds_alternative<std::unique_ptr<binary_expr_t>>(
                 expression)) {
    const auto &binary = std::get<std::unique_ptr<binary_expr_t>>(expression);
    return fmt::format("({} {} {})", lexer::debug_tok(binary->p_operator),
                       expression_to_string(binary->left),
                       expression_to_string(binary->right));
  }
  return ""; // unreachable if variant is well-defined
}

} // namespace debug
} // namespace expression
} // namespace parser
