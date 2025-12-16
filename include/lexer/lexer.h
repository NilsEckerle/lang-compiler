#ifndef LEXER_H
#define LEXER_H

#include "tokens.h"
#include <string>
#include <vector>

namespace compiler {
namespace lexer {

class lexer_t {
public:
  lexer_t();
  std::vector<token_t *> lex(std::string source);
  int get_linenumber();

private:
  std::vector<token_t *> _t_tokens;
  std::string _t_source;
  // int _position = 0;
  int current_linenumber;
};

std::vector<token_t *> lex_file(std::string t_input_file_path);

} // namespace lexer
} // namespace compiler

#endif /* end of include guard: LEXER_H */
