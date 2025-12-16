#include "parser/abstract_syntax_tree.h"
#include "tokens.h"
#include <memory>
#include <string>
#include <variant>

namespace compiler {
namespace parser {
namespace expression {

class atom_t : public ast::node::expression_t {
public:
  atom_t(token_t *p_token) { this->p_token = p_token; }
  ~atom_t() { free(this->p_token); }

public:
  token_t *p_token;
};

class unary_expr_t;
class binary_expr_t;

class unary_expr_t : public ast::node::expression_t {
public:
  token_t *p_operator;
  expression_t *p_expression;

public:
  unary_expr_t(token_t *p_operator, expression_t *p_expr);
  ~unary_expr_t() {
    free(this->p_operator);
    free(this->p_expression);
  }
};

class binary_expr_t : public ast::node::expression_t {
public:
  token_t *p_operator;
  ast::node::expression_t *left;
  ast::node::expression_t *right;

public:
  binary_expr_t(ast::node::expression_t *p_lhs_expr, token_t *p_operator,
                ast::node::expression_t *p_rhs_expr) {
    this->left = p_lhs_expr;
    this->p_operator = p_operator;
    this->right = p_rhs_expr;
  }
  ~binary_expr_t() {
    free(this->left);
    free(this->p_operator);
    free(this->right);
  }
};

std::string to_prefix_notation(const ast::node::expression_t &expression);

} // namespace expression
} // namespace parser
} // namespace compiler
