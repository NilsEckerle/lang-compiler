/**
 * @file parser.h
 * @brief Main parser interface for converting tokens to AST
 *
 * This file defines the parser interface for transforming a stream of tokens
 * into an abstract syntax tree representation.
 */

#ifndef PARSER_H
#define PARSER_H

#include "parser/abstract_syntax_tree.h"
#include "tokens.h"
#include <vector>

namespace compiler {
namespace parser {

/**
 * @class parser_t
 * @brief Base parser class
 *
 * Placeholder for future parser functionality.
 */
class parser_t {};

/**
 * @brief Parses a vector of tokens into abstract syntax trees
 * @param tokens Vector of token pointers to parse
 * @return Vector of AST pointers representing the parsed program
 *
 * Processes the token stream and constructs ASTs for all top-level
 * declarations and definitions in the source code.
 */
std::vector<ast::abstract_syntax_tree_t *> *
parse_tokens(std::vector<token_t *> &tokens);

} // namespace parser

} // namespace compiler
#endif // !PARSER_H
