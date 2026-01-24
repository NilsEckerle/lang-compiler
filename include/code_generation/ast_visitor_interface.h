#ifndef VISITOR_H
#define VISITOR_H

namespace compiler::parser::ast::node {
class if_t;
class else_t;
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

class visitor_t {
public:
  visitor_t() = default;
  ~visitor_t() = default;

  virtual void visit_if(compiler::parser::ast::node::if_t *t_if) = 0;
  virtual void visit_else(compiler::parser::ast::node::else_t *t_else) = 0;
  virtual void visit_while(compiler::parser::ast::node::while_t *t_while) = 0;
  virtual void visit_do(compiler::parser::ast::node::do_t *t_do) = 0;
  virtual void
  visit_return(compiler::parser::ast::node::return_t *t_return) = 0;
  virtual void
  visit_function(compiler::parser::ast::node::function_t *t_function) = 0;
  virtual void
  visit_variable(compiler::parser::ast::node::variable_t *t_variable) = 0;
  virtual void visit_assign_expr(
      compiler::parser::ast::node::assign_expr_t *t_assign_expr) = 0;
  virtual void
  visit_unary_expr(compiler::parser::ast::node::unary_expr_t *t_unary_expr) = 0;
  virtual void
  visit_call_expr(compiler::parser::ast::node::call_expr_t *t_call_expr) = 0;
  virtual void visit_subscript_expr(
      compiler::parser::ast::node::subscript_expr_t *t_subscript_expr) = 0;
  virtual void visit_binary_expr(
      compiler::parser::ast::node::binary_expr_t *t_binary_expr) = 0;
  virtual void visit_literal_expr(
      compiler::parser::ast::node::literal_expr_t *t_literal_expr) = 0;
  virtual void visit_block(compiler::parser::ast::node::block_t *t_block) = 0;

private:
};

} // namespace compiler::ast

#endif // !VISITOR_H
