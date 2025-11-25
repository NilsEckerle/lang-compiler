#include "tokens.h"
#include <string>

namespace lexer {

token_t::token_t(token_e e_tok_type, std::string t_val) {
  this->e_tok_type = e_tok_type;
  this->t_val = t_val;
}

std::string token_t::type_name() {
  switch (this->e_tok_type) {
  case tok_eof:
    return "eof";
  case tok_comment:
    return "comment";
  case tok_id:
    return "id";
  case tok_number:
    return "number";
  case tok_string:
    return "string";
  case tok_let:
    return "let";
  case tok_mut:
    return "mut";
  case tok_dyn:
    return "dyn";
  case tok_short:
    return "short";
  case tok_int:
    return "int";
  case tok_long:
    return "long";
  case tok_long_long:
    return "longlong";
  case tok_float:
    return "float";
  case tok_double:
    return "double";
  case tok_char:
    return "char";
  case tok_bool:
    return "bool";
  case tok_if:
    return "if";
  case tok_else:
    return "else";
  case tok_questionmark:
    return "questionmark";
  case tok_exclaimationmark:
    return "exclaimationmark";
  case tok_do:
    return "do";
  case tok_while:
    return "while";
  case tok_for:
    return "for";
  case tok_return:
    return "return";
  case tok_plus:
    return "plus";
  case tok_minus:
    return "minus";
  case tok_star:
    return "star";
  case tok_slash:
    return "slash";
  case tok_percent:
    return "percent";
  case tok_assign:
    return "assign";
  case tok_eq:
    return "eq";
  case tok_neq:
    return "eq";
  case tok_lt:
    return "lt";
  case tok_leq:
    return "leq";
  case tok_gt:
    return "gt";
  case tok_geq:
    return "geq";
  case tok_lparen:
    return "lparen";
  case tok_rparen:
    return "rparen";
  case tok_lbracket:
    return "lbracket";
  case tok_rbracket:
    return "rbracket";
  case tok_lbrace:
    return "lbrace";
  case tok_rbrace:
    return "rbrace";
  case tok_semicolon:
    return "semicolon";
  case tok_comma:
    return "comma";
  case tok_dot:
    return "dot";
  }
  return "TOKEN NOT YET NAMED!!!";
}

token_t *create_token_t(token_e e_tok_type, std::string t_val) {
  token_t *tok;
  switch (e_tok_type) {
  case tok_id:
  case tok_number:
  case tok_string:
  case tok_comment:
    tok = new token_t(e_tok_type, t_val);
    break;
  default:
    tok = new token_t(e_tok_type, "");
    break;
  }
  return tok;
}

} // namespace lexer
