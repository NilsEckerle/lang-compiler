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

enum class ParseContext {
  START,          // At the beginning
  AFTER_OPERATOR, // After binary operator
  AFTER_LPAREN,   // After '('
  AFTER_OPERAND   // After number/identifier/rparen
};

bool is_unary_context(ParseContext context);

static std::map<token_e, std::tuple<int, int>> presidence_map = {
    // {tok_assign, std::tuple<int, int>{0, 0}},
    // math
    {tok_plus, std::tuple<int, int>{50, 51}},
    {tok_minus, std::tuple<int, int>{50, 51}},
    {tok_star, std::tuple<int, int>{60, 61}},
    {tok_slash, std::tuple<int, int>{60, 61}},
    // {tok_percent, std::tuple<int, int>{60, 61}},
};

int get_presidence(token_e tok);

bool is_expression_delimiter(token_e tok);

ast::node::expression_t *
parse_expression(std::vector<token_t *> &tokens,
                 symbol_table::symbol_table_t *p_symbol_table,
                 int last_op_precedence = 0,
                 ParseContext context = ParseContext::START);

} // namespace pratt_parser
} // namespace parser
} // namespace compiler

#endif // !PRATT_PARSER_H
