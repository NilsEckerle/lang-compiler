#include "parser/parser.h"
#include "exceptions.h"
#include "parser/abstract_syntax_tree.h"
#include "parser/pratt_parser.h"
#include "spdlog/fmt/bundled/format.h"
#include "symbol_table.h"
#include "tokens.h"
#include <algorithm>
#include <cassert>
#include <string>
#include <tuple>
#include <vector>

#define DEBUG(msg) std::cout << msg
#define DEBUGLN(msg) std::cout << msg << std::endl
// #define DEBUG(msg) (void)0

namespace compiler::parser {

static symbol_table::symbol_table_t *g_symbol_table = nullptr;

ast::node::block_t *parse_block(std::vector<token_t *> &tokens);

token_t *peek(std::vector<token_t *> &tokens, size_t n = 1) {
  assert(tokens.size() >= n + 1);
  return tokens.at(tokens.size() - n);
}

void match(std::vector<token_t *> &tokens, token_e type) {
  token_t *tok = tokens.back();
  if (tok->e_tok_type != type) {
    throw exceptions::parser_error(
        fmt::format("Token type didn't match to type {}",
                    token_t(type, "", -1).type_name()),
        tok);
  }
  tokens.pop_back();
}

token_e match_type(std::vector<token_t *> &tokens) {
  token_e type;
  // DEBUG(debug_tok(peek(tokens)));

  token_t *tok = peek(tokens);
  type = tok->e_tok_type;

  switch (type) {
  case tok_int:
  case tok_char:
  case tok_void:
  case tok_float:
  case tok_bool:
    break; // type accepted
  case tok_eof:
  case tok_comment:
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
  case tok_id:
  case tok_number:
  case tok_string:
  case tok_char_literal:
    throw exceptions::parser_error("not an valid type", peek(tokens));
  }

  tokens.pop_back();
  return type;
}

ast::node::expression_t *parse_expression(std::vector<token_t *> &tokens) {
  ast::node::expression_t *p_expr =
      pratt_parser::parse_expression(tokens, g_symbol_table);

  return p_expr;
}

ast::node::variable_t *parse_define(std::vector<token_t *> &tokens) {
  ast::node::variable_t *p_var = nullptr;

  token_e type;
  int pl = 0;
  std::string id;
  ast::node::expression_t *expr = nullptr;

  // match type
  type = match_type(tokens);
  // match n stars for pointer_level
  while (tokens.back()->e_tok_type == tok_star) {
    pl++;
    tokens.pop_back();
  }
  // match id
  token_t *tok = tokens.back();
  tokens.pop_back();
  if (tok->e_tok_type != tok_id) {
    throw exceptions::parser_error("Expected identifier", tok);
  }
  id = tok->t_val;
  // if =
  if (peek(tokens)->e_tok_type == tok_assign) {
    // match =
    match(tokens, tok_assign);
    // match expr
    expr = parse_expression(tokens);
  }

  p_var = new ast::node::variable_t(type, pl, id, expr);
  g_symbol_table->add(p_var);

  return p_var;
}

ast::node::assign_expr_t *parse_assign(std::vector<token_t *> &tokens) {
  ast::node::assign_expr_t *assign = nullptr;

  std::string id;
  ast::node::expression_t *expr = nullptr;

  // match id
  token_t *tok = tokens.back();
  tokens.pop_back();
  if (tok->e_tok_type != tok_id) {
    throw exceptions::parser_error("Expected identifier", tok);
  }
  id = tok->t_val;
  if (!g_symbol_table->is_defined(id)) {
    throw exceptions::variable_not_declared_error("variable is not defined",
                                                  tok);
  }
  // if =
  if (peek(tokens)->e_tok_type == tok_assign) {
    // match =
    match(tokens, tok_assign);
    // match expr
    expr = parse_expression(tokens);
  }

  token_e type = g_symbol_table->get(id)->type;
  int pl = g_symbol_table->get(id)->pointer_level;
  (void)type;
  (void)pl;
  // TODO: make sure the id is already in the simbol table

  assign = new ast::node::assign_expr_t(id, expr);

  return assign;
}

ast::node::statement_t *parse_statement(std::vector<token_t *> &tokens) {
  ast::node::statement_t *p_stmt = nullptr;
  (void)tokens;

  token_t *tok = tokens.back();
  tokens.pop_back();

  switch (tok->e_tok_type) {
  case tok_if:
    p_stmt =
        new ast::node::if_t(parse_expression(tokens), parse_statement(tokens));
    break;
  case tok_else:
    p_stmt = new ast::node::else_t(parse_statement(tokens));
    break;
  case tok_while:
    p_stmt = new ast::node::while_t(parse_expression(tokens),
                                    parse_statement(tokens));
    break;
  // case tok_do:
  //   p_stmt =
  //       new ast::node::do_t(parse_expression(tokens),
  //       parse_statement(tokens));
  //   break;
  // case tok_switch:
  // case tok_case:
  case tok_for: {
    // (
    match(tokens, tok_lparen);
    // stmt ;
    ast::node::statement_t *p_first = parse_statement(tokens);
    // match(tokens, tok_semicolon);
    // expr ;
    ast::node::expression_t *p_expr = parse_expression(tokens);
    // stmt)
    ast::node::statement_t *p_last = parse_statement(tokens);
    // stmt
    ast::node::block_t *p_block = parse_block(tokens);

    p_stmt = new ast::node::for_t(p_first, p_expr, p_last, p_block);
    break;
  }
  case tok_return:
    p_stmt = new ast::node::return_t(parse_expression(tokens));
    break;
  case tok_id:
    tokens.push_back(tok);
    p_stmt = parse_assign(tokens);
    break;
  case tok_int:
  case tok_float:
  case tok_char:
  case tok_void:
  case tok_bool:
    tokens.push_back(tok);
    p_stmt = parse_define(tokens);
    break;
  case tok_eof:
  case tok_comment:
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
  case tok_number:
  case tok_string:
  case tok_char_literal:
    throw compiler::exceptions::syntax_error("No statement found", tok);
  }

  return p_stmt;
}

ast::node::block_t *parse_block(std::vector<token_t *> &tokens) {
  ast::node::block_t *p_block = new ast::node::block_t();
  g_symbol_table = new symbol_table::symbol_table_t(g_symbol_table);

  // match {
  match(tokens, tok_lbrace);
  // match stmts
  while (tokens.back()->e_tok_type != tok_rbrace) {
    ast::node::statement_t *stmt = parse_statement(tokens);
    p_block->statements.push_back(stmt);
  }
  // match }
  match(tokens, tok_rbrace);

  // restore symbol_table
  symbol_table::symbol_table_t *function_symbol_table = g_symbol_table;
  g_symbol_table = g_symbol_table->p_previous;
  free(function_symbol_table);

  return p_block;
}

ast::node::function_t *parse_function(std::vector<token_t *> &tokens) {
  g_symbol_table = new symbol_table::symbol_table_t(g_symbol_table);
  ast::node::function_t *p_func = nullptr;

  token_e type;
  int pointer_level = 0;
  std::string id;
  std::vector<std::tuple<token_e, int>> *parameter_type_pointer_level_tuple =
      new std::vector<std::tuple<token_e, int>>;
  ast::node::block_t *block = nullptr;

  // match type
  type = match_type(tokens);

  // match pl
  while (tokens.back()->e_tok_type == tok_star) {
    pointer_level++;
    tokens.pop_back();
  }

  // match id
  token_t *tok = tokens.back();
  if (tok->e_tok_type != tok_id) {
    throw exceptions::parser_error("Expected identifier", tok);
  }
  id = tok->t_val;
  match(tokens, tok_id);

  // match (
  match(tokens, tok_lparen);

  // match defines
  tok = tokens.back();
  while (tok->e_tok_type != tok_rparen) {
    ast::node::variable_t *param = parse_define(tokens);
    // TODO: add has_default_value
    parameter_type_pointer_level_tuple->push_back(
        std::tuple<token_e, int>{param->type, param->pointer_level});

    if (peek(tokens)->e_tok_type == tok_comma) {
      tokens.pop_back();
    }
    tok = tokens.back();
  }

  // match )
  match(tokens, tok_rparen);

  // add func to old symbol_table so it is accassible outside
  // add func here to symbol_table so it is accessible inside the block
  p_func = new ast::node::function_t(
      type, pointer_level, id, parameter_type_pointer_level_tuple, nullptr);
  g_symbol_table->p_previous->add(p_func);

  // match block
  block = parse_block(tokens);

  // build full func
  p_func->block = block;

  // restore symbol_table
  symbol_table::symbol_table_t *function_symbol_table = g_symbol_table;
  g_symbol_table = g_symbol_table->p_previous;
  free(function_symbol_table);

  return p_func;
}

ast::node::node_t *parse_program(std::vector<token_t *> &tokens) {
  if (tokens.size() == 1 && tokens.back()->e_tok_type == tok_eof) {
    return nullptr;
  }

  ast::node::node_t *p_node = nullptr;

  token_t *tok = peek(tokens);
  switch (tok->e_tok_type) {
  // type
  // case tok_short:
  case tok_int:
  // case tok_long:
  // case tok_long_long:
  case tok_float:
  // case tok_double:
  case tok_bool:
  case tok_void:
  case tok_char:
    if (peek(tokens, 2)->e_tok_type != tok_id) {
      throw exceptions::parser_error("missing identifier after", tok);
    }
    if (peek(tokens, 3)->e_tok_type == tok_lparen) {
      p_node = parse_function(tokens);
      g_symbol_table->add(p_node);
    } else {
      p_node = parse_define(tokens);
    }
    break;
  case tok_eof:
  case tok_comment:
  case tok_if:
  case tok_else:
  case tok_for:
  case tok_while:
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
  case tok_id:
  case tok_number:
  case tok_string:
  case tok_char_literal:
    throw compiler::exceptions::syntax_error("No statement found", tok);
  }

  return p_node;
}

std::vector<ast::abstract_syntax_tree_t *> *
parse_tokens(std::vector<token_t *> &tokens) {
  assert(tokens.size() > 0);

  std::vector<ast::abstract_syntax_tree_t *> *asts =
      new std::vector<ast::abstract_syntax_tree_t *>();

  int i = 0;
  size_t old_token_len = tokens.size();
  std::reverse(tokens.begin(), tokens.end());
  g_symbol_table = new symbol_table::symbol_table_t();

  while (tokens.size() > 1 && i < 100) {
    ast::abstract_syntax_tree_t *tree = new ast::abstract_syntax_tree_t();

    tree->p_head = parse_program(tokens);
    asts->push_back(tree);

    if (old_token_len == tokens.size()) {
      i++;
    } else {
      old_token_len = tokens.size();
      i = 0;
    }
  }

  if (i > 1) {
    if (tokens.size() > 0) {
      throw exceptions::parser_error("parser runns infinitly long",
                                     tokens.back());
    } else {
      throw exceptions::parser_error(
          "parser runns infinitly long without a token");
    }
  }

  free(g_symbol_table);
  g_symbol_table = nullptr;

  return asts;
}

} // namespace compiler::parser
