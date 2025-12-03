#ifndef TOKENS_H
#define TOKENS_H

#include <map>
#include <regex>
#include <string>
#include <tuple>
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

/** @brief definition of precidence levels
 *
 * @note Precedence hierarchy (C-style operator precedence):
 *
 * 0-10: Lowest (comma, assignment, ternary)
 * 20-30: Assignment operators
 * 40-49: Comparison/relational
 * 50-59: Addition/subtraction
 * 55-59: Multiplication/division/modulo
 * 60-69: Unary operators
 * 70-79: Array subscript
 * 80-89: Member access (highest binary)
 * 100: Atomic values (literals, identifiers)
 *
 * For right-associative operators (like = and ?:), rbp < lbp ensures
 * right-to-left grouping.*/
static std::map<token_e, std::tuple<int, int>> token_presidence = {
    {tok_eof, std::tuple<int, int>(0, 0)},
    {tok_comment, std::tuple<int, int>(0, 0)},

    // Keywords - Type qualifiers
    {tok_let, std::tuple<int, int>(0, 0)},
    {tok_mut, std::tuple<int, int>(0, 0)},
    {tok_dyn, std::tuple<int, int>(0, 0)},
    {tok_static, std::tuple<int, int>(0, 0)},
    {tok_typedef, std::tuple<int, int>(0, 0)},

    // Keywords - Primitive types
    {tok_bool, std::tuple<int, int>(0, 100)},
    {tok_char, std::tuple<int, int>(0, 100)},
    {tok_short, std::tuple<int, int>(0, 100)},
    {tok_int, std::tuple<int, int>(0, 100)},
    {tok_long_long, std::tuple<int, int>(0, 100)},
    {tok_long, std::tuple<int, int>(0, 100)},
    {tok_float, std::tuple<int, int>(0, 100)},
    {tok_double, std::tuple<int, int>(0, 100)},
    {tok_void, std::tuple<int, int>(0, 100)},

    // Keywords - Complex types
    {tok_enum, std::tuple<int, int>(0, 100)},
    {tok_struct, std::tuple<int, int>(0, 100)},
    {tok_union, std::tuple<int, int>(0, 100)},

    // Keywords - Control flow
    {tok_if, std::tuple<int, int>(0, 100)},
    {tok_else, std::tuple<int, int>(0, 100)},
    {tok_switch, std::tuple<int, int>(0, 100)},
    {tok_case, std::tuple<int, int>(0, 100)},
    {tok_default, std::tuple<int, int>(0, 100)},
    {tok_for, std::tuple<int, int>(0, 100)},
    {tok_while, std::tuple<int, int>(0, 100)},
    {tok_do, std::tuple<int, int>(0, 100)},
    {tok_break, std::tuple<int, int>(100, 0)},
    {tok_continue, std::tuple<int, int>(100, 0)},
    {tok_return, std::tuple<int, int>(100, 100)},

    // Identifiers
    {tok_id, std::tuple<int, int>(0, 100)},

    // Literals
    {tok_number, std::tuple<int, int>(0, 100)},
    {tok_string, std::tuple<int, int>(0, 100)},
    {tok_char_literal, std::tuple<int, int>(0, 100)},

    // Two-character operators
    {tok_leq, std::tuple<int, int>(40, 41)},
    {tok_geq, std::tuple<int, int>(40, 41)},
    {tok_eq, std::tuple<int, int>(40, 41)},
    {tok_neq, std::tuple<int, int>(40, 41)},

    // Operators - Arithmetic
    {tok_plus, std::tuple<int, int>(50, 51)},
    {tok_minus, std::tuple<int, int>(50, 51)},
    {tok_star, std::tuple<int, int>(55, 56)},
    {tok_slash, std::tuple<int, int>(55, 56)},
    {tok_percent, std::tuple<int, int>(55, 56)},

    // Operators - Assignment (right-associative)
    {tok_assign, std::tuple<int, int>(20, 19)},

    // Operators - Comparison
    {tok_lt, std::tuple<int, int>(40, 41)},
    {tok_gt, std::tuple<int, int>(40, 41)},

    // Operators - Logical
    {tok_exclaimationmark, std::tuple<int, int>(0, 60)}, // prefix unary
    {tok_questionmark, std::tuple<int, int>(15, 14)}, // ternary (right-assoc)

    // Delimiters
    {tok_lparen, std::tuple<int, int>(0, 100)},
    {tok_rparen, std::tuple<int, int>(100, 0)},
    {tok_lbrace, std::tuple<int, int>(0, 100)},
    {tok_rbrace, std::tuple<int, int>(100, 0)},
    {tok_lbracket, std::tuple<int, int>(70, 71)}, // array subscript
    {tok_rbracket, std::tuple<int, int>(100, 0)},

    // Delimiters - Punctuation
    {tok_comma, std::tuple<int, int>(10, 11)},
    {tok_semicolon, std::tuple<int, int>(0, 0)},
    {tok_colon, std::tuple<int, int>(5, 6)},
    {tok_dot,
     std::tuple<int, int>(80, 81)}, // member access (highest precedence)
};

int get_left_precidence(token_e tok);

int get_right_precidence(token_e tok);

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
