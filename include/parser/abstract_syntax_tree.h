/**
 * @file abstract_syntax_tree.h
 * @brief Abstract Syntax Tree node definitions for the compiler
 *
 * This file defines the node types that make up the abstract syntax tree (AST)
 * representation of parsed source code. The AST represents the hierarchical
 * structure of the program with nodes for statements, expressions, and
 * declarations.
 */

#ifndef ABSTRACT_SYNTAX_TREE_H
#define ABSTRACT_SYNTAX_TREE_H

#include "code_generation/ast_visitor_interface.h"
#include "tokens.h"
#include <string>
#include <tuple>
#include <variant>
#include <vector>

namespace compiler {
namespace parser {
namespace ast {
namespace node {

using namespace compiler::ast;

/**
 * @class node_t
 * @brief Base class for all AST nodes
 *
 * Provides the common interface for all nodes in the abstract syntax tree,
 * including debug printing and visitor pattern support for code generation.
 */
class node_t {
protected:
  node_t() = default;

public:
  virtual ~node_t() = default;

  /**
   * @brief Generates a debug string representation of the node
   * @return String containing debug information about this node
   */
  virtual std::string debug_print() const = 0;

  /**
   * @brief Accepts a visitor for traversing the AST
   * @param visitor Pointer to the visitor implementation
   */
  virtual void accept_visitor(visitor_t *visitor) = 0;
};

/**
 * @class statement_t
 * @brief Base class for all statement nodes
 *
 * Represents executable statements in the program such as variable
 * declarations, control flow structures, and expression statements.
 */
class statement_t : public node_t {
protected:
  statement_t() = default;

public:
  virtual ~statement_t() = default;
  virtual std::string debug_print() const = 0;
  virtual void accept_visitor(visitor_t *visitor) = 0;
};

/**
 * @class expression_t
 * @brief Base class for all expression nodes
 *
 * Represents expressions that evaluate to values, including literals,
 * binary operations, unary operations, and function calls.
 */
class expression_t : public node_t {

public:
  expression_t() = default;
  virtual ~expression_t() = default;

  /**
   * @brief Converts the expression to prefix notation
   * @return String representation in prefix notation (operator before operands)
   */
  virtual std::string to_prefix_notation() const = 0;

  virtual std::string debug_print() const = 0;
  virtual void accept_visitor(visitor_t *visitor) = 0;

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

/**
 * @class if_t
 * @brief Represents an if statement
 *
 * Contains a condition expression and a statement to execute when the condition
 * is true.
 */
class if_t : public statement_t {
public:
  if_t(expression_t *p_expr, statement_t *p_statement);
  ~if_t();

  expression_t *p_expr; ///< Condition expression
  statement_t *p_stmt;  ///< Statement to execute if condition is true

  virtual std::string debug_print() const;
  virtual void accept_visitor(visitor_t *visitor) { visitor->visit_if(this); }
};

/**
 * @class else_t
 * @brief Represents an else statement
 *
 * Contains a statement to execute when the associated if condition is false.
 */
class else_t : public statement_t {
public:
  else_t(statement_t *p_statement);
  ~else_t();

  statement_t *p_stmt; ///< Statement to execute in else branch

  virtual std::string debug_print() const;
  virtual void accept_visitor(visitor_t *visitor) { visitor->visit_else(this); }
};

/**
 * @class for_t
 * @brief Represents a for loop statement
 *
 * Contains initialization, condition, iteration statements, and a loop body.
 */
class for_t : public statement_t {
public:
  for_t(statement_t *p_first, expression_t *p_expr, statement_t *p_last,
        block_t *p_block);
  ~for_t();
  virtual std::string debug_print() const;
  virtual void accept_visitor(visitor_t *visitor) { visitor->visit_for(this); }

public:
  statement_t *p_first; ///< Initialization statement
  expression_t *p_expr; ///< Loop condition expression
  statement_t *p_last;  ///< Iteration statement (executed after each loop)
  block_t *p_block;     ///< Loop body
};

/**
 * @class while_t
 * @brief Represents a while loop statement
 *
 * Contains a condition expression and a statement to execute repeatedly while
 * the condition is true.
 */
class while_t : public statement_t {
public:
  while_t(expression_t *p_expr, statement_t *p_statement);
  ~while_t();

  expression_t *p_expr; ///< Loop condition expression
  statement_t *p_stmt;  ///< Loop body statement

  virtual std::string debug_print() const;
  virtual void accept_visitor(visitor_t *visitor) {
    visitor->visit_while(this);
  }
};

/**
 * @class do_t
 * @brief Represents a do-while loop statement
 *
 * Contains a statement to execute and a condition expression that is checked
 * after each iteration.
 */
class do_t : public statement_t {
public:
  do_t(expression_t *p_expr, statement_t *p_statement);
  ~do_t();

  expression_t *p_expr; ///< Loop condition expression
  statement_t *p_stmt;  ///< Loop body statement

  virtual std::string debug_print() const;
  virtual void accept_visitor(visitor_t *visitor) { visitor->visit_do(this); }
};

/**
 * @class return_t
 * @brief Represents a return statement
 *
 * Contains an expression whose value is returned from the current function.
 */
class return_t : public statement_t {
public:
  return_t(expression_t *p_expr);
  ~return_t();

  expression_t *p_expr; ///< Expression to return

  virtual std::string debug_print() const;
  virtual void accept_visitor(visitor_t *visitor) {
    visitor->visit_return(this);
  }
};

/**
 * @class block_t
 * @brief Represents a block of statements
 *
 * Contains a sequence of statements enclosed in braces, creating a new scope.
 */
class block_t : public statement_t {
public:
  block_t() = default;

