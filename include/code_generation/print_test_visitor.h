/**
 * @file print_test_visitor.h
 * @brief Defines a visitor that prints a textual representation of AST nodes.
 */

#include "code_generation/ast_visitor_interface.h"
#include "parser/abstract_syntax_tree.h"

namespace compiler::ast {

/**
 * @class print_test_visitor_t
 * @brief Visitor implementation that prints AST nodes in a readable format.
 *
 * This visitor traverses the abstract syntax tree and outputs a textual
 * representation of each node for debugging and testing purposes.
 */
class print_test_visitor_t : public compiler::ast::visitor_t {
  /**
   * @brief Prints a textual representation of an if statement.
   * @param t_if Pointer to the if statement node.
   */
  void visit_if(compiler::parser::ast::node::if_t *t_if);

  /**
   * @brief Prints a textual representation of an else statement.
   * @param t_else Pointer to the else statement node.
   */
  void visit_else(compiler::parser::ast::node::else_t *t_else);

  /**
   * @brief Prints a textual representation of a for loop.
   * @param t_for Pointer to the for loop node.
   */
  void visit_for(compiler::parser::ast::node::for_t *t_for);

  /**
   * @brief Prints a textual representation of a while loop.
   * @param t_while Pointer to the while loop node.
   */
  void visit_while(compiler::parser::ast::node::while_t *t_while);

  /**
   * @brief Prints a textual representation of a do-while loop.
   * @param t_do Pointer to the do-while loop node.
   */
  void visit_do(compiler::parser::ast::node::do_t *t_do);

  /**
   * @brief Prints a textual representation of a return statement.
   * @param t_return Pointer to the return statement node.
   */
  void visit_return(compiler::parser::ast::node::return_t *t_return);

  /**
   * @brief Prints a textual representation of a function declaration.
   * @param t_function Pointer to the function declaration node.
   */
  void visit_function(compiler::parser::ast::node::function_t *t_function);

  /**
   * @brief Prints a textual representation of a variable declaration.
   * @param t_variable Pointer to the variable declaration node.
   */
  void visit_variable(compiler::parser::ast::node::variable_t *t_variable);

  /**
   * @brief Prints a textual representation of an assignment expression.
   * @param t_assign_expr Pointer to the assignment expression node.
   */
  void
  visit_assign_expr(compiler::parser::ast::node::assign_expr_t *t_assign_expr);

  /**
   * @brief Prints a textual representation of a unary expression.
   * @param t_unary_expr Pointer to the unary expression node.
   */
  void
  visit_unary_expr(compiler::parser::ast::node::unary_expr_t *t_unary_expr);

  /**
   * @brief Prints a textual representation of a function call expression.
   * @param t_call_expr Pointer to the function call expression node.
   */
  void visit_call_expr(compiler::parser::ast::node::call_expr_t *t_call_expr);

  /**
   * @brief Prints a textual representation of a subscript/array access
   * expression.
   * @param t_subscript_expr Pointer to the subscript expression node.
   */
  void visit_subscript_expr(
      compiler::parser::ast::node::subscript_expr_t *t_subscript_expr);

  /**
   * @brief Prints a textual representation of a binary expression.
   * @param t_binary_expr Pointer to the binary expression node.
   */
  void
  visit_binary_expr(compiler::parser::ast::node::binary_expr_t *t_binary_expr);

  /**
   * @brief Prints a textual representation of a literal expression.
   * @param t_literal_expr Pointer to the literal expression node.
   */
  void visit_literal_expr(
      compiler::parser::ast::node::literal_expr_t *t_literal_expr);

  /**
   * @brief Prints a textual representation of a code block.
   * @param t_block Pointer to the block node.
   */
  void visit_block(compiler::parser::ast::node::block_t *t_block);
};

} // namespace compiler::ast
