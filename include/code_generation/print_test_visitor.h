#include "code_generation/ast_visitor_interface.h"
#include "parser/abstract_syntax_tree.h"
#include "tokens.h"
#include <cstdio>
#include <tuple>

namespace compiler::ast {

class print_test_visitor_t : public compiler::ast::visitor_t {
  void visit_if(compiler::parser::ast::node::if_t *t_if);

  void visit_else(compiler::parser::ast::node::else_t *t_else);

  void visit_for(compiler::parser::ast::node::for_t *t_for);

  void visit_while(compiler::parser::ast::node::while_t *t_while);

  void visit_do(compiler::parser::ast::node::do_t *t_do);

  void visit_return(compiler::parser::ast::node::return_t *t_return);

  void visit_function(compiler::parser::ast::node::function_t *t_function);

  void visit_variable(compiler::parser::ast::node::variable_t *t_variable);

  void
  visit_assign_expr(compiler::parser::ast::node::assign_expr_t *t_assign_expr);

  void
  visit_unary_expr(compiler::parser::ast::node::unary_expr_t *t_unary_expr);

  void visit_call_expr(compiler::parser::ast::node::call_expr_t *t_call_expr);

  void visit_subscript_expr(
      compiler::parser::ast::node::subscript_expr_t *t_subscript_expr);

  void
  visit_binary_expr(compiler::parser::ast::node::binary_expr_t *t_binary_expr);

  void visit_literal_expr(
      compiler::parser::ast::node::literal_expr_t *t_literal_expr);

  void visit_block(compiler::parser::ast::node::block_t *t_block);
};

} // namespace compiler::ast
