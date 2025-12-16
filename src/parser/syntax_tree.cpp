#include "parser/syntax_tree.h"
#include <stdexcept>

namespace compiler::parser::ast {

std::string syntax_tree_t::to_prefix_notation() {
  throw std::runtime_error("to_prefix_notation Not yet implemented!");
  return "";
}

} // namespace compiler::parser::ast
