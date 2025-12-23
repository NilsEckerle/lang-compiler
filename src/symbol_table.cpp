#include "symbol_table.h"
#include "exceptions.h"
#include "parser/abstract_syntax_tree.h"

namespace compiler::symbol_table {

symbol_table_t::symbol_table_t() { this->p_previous = nullptr; }
symbol_table_t::symbol_table_t(symbol_table_t *p_previous) {
  this->p_previous = p_previous;
}
void symbol_table_t::add(parser::ast::node::node_t *p_node) {
  if (auto *function_node =
          dynamic_cast<parser::ast::node::function_t *>(p_node)) {

    // TODO: change is_const and is_static default values to actual values after
    // implementing this in the lexer and parser
    data_t *data = new data_t(function_node->type, function_node->pointer_level,
                              (parser::ast::node::expression_t *)nullptr, false,
                              false, true);
    this->table[function_node->identifier] = data;

  } else if (auto *variable_node =
                 dynamic_cast<parser::ast::node::variable_t *>(p_node)) {

    if (this->is_defined(variable_node->identifier)) {
      throw exceptions::variable_already_declared_error(
          "The variable already exists in the scope");
    }
    // TODO: change is_const and is_static default values to actual values after
    // implementing this in the lexer and parser
    data_t *data = new data_t(variable_node->type, variable_node->pointer_level,
                              variable_node->p_expr, variable_node->is_const,
                              variable_node->is_static, false);
    this->table[variable_node->identifier] = data;

  } else {

    throw exceptions::parser_error("wrong type of node to add to symbol_table");
  }
}

bool symbol_table_t::is_defined(std::string id) {
  for (symbol_table_t *symtab = this; symtab != nullptr;
       symtab = symtab->p_previous) {
    for (std::pair<const std::basic_string<char>,
                   compiler::symbol_table::data_t *>
             entry : symtab->table) {
      if (entry.first == id) {
        return true;
      }
    }
  }
  return false;
}

void symbol_table_t::assign(parser::ast::node::assign_expr_t *p_assign) {
  if (!this->is_defined(p_assign->identifier)) {
    throw exceptions::variable_not_declared_error(
        fmt::format("Variable {} is not defined, thus it's not possible to "
                    "assign something to it",
                    p_assign->identifier));
  }

  this->table[p_assign->identifier]->set_expr(p_assign->right);
}

parser::ast::node::expression_t *data_t::get_expr() { return this->p_expr; }

void data_t::set_expr(parser::ast::node::expression_t *p_expr) {
  free(this->p_expr);
  this->p_expr = p_expr;
}

} // namespace compiler::symbol_table