  std::vector<statement_t *> statements; ///< Statements within the block

  virtual std::string debug_print() const;
  virtual void accept_visitor(visitor_t *visitor) {
    visitor->visit_block(this);
  }
};

/**
 * @class function_t
 * @brief Represents a function definition
 *
 * Contains the function's return type, name, parameters, and body.
 */
class function_t : public node_t {
public:
  function_t(
      token_e type, int pointer_level, std::string identifier,
      std::vector<std::tuple<token_e, int>> *parameter_type_pointer_level_tuple,
      block_t *block);
  ~function_t();
  virtual std::string debug_print() const;
  virtual void accept_visitor(visitor_t *visitor) {
    visitor->visit_function(this);
  }

public:
  token_e type;      ///< Return type
  int pointer_level; ///< Number of pointer indirections for return type
  std::vector<std::tuple<token_e, int>>
      *parameter_type_pointer_level_tuple; ///< Parameter types and pointer
                                           ///< levels
  block_t *block;                          ///< Function body
  std::string identifier;                  ///< Function name
};

/**
 * @class variable_t
 * @brief Represents a variable declaration
 *
 * Contains the variable's type, name, optional initialization expression, and
 * qualifiers.
 */
class variable_t : public statement_t {
public:
  variable_t(token_e type, int pointer_level, std::string identifier,
             expression_t *p_expr, bool is_const = false,
             bool is_static = false);
  ~variable_t();
  virtual std::string debug_print() const;
  virtual void accept_visitor(visitor_t *visitor) {
    visitor->visit_variable(this);
  }

public:
  token_e type;           ///< Variable type
  int pointer_level;      ///< Number of pointer indirections
  std::string identifier; ///< Variable name
  expression_t *p_expr;   ///< Initialization expression (can be null)
  bool is_const;          ///< Whether variable is declared const
  bool is_static;         ///< Whether variable is declared static
};

/**
 * @class assign_expr_t
 * @brief Represents an assignment expression
 *
 * Assigns the value of an expression to a variable.
 */
class assign_expr_t : public statement_t {
public:
  assign_expr_t(std::string identifier, expression_t *right)
      : identifier(identifier), right(right) {}
  ~assign_expr_t() { delete right; }
  virtual std::string to_prefix_notation() const;
  virtual std::string debug_print() const;
  virtual void accept_visitor(visitor_t *visitor) {
    visitor->visit_assign_expr(this);
  }

public:
  std::string identifier; ///< Variable being assigned to
  expression_t *right;    ///< Expression on the right side of assignment
};

/**
 * @class binary_expr_t
 * @brief Represents a binary expression
 *
 * Contains two operands and a binary operator (e.g., addition, multiplication,
 * comparison).
 */
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
  virtual void accept_visitor(visitor_t *visitor) {
    visitor->visit_binary_expr(this);
  }

public:
  expression_t *left;  ///< Left operand
  token_e op;          ///< Binary operator
  expression_t *right; ///< Right operand
};

/**
 * @class unary_expr_t
 * @brief Represents a unary expression
 *
 * Contains a single operand and a unary operator (e.g., negation, logical NOT).
 */
class unary_expr_t : public expression_t {
public:
  unary_expr_t(token_e op, expression_t *operand) : op(op), operand(operand) {}
  ~unary_expr_t() { delete operand; }
  virtual std::string to_prefix_notation() const;
  virtual std::string debug_print() const;
  virtual void accept_visitor(visitor_t *visitor) {
    visitor->visit_unary_expr(this);
  }

public:
  token_e op;            ///< Unary operator
  expression_t *operand; ///< Operand expression
};

/// @brief Type alias for literal values that can be stored in the AST
using literal_t = std::variant<int, float, double, char, char *, std::string>;

/**
 * @class literal_expr_t
 * @brief Represents a literal value expression
 *
 * Contains a constant value such as a number, string, or character literal.
 */
class literal_expr_t : public expression_t {
public:
  literal_expr_t(token_e type, literal_t value) : type(type), value(value) {}
  ~literal_expr_t() = default;
  virtual std::string to_prefix_notation() const;
  virtual std::string debug_print() const;
  virtual void accept_visitor(visitor_t *visitor) {
    visitor->visit_literal_expr(this);
  }

public:
  token_e type;    ///< Type of the literal
  literal_t value; ///< Value of the literal
};

/**
 * @class call_expr_t
 * @brief Represents a function call expression
 *
 * Contains the function name and a list of argument expressions.
 */
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
  virtual void accept_visitor(visitor_t *visitor) {
    visitor->visit_call_expr(this);
  }

public:
  std::string function_name;             ///< Name of the function being called
  std::vector<expression_t *> arguments; ///< List of argument expressions
};

/**
 * @class subscript_expr_t
 * @brief Represents an array subscript expression
 *
 * Accesses an element of an array using an index expression.
 */
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
  virtual void accept_visitor(visitor_t *visitor) {
    visitor->visit_subscript_expr(this);
  }

public:
  expression_t *array; ///< Array expression being indexed
  expression_t *index; ///< Index expression
};

} // namespace node

/**
 * @class abstract_syntax_tree_t
 * @brief Container for a complete abstract syntax tree
 *
 * Represents a parsed program or program fragment with a root node.
 */
class abstract_syntax_tree_t {
public:
  /**
   * @brief Generates a debug string representation of the entire AST
   * @return String containing the tree structure and node information
   */
  std::string debug_print();

  node::node_t *p_head; ///< Root node of the abstract syntax tree
};

} // namespace ast
} // namespace parser
} // namespace compiler

#endif // !ABSTRACT_SYNTAX_TREE_H
