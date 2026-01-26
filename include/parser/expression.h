/**
 * @file expression.h
 * @brief Alternative expression node types for parsing
 *
 * This file defines additional expression node types used during parsing,
 * providing an alternative representation before conversion to the final AST.
 */

#include "parser/abstract_syntax_tree.h"
#include "tokens.h"
#include <memory>
#include <string>
#include <variant>

namespace compiler {
namespace parser {
namespace expression {

/**
 * @class atom_t
 * @brief Represents an atomic expression token
 *
 * Wraps a single token as an expression during the parsing process.
 */
class atom_t : public ast::node::expression_t {
public:
  atom_t(token_t *p_token) { this->p_token = p_token; }
  ~atom_t() { free(this->p_token); }

public:
  token_t *p_token; ///< The token representing this atomic expression
};

class unary_expr_t;
class binary_expr_t;

/**
 * @class unary_expr_t
 * @brief Represents a unary expression during parsing
 *
 * Contains an operator token and an operand expression.
 */
class unary_expr_t : public ast::node::expression_t {
public:
  token_t *p_operator;        ///< Unary operator token
  expression_t *p_expression; ///< Operand expression

public:
  unary_expr_t(token_t *p_operator, expression_t *p_expr);
  ~unary_expr_t() {
    free(this->p_operator);
    free(this->p_expression);
  }
};

/**
 * @class binary_expr_t
 * @brief Represents a binary expression during parsing
 *
 * Contains an operator token and left and right operand expressions.
 */
class binary_expr_t : public ast::node::expression_t {
public:
  token_t *p_operator;            ///< Binary operator token
  ast::node::expression_t *left;  ///< Left operand
  ast::node::expression_t *right; ///< Right operand

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

/**
 * @brief Converts an expression to prefix notation
 * @param expression The expression to convert
 * @return String representation in prefix notation
 */
std::string to_prefix_notation(const ast::node::expression_t &expression);

} // namespace expression
} // namespace parser
} // namespace compiler
