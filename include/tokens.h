#ifndef TOKENS_H
#define TOKENS_H

#include <map>
#include <regex>
#include <string>
namespace lexer {

// order is inportant for regex matching
enum token_e {
  // Special tokens
  tok_eof,
  tok_comment,

  // Keywords - Type qualifiers
  tok_let,
  tok_mut,
  tok_dyn,
  tok_static,
  tok_typedef,

  // Keywords - Primitive types
  tok_bool,
  tok_char,
  tok_short,
  tok_int,
  tok_long,
  tok_long_long,
  tok_float,
  tok_double,
  tok_void,

  // Keywords - Complex types
  tok_enum,
  tok_struct,
  tok_union,

  // Keywords - Control flow
  tok_if,
  tok_else,
  tok_switch,
  tok_case,
  tok_default,
  tok_for,
  tok_while,
  tok_do,
  tok_break,
  tok_continue,
  tok_return,

  // Operators - Arithmetic
  tok_plus,
  tok_minus,
  tok_star,
  tok_slash,
  tok_percent,

  // Operators - Assignment
  tok_assign,

  // Operators - Comparison
  tok_lt,
  tok_leq,
  tok_gt,
  tok_geq,
  tok_eq,
  tok_neq,

  // Operators - Logical
  tok_exclaimationmark,
  tok_questionmark,

  // Delimiters - Parentheses and brackets
  tok_lparen,
  tok_rparen,
  tok_lbrace,
  tok_rbrace,
  tok_lbracket,
  tok_rbracket,

  // Delimiters - Punctuation
  tok_comma,
  tok_semicolon,
  tok_colon,
  tok_dot,

  // Literals and identifiers
  tok_id,
  tok_number,
  tok_string,
  tok_char_literal
};

static std::map<token_e, std::regex> token_regex = {
    {tok_eof, std::regex("(?!)")},
    {tok_comment, std::regex("//[^\r\n]*")},

    // Keywords - Type qualifiers (with word boundaries)
    {tok_let, std::regex("\\blet\\b")},
    {tok_mut, std::regex("\\bmut\\b")},
    {tok_dyn, std::regex("\\bdyn\\b")},
    {tok_static, std::regex("\\bstatic\\b")},
    {tok_typedef, std::regex("\\btypedef\\b")},

    // Keywords - Primitive types
    {tok_bool, std::regex("\\bbool\\b")},
    {tok_char, std::regex("\\bchar\\b")},
    {tok_short, std::regex("\\bshort\\b")},
    {tok_int, std::regex("\\bint\\b")},
    {tok_long_long,
     std::regex("\\blong\\s+long\\b")}, // Must come before tok_long
    {tok_long, std::regex("\\blong\\b")},
    {tok_float, std::regex("\\bfloat\\b")},
    {tok_double, std::regex("\\bdouble\\b")},
    {tok_void, std::regex("\\bvoid\\b")},

    // Keywords - Complex types
    {tok_enum, std::regex("\\benum\\b")},
    {tok_struct, std::regex("\\bstruct\\b")},
    {tok_union, std::regex("\\bunion\\b")},

    // Keywords - Control flow
    {tok_if, std::regex("\\bif\\b")},
    {tok_else, std::regex("\\belse\\b")},
    {tok_switch, std::regex("\\bswitch\\b")},
    {tok_case, std::regex("\\bcase\\b")},
    {tok_default, std::regex("\\bdefault\\b")},
    {tok_for, std::regex("\\bfor\\b")},
    {tok_while, std::regex("\\bwhile\\b")},
    {tok_do, std::regex("\\bdo\\b")},
    {tok_break, std::regex("\\bbreak\\b")},
    {tok_continue, std::regex("\\bcontinue\\b")},
    {tok_return, std::regex("\\breturn\\b")},

    // Identifiers (must come after all keywords)
    {tok_id, std::regex("[a-zA-Z_][a-zA-Z_0-9]*")},

    // Literals
    {tok_number, std::regex("\\d+\\.\\d+|\\.\\d+|\\d+")},
    {tok_string, std::regex("\".*?\"")},
    {tok_char_literal, std::regex("\'.\'")},

    // Two-character operators (must come before single-character versions)
    {tok_leq, std::regex("<=")},
    {tok_geq, std::regex(">=")},
    {tok_eq, std::regex("==")},
    {tok_neq, std::regex("!=")},

    // Operators - Arithmetic
    {tok_plus, std::regex("\\+")},
    {tok_minus, std::regex("\\-")},
    {tok_star, std::regex("\\*")},
    {tok_slash, std::regex("/")},
    {tok_percent, std::regex("%")},

    // Operators - Assignment
    {tok_assign, std::regex("=")},

    // Operators - Comparison (single-character, after two-character)
    {tok_lt, std::regex("<")},
    {tok_gt, std::regex(">")},

    // Operators - Logical
    {tok_exclaimationmark, std::regex("!")},
    {tok_questionmark, std::regex("\\?")},

    // Delimiters - Parentheses and brackets
    {tok_lparen, std::regex("\\(")},
    {tok_rparen, std::regex("\\)")},
    {tok_lbrace, std::regex("\\{")},
    {tok_rbrace, std::regex("\\}")},
    {tok_lbracket, std::regex("\\[")},
    {tok_rbracket, std::regex("\\]")},

    // Delimiters - Punctuation
    {tok_comma, std::regex(",")},
    {tok_semicolon, std::regex(";")},
    {tok_colon, std::regex(":")},
    {tok_dot, std::regex("\\.")},
};

class token_t {
public:
  token_t(token_e e_tok_type, std::string tp_val, int linenumber);
  ~token_t() = default;
  std::string type_name();

public:
  token_e e_tok_type;
  std::string t_val;
  int linenumber;
};

token_t *create_token_t(token_e e_tok_type, std::string t_val, int linenumber);

} // namespace lexer

#endif /* end of include guard: TOKENS_H */
