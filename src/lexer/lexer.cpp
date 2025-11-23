#include "lexer/lexer.h"
#include "tokens.h"
#include <cassert>
#include <cstddef>
#include <cstdio>
#include <optional>
#include <stdexcept>
#include <string>
#include <vector>

namespace lexer {

std::optional<token_t *> lexer_t::lex(char t_c) {
  switch (this->e_state) {
  case lex_undefined:
    throw std::runtime_error("Lexer run into state undefined");
  default:
    return std::nullopt;
  }
}

std::vector<token_t *> lex_file(std::string t_input_file_path) {
  FILE *tp_file = fopen(t_input_file_path.c_str(), "rb");
  if (tp_file == NULL) {
    throw std::runtime_error("Failed to open file: " + t_input_file_path);
  }

  char ta_buffer[I_BUFFER_SIZE];
  size_t t_bytes_read;
  lexer_t lxr = lexer_t();
  std::vector<token_t *> tokens;

  // read full file in chunks
  while ((t_bytes_read = fread(ta_buffer, 1, I_BUFFER_SIZE, tp_file)) > 0) {
    for (size_t i = 0; i < t_bytes_read; ++i) { // for each char in chunk
      std::optional<token_t *> tok = lxr.lex(ta_buffer[i]); // convert to token
      if (tok.has_value()) {
        tokens.push_back(tok.value()); // add token
      }
    }
  }

  fclose(tp_file);
  return tokens;
}

} // namespace lexer
