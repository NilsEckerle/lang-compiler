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

    data_t *data =
        new data_t(function_node->type, function_node->pointer_level);
    this->table[function_node->identifyer] = data;

  } else if (auto *variable_node =
                 dynamic_cast<parser::ast::node::variable_t *>(p_node)) {

    data_t *data =
        new data_t(variable_node->type, variable_node->pointer_level);
    this->table[variable_node->identifier] = data;

  } else {

    throw exceptions::parser_error("wrong type of node to add to symbol_table");
  }
}

bool symbol_table_t::is_defined(std::string id) {
  for (symbol_table_t *symtab = this; symtab != nullptr;
       symtab = this->p_previous) {
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

} // namespace compiler::symbol_table
