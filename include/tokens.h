#ifndef TOKENS_H
#define TOKENS_H

#include <string>
namespace lexer {

enum token_e {
  // special
  tok_eof,

  // literals
  tok_id,
  tok_number,
  tok_string,

  // typing
  tok_let,
  tok_mut,
  tok_dyn,

  // types
  tok_short,
  tok_int,
  tok_long,
  tok_float,
  tok_double,
  tok_char,
  tok_bool,

  // code flow
  tok_if,
  tok_else,
  tok_do,
  tok_while,
  tok_for,
  tok_return,

  // math
  tok_plus,
  tok_minus,
  tok_star,
  tok_slash,
  tok_percent,

  tok_assign,
  // logic
  tok_questionmark,
  tok_exclaimationmark,
  tok_lt,
  tok_gt,

  // delimiters
  tok_lparen,
  tok_rparen,
  tok_lbracket,
  tok_rbracket,
  tok_lbrace,
  tok_rbrace,

  tok_semicolon,
  tok_comma,
  tok_dot
};

class token_t {
public:
  token_t(token_e e_tok_type, std::string tp_val);
  ~token_t() = default;
  std::string type_name();

public:
  token_e e_tok_type;
  std::string t_val;
};

} // namespace lexer

#endif /* end of include guard: TOKENS_H */
