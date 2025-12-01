#include "parser/parser.h"
#include "parser/syntax_tree.h"
#include "tokens.h"
#include <stdexcept>
#include <vector>

namespace parser {

syntax_tree_t *parse_tokens(std::vector<lexer::token_t *> tokens) {
  tokens = tokens;
  throw std::runtime_error("parse_tokens Not yet implemented!");
  return new syntax_tree_t;
}

} // namespace parser
