#ifndef SYNTAX_TREE_H
#define SYNTAX_TREE_H

#include "tokens.h"
#include <string>

namespace compiler {
namespace parser {
namespace ast {

class syntax_tree_t {
public:
  std::string to_prefix_notation();
};

class operant {
private:
  token_t *m_tok;
};

class operation {
private:
  token_t *m_tok;
  int lh_precedence;
  int rh_precedence;
};

class binary_operation {
private:
  operation m_operation;
  operant m_lh_operant;
  operant m_rh_operant;
};

} // namespace ast
} // namespace parser
} // namespace compiler

#endif // !SYNTAX_TREE_H
