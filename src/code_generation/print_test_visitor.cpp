#include "code_generation/print_test_visitor.h"
#include "parser/abstract_syntax_tree.h"
#include "tokens.h"
#include <iostream>

using namespace compiler::ast;
using namespace compiler::parser::ast::node;

void print_test_visitor_t::visit_if(compiler::parser::ast::node::if_t *t_if) {
  printf("if (");
  t_if->p_expr->accept_visitor(this);
  printf(") ");
  t_if->p_stmt->accept_visitor(this);
  printf("\n");
}

void print_test_visitor_t::visit_else(
    compiler::parser::ast::node::else_t *t_else) {
  printf("else ");
  t_else->p_stmt->accept_visitor(this);
  printf("\n");
}

void print_test_visitor_t::visit_for(
    compiler::parser::ast::node::for_t *t_for) {
  printf("for (");
  t_for->p_first->accept_visitor(this);
  printf(";");
  t_for->p_expr->accept_visitor(this);
  printf(";");
  t_for->p_last->accept_visitor(this);
  printf(") ");
  t_for->p_block->accept_visitor(this);
}

void print_test_visitor_t::visit_while(
    compiler::parser::ast::node::while_t *t_while) {
  printf("while (");
  t_while->p_expr->accept_visitor(this);
  printf(") ");
  t_while->p_stmt->accept_visitor(this);
  printf("\n");
}

void print_test_visitor_t::visit_do(compiler::parser::ast::node::do_t *t_do) {
  printf("do ");
  t_do->p_stmt->accept_visitor(this);
  printf(" while (");
  t_do->p_expr->accept_visitor(this);
  printf(")\n");
}

void print_test_visitor_t::visit_return(
    compiler::parser::ast::node::return_t *t_return) {
  printf("return ");
  t_return->p_expr->accept_visitor(this);
  printf(";\n");
}

void print_test_visitor_t::visit_function(
    compiler::parser::ast::node::function_t *t_function) {
  printf("%s", token_t(t_function->type, "", 0).type_name().c_str());
  for (int i = 0; i < t_function->pointer_level; i++) {
    printf("*");
  }
  printf(" %s(", t_function->identifier.c_str());
  for (std::tuple<token_e, int> type_pl :
       *(t_function->parameter_type_pointer_level_tuple)) {
    printf("type: %d pl: %d, ", std::get<token_e>(type_pl),
           std::get<int>(type_pl));
  }
  printf(") ");
  t_function->block->accept_visitor(this);
  printf("\n");
}

void print_test_visitor_t::visit_variable(
    compiler::parser::ast::node::variable_t *t_variable) {
  printf("%s", token_t(t_variable->type, "", 0).type_name().c_str());
  for (int i = 0; i < t_variable->pointer_level; i++) {
    printf("*");
  }
  printf(" %s = ", t_variable->identifier.c_str());
  t_variable->p_expr->accept_visitor(this);
  printf(";\n");
}

void print_test_visitor_t::visit_assign_expr(
    compiler::parser::ast::node::assign_expr_t *t_assign_expr) {
  printf("%s = ", t_assign_expr->identifier.c_str());
  t_assign_expr->right->accept_visitor(this);
  printf(" ");
}

void print_test_visitor_t::visit_unary_expr(
    compiler::parser::ast::node::unary_expr_t *t_unary_expr) {
  printf("%s", token_t(t_unary_expr->op, "", 0).type_name().c_str());
  t_unary_expr->operand->accept_visitor(this);
}

void print_test_visitor_t::visit_call_expr(
    compiler::parser::ast::node::call_expr_t *t_call_expr) {
  printf("call(%s ", t_call_expr->function_name.c_str());
  for (parser::ast::node::expression_t *e : t_call_expr->arguments) {
    e->accept_visitor(this);
  }
  printf(") ");
}

void print_test_visitor_t::visit_subscript_expr(
    compiler::parser::ast::node::subscript_expr_t *t_subscript_expr) {
  (void)t_subscript_expr;
  printf("SUBSCRIPT_NOT_IMPLEMENTED ");
}

void print_test_visitor_t::visit_binary_expr(
    compiler::parser::ast::node::binary_expr_t *t_binary_expr) {
  t_binary_expr->left->accept_visitor(this);
  printf("%s ", token_t(t_binary_expr->op, "", 0).type_name().c_str());
  t_binary_expr->right->accept_visitor(this);
}

template <class... Ts> struct overload : Ts... {
  using Ts::operator()...;
};
template <class... Ts> overload(Ts...) -> overload<Ts...>;

void print_test_visitor_t::visit_literal_expr(
    compiler::parser::ast::node::literal_expr_t *t_literal_expr) {
  (void)t_literal_expr;
  // printf("LITERALEXPRESSION_NOT_IMPLEMENTED ");
  std::visit(overload{[](int v) { std::cout << v << ' '; },
                      [](float v) { std::cout << v << ' '; },
                      [](double v) { std::cout << v << ' '; },
                      [](char v) { std::cout << v << ' '; },
                      [](char *v) { std::cout << v << ' '; },
                      [](const std::string &v) { std::cout << v << ' '; }},
             t_literal_expr->value);
}

void print_test_visitor_t::visit_block(
    compiler::parser::ast::node::block_t *t_block) {
  printf("{\n");
  for (parser::ast::node::statement_t *s : t_block->statements) {
    s->accept_visitor(this);
  }
  printf("}\n");
}
