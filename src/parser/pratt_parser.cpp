// The main pratt parsing part was completly written by claude Sonnet 4.5
// because my little brain can't plan and write a pratt parser. it failed at
// laying out the logic in comments.
// TODO: rewrite this

#include "parser/pratt_parser.h"
#include "exceptions.h"
#include "parser/abstract_syntax_tree.h"
#include "symbol_table.h"
#include "tokens.h"
#include <cassert>
#include <cstddef>
#include <iostream>
#include <ostream>
#include <stdexcept>
#include <vector>

namespace compiler {
namespace parser {
namespace pratt_parser {

int get_precedence(token_e tok) {
  switch (tok) {

  case tok_assign:
    return 0;

    // case OR (||):
    // return 1;

    // case AND (&&):
    // return 2;

  case tok_eq:  // ==
  case tok_neq: // !=
    return 3;

  case tok_lt:  //
  case tok_leq: // <=
  case tok_gt:  // >
  case tok_geq: // >=
    return 4;

  case tok_plus:  // +
  case tok_minus: // -
    return 5;

  case tok_star:  // *
  case tok_slash: // /
                  // case tok_percent: // %
    return 6;

  case tok_exclaimationmark: // *
    return 7;

  case tok_lparen: // (
    return 10;
  case tok_rparen: // )
    return 0;

  case tok_eof:
  case tok_comment:
  case tok_bool:
  case tok_char:
  case tok_int:
  case tok_float:
  case tok_void:
  case tok_if:
  case tok_else:
  case tok_while:
  case tok_for:
  case tok_return:
  case tok_lbrace:
  case tok_rbrace:
  case tok_lbracket:
  case tok_rbracket:
  case tok_comma:
  case tok_semicolon:
  case tok_colon:
  case tok_id:
  case tok_number:
  case tok_string:
  case tok_char_literal:
    break;
  }
  return -1; // Not an operator
}

bool is_expression_delimiter(token_e tok) {
  switch (tok) {
  case tok_semicolon:
  case tok_comma:
  case tok_colon:
  case tok_rparen:
  case tok_rbrace:
  case tok_rbracket:
    return true;
  default:
    return false;
  }
}

token_t *consume(std::vector<token_t *> &tokens) {
  token_t *tok = tokens.back();
  tokens.pop_back();
  return tok;
}

token_t *peek(std::vector<token_t *> tokens, size_t n = 1) {
  if (n > tokens.size()) {
    throw std::range_error("peek out of range");
  }

  token_t *tok = tokens.at(tokens.size() - n);

  return tok;
}

void match(std::vector<token_t *> &tokens, token_e type) {
  if (peek(tokens)->e_tok_type != type) {
    throw exceptions::parser_error("Doesn't match expected token type",
                                   peek(tokens));
  }
  tokens.pop_back();
  return;
}

int next_op_pressidence(const std::vector<token_t *> &tokens) {
  token_t *tok;

  for (size_t i = 1; i < tokens.size(); i++) {

    try {
      tok = peek(tokens, i);
    } catch (const std::range_error &e) {
      return 0;
    }

    // Check if it's an operator
    switch (tok->e_tok_type) {
    case tok_plus:
    case tok_minus:
    case tok_star:
    case tok_slash:
    case tok_lparen:
      // case tok_percent:
      return get_precedence(tok->e_tok_type);
    case tok_eof:
    case tok_comment:
    case tok_bool:
    case tok_char:
    case tok_int:
    case tok_float:
    case tok_void:
    case tok_if:
    case tok_else:
    case tok_while:
    case tok_for:
    case tok_return:
    case tok_assign:
    case tok_lt:
    case tok_leq:
    case tok_gt:
    case tok_geq:
    case tok_eq:
    case tok_neq:
    case tok_exclaimationmark:
    case tok_rparen:
    case tok_lbrace:
    case tok_rbrace:
    case tok_lbracket:
    case tok_rbracket:
    case tok_comma:
    case tok_semicolon:
    case tok_colon:
    case tok_id:
    case tok_number:
    case tok_string:
    case tok_char_literal:
      break;
    }
  }

  return 0;
}

// Forward declarations
ast::node::expression_t *parse_primary(std::vector<token_t *> &tokens,
                                       ParseContext context);

/**
 * Create binary expression node
 */
ast::node::expression_t *create_binary_expression(ast::node::expression_t *lhs,
                                                  token_t *op,
                                                  ast::node::expression_t *rhs,
                                                  ParseContext context) {
  (void)context;
  return new ast::node::binary_expr_t(lhs, op->e_tok_type, rhs);
}

/**
 * Create unary expression node
 */
ast::node::expression_t *
create_unary_expression(token_t *op, ast::node::expression_t *operand,
                        ParseContext context) {
  (void)context;
  return new ast::node::unary_expr_t(op->e_tok_type, operand);
}

/**
 * Create literal expression node
 */
ast::node::expression_t *create_literal_expression(token_t *tok,
                                                   ParseContext context) {
  (void)context;

  // Convert string to appropriate type based on token type
  switch (tok->e_tok_type) {
  case token_e::tok_number: {
    // Convert string to int
    int value = std::stoi(tok->t_val);
    return new ast::node::literal_expr_t(tok->e_tok_type, value);
  }

  case token_e::tok_float: {
    // Convert string to float
    float value = std::stof(tok->t_val);
    return new ast::node::literal_expr_t(tok->e_tok_type, value);
  }

  case token_e::tok_char_literal: {
    // Get the character (assuming it's already parsed correctly)
    char value = tok->t_val[0];
    return new ast::node::literal_expr_t(tok->e_tok_type, value);
  }

  case token_e::tok_string:
  case token_e::tok_id:
  default: {
    // Keep as string for identifiers and string literals
    return new ast::node::literal_expr_t(tok->e_tok_type, tok->t_val);
  }
  }
}

/**
 * Parse primary expressions (atoms and prefix operators)
 */
ast::node::expression_t *
parse_primary(std::vector<token_t *> &tokens, ParseContext context,
              symbol_table::symbol_table_t *p_symbol_table) {
  if (tokens.empty()) {
    throw exceptions::parser_error("Unexpected end of expression");
  }

  token_t *tok = peek(tokens);

  // Handle parenthesized expressions: (expr)
  if (tok->e_tok_type == tok_lparen) {
    consume(tokens); // consume '('
    ast::node::expression_t *expr =
        parse_expression(tokens, p_symbol_table, 0, context);
    // match(tokens, tok_rparen); // consume and verify ')'
    return expr;
  }

  // Handle prefix unary operators: !, -, +
  if (tok->e_tok_type == tok_exclaimationmark || tok->e_tok_type == tok_minus ||
      tok->e_tok_type == tok_plus) {
    token_t *unary_op = consume(tokens);
    // Use high precedence for unary operators (they bind tightly)
    ast::node::expression_t *operand =
        parse_expression(tokens, p_symbol_table, 60, context);
    return create_unary_expression(unary_op, operand, context);
  }

  // Handle literals and identifiers
  switch (tok->e_tok_type) {
  case tok_number:
  case tok_string:
  case tok_char_literal:
    return create_literal_expression(consume(tokens), context);

  case tok_id: {
    token_t *id_tok = consume(tokens);
    if (!p_symbol_table->is_defined(id_tok->t_val)) {
      throw exceptions::variable_not_declared_error("identifier is not defined",
                                                    id_tok);
    }

    // Check if it's a function call: identifier(args)
    if (tokens.size() > 1 && peek(tokens)->e_tok_type == tok_lparen) {
      consume(tokens); // consume '('

      std::vector<ast::node::expression_t *> arguments;

      // Parse arguments
      while (!tokens.empty() && peek(tokens)->e_tok_type != tok_rparen) {
        std::cout << debug_tok(tokens.back()) << std::endl;
        arguments.push_back(
            parse_expression(tokens, p_symbol_table, 0, context));
        std::cout << debug_tok(tokens.back()) << std::endl;

        // Check for comma (more arguments)
        if (!tokens.empty() && peek(tokens)->e_tok_type == tok_comma) {
          consume(tokens); // consume ','
        }
      }

      match(tokens, tok_rparen); // consume ')'

      // check for ; (end of expression)
      if (!tokens.empty() && peek(tokens)->e_tok_type == tok_semicolon) {
        consume(tokens); // consume ';'
      }

      return new ast::node::call_expr_t(id_tok->t_val, arguments);
    }

    // Just an identifier
    return create_literal_expression(id_tok, context);
  }
  case tok_eof:
  case tok_comment:
  case tok_bool:
  case tok_char:
  case tok_int:
  case tok_float:
  case tok_void:
  case tok_if:
  case tok_else:
  case tok_while:
  case tok_for:
  case tok_return:
  case tok_plus:
  case tok_minus:
  case tok_star:
  case tok_slash:
  case tok_assign:
  case tok_lt:
  case tok_leq:
  case tok_gt:
  case tok_geq:
  case tok_eq:
  case tok_neq:
  case tok_exclaimationmark:
  case tok_lparen:
  case tok_rparen:
  case tok_lbrace:
  case tok_rbrace:
  case tok_lbracket:
  case tok_rbracket:
  case tok_comma:
  case tok_semicolon:
  case tok_colon:
    break;
  }
  throw exceptions::parser_error("Expected primary expression", tok);
}

/**
 * Main expression parser using Pratt parsing
 * Assumes token vector is REVERSED (last element is first token)
 */
ast::node::expression_t *
parse_expression(std::vector<token_t *> &tokens,
                 symbol_table::symbol_table_t *p_symbol_table,
                 int last_op_precedence, ParseContext context) {
  // Parse the leftmost operand (primary expression)
  ast::node::expression_t *lhs = parse_primary(tokens, context, p_symbol_table);

  // Process binary operators with precedence climbing
  while (!tokens.empty()) {
    // Peek at the next token
    token_t *next_tok = peek(tokens);

    // Stop if we hit an expression delimiter or EOF
    if (is_expression_delimiter(next_tok->e_tok_type) ||
        next_tok->e_tok_type == tok_eof) {
      if (peek(tokens, 2)->e_tok_type == tok_semicolon) {
        break;
      }
      consume(tokens);
      break;
    }

    // Get the operator's precedence
    int op_prec = get_precedence(next_tok->e_tok_type);

    // If not an operator or precedence is too low, stop
    if (op_prec < 0 || op_prec < last_op_precedence) {
      break;
    }

    // Handle array subscript: arr[index]
    if (next_tok->e_tok_type == tok_lbracket) {
      consume(tokens); // consume '['
      ast::node::expression_t *index =
          parse_expression(tokens, p_symbol_table, 0, context);
      match(tokens, tok_rbracket); // consume ']'
      lhs = new ast::node::subscript_expr_t(lhs, index);
      continue;
    }

    // Consume the binary operator
    token_t *op = consume(tokens);

    // Parse the right-hand side
    // Use op_prec + 1 for left-associative operators
    // (ensures operators of same precedence associate left-to-right)
    ast::node::expression_t *rhs =
        parse_expression(tokens, p_symbol_table, op_prec + 1, context);

    // Create binary expression and make it the new lhs
    lhs = create_binary_expression(lhs, op, rhs, context);
  }

  return lhs;
}

} // namespace pratt_parser
} // namespace parser
} // namespace compiler
