#include "lexer/lexer.h"
#include "tokens.h"
#include <cassert>
#include <cstddef>
#include <cstdio>
#include <stdexcept>
#include <string>
#include <vector>

namespace lexer {

std::vector<token_t *> lexer_t::lex(std::string source) {
  printf("%s\n", source.c_str());
  for (char c : source) {
    switch (c) {
    case ' ':
      break;
    case '=':
      this->_t_tokens.push_back(new token_t(tok_assign, ""));
      break;
    case '!':
      this->_t_tokens.push_back(new token_t(tok_exclaimationmark, ""));
      break;
    case '?':
      this->_t_tokens.push_back(new token_t(tok_questionmark, ""));
      break;
    case '+':
      this->_t_tokens.push_back(new token_t(tok_plus, ""));
      break;
    case '-':
      this->_t_tokens.push_back(new token_t(tok_minus, ""));
      break;
    case '*':
      this->_t_tokens.push_back(new token_t(tok_star, ""));
      break;
    case '/':
      this->_t_tokens.push_back(new token_t(tok_slash, ""));
      break;
    case '%':
      this->_t_tokens.push_back(new token_t(tok_percent, ""));
      break;
    case '<':
      this->_t_tokens.push_back(new token_t(tok_lt, ""));
      break;
    case '>':
      this->_t_tokens.push_back(new token_t(tok_gt, ""));
      break;
    case '.':
      this->_t_tokens.push_back(new token_t(tok_dot, ""));
      break;
    case ',':
      this->_t_tokens.push_back(new token_t(tok_comma, ""));
      break;
    case ';':
      this->_t_tokens.push_back(new token_t(tok_semicolon, ""));
      break;
    }
  }
  return this->_t_tokens;
}

std::vector<token_t *> lex_file(std::string t_input_file_path) {
  // open file
  FILE *tp_file = fopen(t_input_file_path.c_str(), "rb");
  if (tp_file == NULL) {
    throw std::runtime_error("Failed to open file: " + t_input_file_path);
  }

  // get file size
  fseek(tp_file, 0, SEEK_END);
  long file_size = ftell(tp_file);
  fseek(tp_file, 0, SEEK_SET);

  // allocate buffer and read file
  std::string source;
  source.resize(file_size);
  size_t bytes_read = fread(&source[0], 1, file_size, tp_file);
  fclose(tp_file);

  if (bytes_read != static_cast<size_t>(file_size)) {
    throw std::runtime_error("Failed to read entire file: " +
                             t_input_file_path);
  }

  // lex source
  lexer_t lxr = lexer_t();
  std::vector<token_t *> tokens = lxr.lex(source);
  return tokens;
}

void debug_print(token_t *token) {
  printf("%s (%s)\n", token->type_name().c_str(), token->t_val.c_str());
}

} // namespace lexer
