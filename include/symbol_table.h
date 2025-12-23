#ifndef SYMBOL_TABLE_H
#define SYMBOL_TABLE_H

#include "parser/abstract_syntax_tree.h"
#include "tokens.h"
#include <map>
#include <string>
#include <tuple>
#include <variant>
#include <vector>

namespace compiler::symbol_table {

using value_t = std::variant<int, float, char, char *, std::string, bool,
                             void *, parser::ast::node::expression_t *>;

class data_t {
public:
  data_t(token_e type, int pointer_level, value_t value, bool is_const,
         bool is_static, bool is_function) {
    this->type = type;
    this->pointer_level = pointer_level;
    this->is_const = is_const;
    this->is_static = is_static;
    this->is_function = is_function;
    this->value = value;
  }
  value_t get_value();

public:
  token_e type;
  int pointer_level;
  std::vector<std::tuple<token_e, int>> parameter_type_pointer_level;
  bool is_const;
  bool is_static;
  bool is_function;

private:
  value_t value;
};

class symbol_table_t {
public:
  symbol_table_t();
  symbol_table_t(symbol_table_t *p_previous);
  bool is_defined(std::string id);
  void add(parser::ast::node::node_t *p_node);
  data_t *get(std::string id) { return this->table.find(id)->second; }

public:
  symbol_table_t *p_previous;
  std::map<std::string, data_t *> table;
};

} // namespace compiler::symbol_table

#endif // !SYMBOL_TABLE_H
