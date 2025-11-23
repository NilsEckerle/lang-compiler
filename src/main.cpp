#include "lexer/lexer.h"
#include "tokens.h"
#include <iostream>
#include <vector>

int main() {
  std::vector<lexer::token_t *> tokens =
      lexer::lex_file("examples/expressions.lang");

  for (lexer::token_t *tok : tokens) {
    lexer::debug_print(tok);
  }
  std::cout << std::endl;

  return 0;
}
