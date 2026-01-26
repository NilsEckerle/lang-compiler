/**
 * @file lexer.h
 * @brief Lexical analyzer for tokenizing source code
 */

#ifndef LEXER_H
#define LEXER_H

#include "tokens.h"
#include <string>
#include <vector>

namespace compiler {
namespace lexer {

/**
 * @class lexer_t
 * @brief Performs lexical analysis on source code strings
 *
 * Tokenizes source code by scanning through the input and identifying
 * lexical tokens such as keywords, operators, identifiers, and literals.
 */
class lexer_t {
public:
  /**
   * @brief Constructs a new lexer instance
   */
  lexer_t();

  /**
   * @brief Tokenizes the provided source code string
   * @param source The source code to tokenize
   * @return A vector of tokens extracted from the source
   */
  std::vector<token_t *> lex(std::string source);

  /**
   * @brief Gets the current line number during lexical analysis
   * @return The current line number being processed
   */
  int get_linenumber();

private:
  std::vector<token_t *> _t_tokens;
  std::string _t_source;
  int current_linenumber;
};

/**
 * @brief Reads and tokenizes a source file
 * @param t_input_file_path Path to the source file to lex
 * @return A vector of tokens extracted from the file
 * @throws std::runtime_error if the file cannot be opened or read
 */
std::vector<token_t *> lex_file(std::string t_input_file_path);

} // namespace lexer
} // namespace compiler

#endif /* end of include guard: LEXER_H */
