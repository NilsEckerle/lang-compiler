#include "tokens.h"
#include <memory>
#include <string>
#include <variant>

namespace parser {

namespace expression {

struct atom_t {
  lexer::token_t *p_token;
};

struct unary_expr_t;
struct binary_expr_t;

using expression_t = std::variant<atom_t, std::unique_ptr<unary_expr_t>,
                                  std::unique_ptr<binary_expr_t>>;

struct unary_expr_t {
  lexer::token_t *p_operator;
  expression_t operand;
};

struct binary_expr_t {
  lexer::token_t *p_operator;
  expression_t left;
  expression_t right;
};

namespace debug {

std::string expression_to_string(const expression_t &expression);

}
} // namespace expression
} // namespace parser
