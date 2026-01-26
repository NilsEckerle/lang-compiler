#ifndef VISITOR_H
#define VISITOR_H

/**
 * @file ast_visitor_interface.h
 * @brief Defines the visitor interface for traversing abstract syntax tree
 * nodes.
 */

namespace compiler::parser::ast::node {
class if_t;
class else_t;
class for_t;
class while_t;
class do_t;
class return_t;
class function_t;
class variable_t;
class assign_expr_t;
class unary_expr_t;
class binary_expr_t;
class call_expr_t;
class subscript_expr_t;
class literal_expr_t;
class block_t;
} // namespace compiler::parser::ast::node

namespace compiler::ast {

/**
 * @class visitor_t
 * @brief Abstract base class for implementing the visitor pattern on AST nodes.
 *
 * This interface defines visit methods for all AST node types, allowing
 * different operations to be performed on the abstract syntax tree without
 * modifying the node classes themselves.
 */
class visitor_t {
public:
  visitor_t() = default;
  ~visitor_t() = default;

  /**
   * @brief Visits an if statement node.
   * @param t_if Pointer to the if statement node.
   */
  virtual void visit_if(compiler::parser::ast::node::if_t *t_if) = 0;

  /**
   * @brief Visits an else statement node.
   * @param t_else Pointer to the else statement node.
   */
  virtual void visit_else(compiler::parser::ast::node::else_t *t_else) = 0;

  /**
   * @brief Visits a for loop node.
   * @param t_for Pointer to the for loop node.
   */
  virtual void visit_for(compiler::parser::ast::node::for_t *t_for) = 0;

  /**
   * @brief Visits a while loop node.
   * @param t_while Pointer to the while loop node.
   */
  virtual void visit_while(compiler::parser::ast::node::while_t *t_while) = 0;

  /**
   * @brief Visits a do-while loop node.
   * @param t_do Pointer to the do-while loop node.
   */
  virtual void visit_do(compiler::parser::ast::node::do_t *t_do) = 0;

  /**
   * @brief Visits a return statement node.
   * @param t_return Pointer to the return statement node.
   */
  virtual void
  visit_return(compiler::parser::ast::node::return_t *t_return) = 0;

  /**
   * @brief Visits a function declaration node.
   * @param t_function Pointer to the function declaration node.
   */
  virtual void
  visit_function(compiler::parser::ast::node::function_t *t_function) = 0;

  /**
   * @brief Visits a variable declaration node.
   * @param t_variable Pointer to the variable declaration node.
   */
  virtual void
  visit_variable(compiler::parser::ast::node::variable_t *t_variable) = 0;

  /**
   * @brief Visits an assignment expression node.
   * @param t_assign_expr Pointer to the assignment expression node.
   */
  virtual void visit_assign_expr(
      compiler::parser::ast::node::assign_expr_t *t_assign_expr) = 0;

  /**
   * @brief Visits a unary expression node.
   * @param t_unary_expr Pointer to the unary expression node.
   */
  virtual void
  visit_unary_expr(compiler::parser::ast::node::unary_expr_t *t_unary_expr) = 0;

  /**
   * @brief Visits a function call expression node.
   * @param t_call_expr Pointer to the function call expression node.
   */
  virtual void
  visit_call_expr(compiler::parser::ast::node::call_expr_t *t_call_expr) = 0;

  /**
   * @brief Visits a subscript/array access expression node.
   * @param t_subscript_expr Pointer to the subscript expression node.
   */
  virtual void visit_subscript_expr(
      compiler::parser::ast::node::subscript_expr_t *t_subscript_expr) = 0;

  /**
   * @brief Visits a binary expression node.
   * @param t_binary_expr Pointer to the binary expression node.
   */
  virtual void visit_binary_expr(
      compiler::parser::ast::node::binary_expr_t *t_binary_expr) = 0;

  /**
   * @brief Visits a literal expression node.
   * @param t_literal_expr Pointer to the literal expression node.
   */
  virtual void visit_literal_expr(
      compiler::parser::ast::node::literal_expr_t *t_literal_expr) = 0;

  /**
   * @brief Visits a code block node.
   * @param t_block Pointer to the block node.
   */
  virtual void visit_block(compiler::parser::ast::node::block_t *t_block) = 0;

private:
};

} // namespace compiler::ast

#endif // !VISITOR_H
