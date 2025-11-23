#ifndef LEXER_H
#define LEXER_H

#include "tokens.h"
#include <optional>
#include <string>
#include <vector>

namespace lexer {

#define I_BUFFER_SIZE 4096 /// 4KB chunks buffer size for reading files

/**
 * @brief convert file to token
 * @param std::string t_input_file_path - file that gets converted
 * @return vector of tokens
 */
std::vector<token_t *> lex_file(std::string t_input_file_path);

enum lexer_state_e {
  lex_start,
  lex_undefined,
};

class lexer_t {
public:
  lexer_t() { this->e_state = lex_start; }
  std::optional<token_t *> lex(char t_c);

public:
  lexer_state_e e_state;
};

} // namespace lexer

#endif /* end of include guard: LEXER_H */
