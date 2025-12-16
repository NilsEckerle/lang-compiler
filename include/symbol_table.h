#ifndef SYMBOL_TABLE_H
#define SYMBOL_TABLE_H

#include "parser/abstract_syntax_tree.h"
#include "tokens.h"
#include <map>
#include <string>
#include <vector>
namespace compiler::symbol_table {
class data_t {
public:
  data_t(token_e type, int pointer_level = 0) {
    this->type = type;
    this->pointer_level = pointer_level;
  }

public:
  token_e type;
  int pointer_level;
  std::vector<token_e> parameter_types;
};

class symbol_table_t {
public:
  symbol_table_t();
  symbol_table_t(symbol_table_t *p_previous);
  bool is_defined(std::string id);

public:
  symbol_table_t *p_previous;
  std::map<std::string, data_t *> table;
  void add(parser::ast::node::node_t *p_node);
  data_t *get(std::string id) { return this->table.find(id)->second; }
};

} // namespace compiler::symbol_table

#endif // !SYMBOL_TABLE_H
