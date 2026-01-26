/**
 * @file pratt_parser.h
 * @brief Pratt parser implementation for expression parsing
 *
 * This file implements a Pratt parser (also known as top-down operator
 * precedence parser) for parsing expressions with correct operator precedence
 * and associativity.
 */

#ifndef PRATT_PARSER_H
#define PRATT_PARSER_H

#include "parser/abstract_syntax_tree.h"
#include "parser/expression.h"
#include "symbol_table.h"
#include "tokens.h"
#include <map>
#include <tuple>

namespace compiler {
namespace parser {
namespace pratt_parser {

/**
 * @enum ParseContext
 * @brief Tracks the parsing context to determine operator interpretation
 *
 * Used to distinguish between unary and binary operators based on position.
 */
enum class ParseContext {
  START,          ///< At the beginning of an expression
  AFTER_OPERATOR, ///< After a binary operator
  AFTER_LPAREN,   ///< After an opening parenthesis
  AFTER_OPERAND   ///< After a number, identifier, or closing parenthesis
};

/**
 * @brief Determines if the current context indicates a unary operator
 * @param context The current parse context
 * @return True if context indicates unary operator position
 */
bool is_unary_context(ParseContext context);

/**
 * @brief Gets the precedence level for an operator token
 * @param tok The operator token
 * @return Precedence level (higher values bind more tightly)
 */
int get_presidence(token_e tok);

/**
 * @brief Checks if a token marks the end of an expression
 * @param tok The token to check
 * @return True if the token delimits expressions (semicolon, comma, etc.)
 */
bool is_expression_delimiter(token_e tok);

/**
 * @brief Parses an expression from a token stream
 * @param tokens Vector of tokens to parse (consumed from back)
 * @param p_symbol_table Symbol table for variable/function lookup
 * @param last_op_precedence Precedence of the previous operator (for recursion)
 * @param context Current parsing context (for unary operator detection)
 * @return Pointer to the parsed expression AST node
 *
 * Uses the Pratt parsing algorithm to parse expressions with correct
 * operator precedence and associativity. The token vector is consumed
 * from the back, treating it as a stack.
 */
ast::node::expression_t *
parse_expression(std::vector<token_t *> &tokens,
                 symbol_table::symbol_table_t *p_symbol_table,
                 int last_op_precedence = 0,
                 ParseContext context = ParseContext::START);

} // namespace pratt_parser
} // namespace parser
} // namespace compiler

#endif // !PRATT_PARSER_H
