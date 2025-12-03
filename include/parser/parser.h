#ifndef PARSER_H
#define PARSER_H

#include "parser/syntax_tree.h"
#include "tokens.h"
#include <vector>
namespace parser {

class parser_t {};

ast::syntax_tree_t *parse_tokens(std::vector<lexer::token_t *> tokens);

} // namespace parser

#endif // !PARSER_H
