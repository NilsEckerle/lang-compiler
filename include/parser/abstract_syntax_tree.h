#ifndef ABSTRACT_SYNTAX_TREE_H
#define ABSTRACT_SYNTAX_TREE_H

#include "tokens.h"
#include <string>
#include <tuple>
#include <variant>
#include <vector>

namespace compiler {
namespace parser {
namespace ast {
namespace node {

class node_t {
protected:
  node_t() = default;

public:
  virtual ~node_t() = default;
  virtual std::string debug_print() const = 0;
};

class statement_t : public node_t {
protected:
  statement_t() = default;

public:
  virtual ~statement_t() = default;
  virtual std::string debug_print() const = 0;
};

class expression_t : public node_t {

public:
  expression_t() = default;
  virtual ~expression_t() = default;
  virtual std::string to_prefix_notation() const = 0;
  virtual std::string debug_print() const = 0;
  virtual bool operator==(const expression_t &other) const {
    return debug_print() == other.debug_print();
  }
  virtual bool operator==(const std::string &other) const {
    return debug_print() == other;
  }
  virtual bool operator!=(const expression_t &other) const {
    return !(*this == other);
  }
  virtual bool operator!=(const std::string &other) const {
    return !(*this == other);
  }
};

class if_t : public statement_t {
public:
  if_t(expression_t *p_expr, statement_t *p_statement);
  ~if_t();
  expression_t *p_expr;
  statement_t *p_stmt;
  virtual std::string debug_print() const;
};

class else_t : public statement_t {
public:
  else_t(statement_t *p_statement);
  ~else_t();
  statement_t *p_stmt;
  virtual std::string debug_print() const;
};

class while_t : public statement_t {
public:
  while_t(expression_t *p_expr, statement_t *p_statement);
  ~while_t();
  expression_t *p_expr;
  statement_t *p_stmt;
  virtual std::string debug_print() const;
};

class do_t : public statement_t {
public:
  do_t(expression_t *p_expr, statement_t *p_statement);
  ~do_t();
  expression_t *p_expr;
  statement_t *p_stmt;
  virtual std::string debug_print() const;
};

class return_t : public statement_t {
public:
  return_t(expression_t *p_expr);
  ~return_t();
  expression_t *p_expr;
  virtual std::string debug_print() const;
};

class block_t : public statement_t {
public:
  block_t() = default;
  std::vector<statement_t *> statements;
  virtual std::string debug_print() const;
};

class function_t : public node_t {
public:
  function_t(
      token_e type, int pointer_level, std::string identifier,
      std::vector<std::tuple<token_e, int>> *parameter_type_pointer_level_tuple,
      block_t *block);
  ~function_t();
  virtual std::string debug_print() const;

public:
  token_e type;
  int pointer_level;
  std::vector<std::tuple<token_e, int>> *parameter_type_pointer_level_tuple;
  block_t *block;
  std::string identifier;
};

class variable_t : public statement_t {
public:
  variable_t(token_e type, int pointer_level, std::string identifier,
             expression_t *p_expr, bool is_const = false, bool is_static = false);
  ~variable_t();
  virtual std::string debug_print() const;

public:
  token_e type;
  int pointer_level;
  std::string identifier;
  expression_t *p_expr;
  bool is_const;
  bool is_static;
};

// assign expression node (e.g., a = b, x = 2 * 5 - y)
class assign_expr_t : public expression_t {
public:
  assign_expr_t(std::string identifier, expression_t *right)
      : identifier(identifier), right(right) {}
  ~assign_expr_t() { delete right; }
  virtual std::string to_prefix_notation() const;
  virtual std::string debug_print() const;

public:
  std::string identifier;
  expression_t *right;
};

// Binary expression node (e.g., a + b, x * y, a < b)
class binary_expr_t : public expression_t {
public:
  binary_expr_t(expression_t *left, token_e op, expression_t *right)
      : left(left), op(op), right(right) {}
  ~binary_expr_t() {
    delete left;
    delete right;
  }
  virtual std::string to_prefix_notation() const;
  virtual std::string debug_print() const;

public:
  expression_t *left;
  token_e op;
  expression_t *right;
};

// Unary expression node (e.g., !x, -y, +z)
class unary_expr_t : public expression_t {
public:
  unary_expr_t(token_e op, expression_t *operand) : op(op), operand(operand) {}
  ~unary_expr_t() { delete operand; }
  virtual std::string to_prefix_notation() const;
  virtual std::string debug_print() const;

public:
  token_e op;
  expression_t *operand;
};

using literal_t = std::variant<int, float, double, char, char *, std::string>;

// Literal expression node (numbers, strings, identifiers)
class literal_expr_t : public expression_t {
public:
  literal_expr_t(token_e type, literal_t value) : type(type), value(value) {}
  ~literal_expr_t() = default;
  virtual std::string to_prefix_notation() const;
  virtual std::string debug_print() const;

public:
  token_e type;
  literal_t value;
};

// Function call expression node (e.g., foo(a, b, c))
class call_expr_t : public expression_t {
public:
  call_expr_t(std::string function_name, std::vector<expression_t *> arguments)
      : function_name(function_name), arguments(arguments) {}

  ~call_expr_t() {
    for (auto arg : arguments) {
      delete arg;
    }
  }
  virtual std::string to_prefix_notation() const;
  virtual std::string debug_print() const;

public:
  std::string function_name;
  std::vector<expression_t *> arguments;
};

// Array subscript expression (e.g., arr[i])
class subscript_expr_t : public expression_t {
public:
  subscript_expr_t(expression_t *array, expression_t *index)
      : array(array), index(index) {}

  ~subscript_expr_t() {
    delete array;
    delete index;
  }
  virtual std::string to_prefix_notation() const;
  virtual std::string debug_print() const;

public:
  expression_t *array;
  expression_t *index;
};

} // namespace node

class abstract_syntax_tree_t {
public:
  std::string debug_print();
  node::node_t *p_head;
};

} // namespace ast
} // namespace parser
} // namespace compiler

#endif // !ABSTRACT_SYNTAX_TREE_H
