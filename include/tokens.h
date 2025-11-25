#ifndef TOKENS_H
#define TOKENS_H

#include <map>
#include <regex>
#include <string>
namespace lexer {

// order is inportant for regex matching
enum token_e {
  // special
  tok_eof,
  tok_comment,

  // typing
  tok_let,
  tok_mut,
  tok_dyn,

  // types
  tok_short,
  tok_int,
  tok_long,
  tok_long_long,
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
  tok_leq,
  tok_gt,
  tok_geq,
  tok_eq,
  tok_neq,

  // delimiters
  tok_lparen,
  tok_rparen,
  tok_lbracket,
  tok_rbracket,
  tok_lbrace,
  tok_rbrace,

  tok_semicolon,
  tok_comma,
  tok_dot,

  // literals
  tok_id,
  tok_number,
  tok_string
};

static std::map<token_e, std::regex> token_regex = {
    {tok_eof, std::regex("(?!)")},
    {tok_comment, std::regex("//[^\r\n]*")},

    // Keywords (with word boundaries to avoid matching identifiers)
    {tok_return, std::regex("\\breturn\\b")},
    {tok_let, std::regex("\\blet\\b")},
    {tok_mut, std::regex("\\bmut\\b")},
    {tok_dyn, std::regex("\\bdyn\\b")},
    {tok_short, std::regex("\\bshort\\b")},
    {tok_int, std::regex("\\bint\\b")},
    {tok_long, std::regex("\\blong\\b")},
    {tok_long_long, std::regex("\\blong\\s+long\\b")}, // handles "long long"
    {tok_float, std::regex("\\bfloat\\b")},
    {tok_double, std::regex("\\bdouble\\b")},
    {tok_char, std::regex("\\bchar\\b")},
    {tok_bool, std::regex("\\bbool\\b")},
    {tok_if, std::regex("\\bif\\b")},
    {tok_else, std::regex("\\belse\\b")},
    {tok_do, std::regex("\\bdo\\b")},
    {tok_while, std::regex("\\bwhile\\b")},
    {tok_for, std::regex("\\bfor\\b")},

    // Identifiers (check after keywords)
    {tok_id, std::regex("[a-zA-Z_][a-zA-Z_0-9]*")},

    // Numbers (fixed to not match empty strings)
    {tok_number, std::regex("\\d+\\.\\d+|\\.\\d+|\\d+")},

    // Strings
    {tok_string, std::regex("\".*?\"")},

    // Two-character operators (check before single-char versions)
    {tok_leq, std::regex("<=")},
    {tok_geq, std::regex(">=")},
    {tok_eq, std::regex("==")},
    {tok_neq, std::regex("!=")},

    // Single-character operators
    {tok_plus, std::regex("\\+")},
    {tok_minus, std::regex("\\-")},
    {tok_star, std::regex("\\*")},
    {tok_slash, std::regex("/")},
    {tok_percent, std::regex("%")},
    {tok_assign, std::regex("=")},
    {tok_questionmark, std::regex("\\?")},
    {tok_exclaimationmark, std::regex("!")},
    {tok_lt, std::regex("<")},
    {tok_gt, std::regex(">")},

    // Delimiters
    {tok_lparen, std::regex("\\(")},
    {tok_rparen, std::regex("\\)")},
    {tok_lbracket, std::regex("\\[")},
    {tok_rbracket, std::regex("\\]")},
    {tok_lbrace, std::regex("\\{")},
    {tok_rbrace, std::regex("\\}")},
    {tok_semicolon, std::regex(";")},
    {tok_comma, std::regex(",")},
    {tok_dot, std::regex("\\.")},
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

token_t *create_token_t(token_e e_tok_type, std::string t_val);

} // namespace lexer

#endif /* end of include guard: TOKENS_H */
