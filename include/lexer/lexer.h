#ifndef LEXER_H
#define LEXER_H

#include "tokens.h"
#include <string>
#include <vector>

namespace lexer {

class lexer_t {
public:
  lexer_t() = default;
  std::vector<token_t *> lex(std::string source);

private:
  std::vector<token_t *> _t_tokens;
  std::string _t_source;
  int _position = 0;
};

std::vector<token_t *> lex_file(std::string t_input_file_path);

void debug_print(token_t *token);

} // namespace lexer

#endif /* end of include guard: LEXER_H */
