#ifndef CODEGEN_VISITOR_H
#define CODEGEN_VISITOR_H

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

class codegen_visitor_t : public compiler::ast::visitor_t {
public:
  codegen_visitor_t();
  ~codegen_visitor_t() = default;

  void visit_if(compiler::parser::ast::node::if_t *t_if) override;
  void visit_else(compiler::parser::ast::node::else_t *t_else) override;
  void visit_for(compiler::parser::ast::node::for_t *t_for) override;
  void visit_while(compiler::parser::ast::node::while_t *t_while) override;
  void visit_do(compiler::parser::ast::node::do_t *t_do) override;
  void visit_return(compiler::parser::ast::node::return_t *t_return) override;
  void
  visit_function(compiler::parser::ast::node::function_t *t_function) override;
  void
  visit_variable(compiler::parser::ast::node::variable_t *t_variable) override;
  void visit_assign_expr(
      compiler::parser::ast::node::assign_expr_t *t_assign_expr) override;
  void visit_unary_expr(
      compiler::parser::ast::node::unary_expr_t *t_unary_expr) override;
  void visit_call_expr(
      compiler::parser::ast::node::call_expr_t *t_call_expr) override;
  void visit_subscript_expr(
      compiler::parser::ast::node::subscript_expr_t *t_subscript_expr) override;
  void visit_binary_expr(
      compiler::parser::ast::node::binary_expr_t *t_binary_expr) override;
  void visit_literal_expr(
      compiler::parser::ast::node::literal_expr_t *t_literal_expr) override;
  void visit_block(compiler::parser::ast::node::block_t *t_block) override;

  // Get the generated module
  llvm::Module *get_module() { return module.get(); }

  // Get the last generated value
  llvm::Value *get_value() { return last_value; }

  // Dump the generated IR to stdout
  void dump_ir();

private:
  std::unique_ptr<llvm::LLVMContext> context;
  std::unique_ptr<llvm::IRBuilder<>> builder;
  std::unique_ptr<llvm::Module> module;
  std::map<std::string, llvm::AllocaInst *> named_values;

  llvm::Value *last_value;
  llvm::Function *current_function;

  // Helper methods
  llvm::Type *get_llvm_type(token_e type, int pointer_level);
  llvm::AllocaInst *create_entry_block_alloca(llvm::Function *func,
                                              const std::string &var_name,
                                              llvm::Type *type);
  llvm::Value *log_error_v(const char *str);
};

} // namespace compiler::ast

#endif // CODEGEN_VISITOR_H
