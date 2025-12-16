#ifndef PARSER_H
#define PARSER_H

#include "parser/abstract_syntax_tree.h"
#include "tokens.h"
#include <vector>

namespace compiler {
namespace parser {

class parser_t {};

std::vector<ast::abstract_syntax_tree_t *> *
parse_tokens(std::vector<token_t *> &tokens);

} // namespace parser

} // namespace compiler
#endif // !PARSER_H
