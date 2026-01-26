#ifndef CODEGEN_VISITOR_H
#define CODEGEN_VISITOR_H

/**
 * @file codegen_visitor.h
 * @brief Defines a visitor that generates LLVM IR code from AST nodes.
 */

#include "code_generation/ast_visitor_interface.h"
#include "parser/abstract_syntax_tree.h"
#include "tokens.h"
#include <llvm/IR/IRBuilder.h>
#include <llvm/IR/LLVMContext.h>
#include <llvm/IR/Module.h>
#include <llvm/IR/Value.h>
#include <map>
#include <string>

namespace compiler::ast {

/**
 * @class codegen_visitor_t
 * @brief Visitor implementation that generates LLVM IR from AST nodes.
 *
 * This visitor traverses the abstract syntax tree and generates corresponding
 * LLVM Intermediate Representation code for compilation.
 */
class codegen_visitor_t : public compiler::ast::visitor_t {
public:
  codegen_visitor_t();
  ~codegen_visitor_t() = default;

  /**
   * @brief Generates LLVM IR for an if statement.
   * @param t_if Pointer to the if statement node.
   */
  void visit_if(compiler::parser::ast::node::if_t *t_if) override;

  /**
   * @brief Generates LLVM IR for an else statement.
   * @param t_else Pointer to the else statement node.
   */
  void visit_else(compiler::parser::ast::node::else_t *t_else) override;

  /**
   * @brief Generates LLVM IR for a for loop.
   * @param t_for Pointer to the for loop node.
   */
  void visit_for(compiler::parser::ast::node::for_t *t_for) override;

  /**
   * @brief Generates LLVM IR for a while loop.
   * @param t_while Pointer to the while loop node.
   */
  void visit_while(compiler::parser::ast::node::while_t *t_while) override;

  /**
   * @brief Generates LLVM IR for a do-while loop.
   * @param t_do Pointer to the do-while loop node.
   */
  void visit_do(compiler::parser::ast::node::do_t *t_do) override;

  /**
   * @brief Generates LLVM IR for a return statement.
   * @param t_return Pointer to the return statement node.
   */
  void visit_return(compiler::parser::ast::node::return_t *t_return) override;

  /**
   * @brief Generates LLVM IR for a function declaration.
   * @param t_function Pointer to the function declaration node.
   */
  void
  visit_function(compiler::parser::ast::node::function_t *t_function) override;

  /**
   * @brief Generates LLVM IR for a variable declaration.
   * @param t_variable Pointer to the variable declaration node.
   */
  void
  visit_variable(compiler::parser::ast::node::variable_t *t_variable) override;

  /**
   * @brief Generates LLVM IR for an assignment expression.
   * @param t_assign_expr Pointer to the assignment expression node.
   */
  void visit_assign_expr(
      compiler::parser::ast::node::assign_expr_t *t_assign_expr) override;

  /**
   * @brief Generates LLVM IR for a unary expression.
   * @param t_unary_expr Pointer to the unary expression node.
   */
  void visit_unary_expr(
      compiler::parser::ast::node::unary_expr_t *t_unary_expr) override;

  /**
   * @brief Generates LLVM IR for a function call expression.
   * @param t_call_expr Pointer to the function call expression node.
   */
  void visit_call_expr(
      compiler::parser::ast::node::call_expr_t *t_call_expr) override;

  /**
   * @brief Generates LLVM IR for a subscript/array access expression.
   * @param t_subscript_expr Pointer to the subscript expression node.
   */
  void visit_subscript_expr(
      compiler::parser::ast::node::subscript_expr_t *t_subscript_expr) override;

  /**
   * @brief Generates LLVM IR for a binary expression.
   * @param t_binary_expr Pointer to the binary expression node.
   */
  void visit_binary_expr(
      compiler::parser::ast::node::binary_expr_t *t_binary_expr) override;

  /**
   * @brief Generates LLVM IR for a literal expression.
   * @param t_literal_expr Pointer to the literal expression node.
   */
  void visit_literal_expr(
      compiler::parser::ast::node::literal_expr_t *t_literal_expr) override;

  /**
   * @brief Generates LLVM IR for a code block.
   * @param t_block Pointer to the block node.
   */
  void visit_block(compiler::parser::ast::node::block_t *t_block) override;

  /**
   * @brief Retrieves the generated LLVM module.
   * @return Pointer to the LLVM module containing generated IR.
   */
  llvm::Module *get_module() { return module.get(); }

  /**
   * @brief Retrieves the last generated LLVM value.
   * @return Pointer to the most recently generated LLVM value.
   */
  llvm::Value *get_value() { return last_value; }

  /**
   * @brief Outputs the generated LLVM IR to standard output.
   */
  void dump_ir();

private:
  std::unique_ptr<llvm::LLVMContext> context;
  std::unique_ptr<llvm::IRBuilder<>> builder;
  std::unique_ptr<llvm::Module> module;
  std::map<std::string, llvm::AllocaInst *> named_values;

  llvm::Value *last_value;
  llvm::Function *current_function;

  /**
   * @brief Converts a token type to the corresponding LLVM type.
   * @param type The token type representing the source type.
   * @param pointer_level The level of pointer indirection.
   * @return The corresponding LLVM type.
   */
  llvm::Type *get_llvm_type(token_e type, int pointer_level);

  /**
   * @brief Creates an alloca instruction in the entry block of a function.
   * @param func The function in which to create the alloca.
   * @param var_name The name of the variable.
   * @param type The LLVM type of the variable.
   * @return Pointer to the created alloca instruction.
   */
  llvm::AllocaInst *create_entry_block_alloca(llvm::Function *func,
                                              const std::string &var_name,
                                              llvm::Type *type);

  /**
   * @brief Logs an error message and returns nullptr.
   * @param str The error message to log.
   * @return nullptr value.
   */
  llvm::Value *log_error_v(const char *str);
};

} // namespace compiler::ast

#endif // CODEGEN_VISITOR_H
