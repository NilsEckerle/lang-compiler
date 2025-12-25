#include "lexer/lexer.h"
#include "tokens.h"
#include <cassert>
#include <cstddef>
#include <cstdio>
#include <iostream>
#include <ostream>
#include <regex>
// #include <spdlog/fmt/fmt.h>
#include <stdexcept>
#include <string>
#include <vector>

namespace compiler::lexer {

std::string remainder(std::string str, int pos) { return str.substr(pos); }

bool starts_with_whitespace(std::string str, int *current_linenumber) {
  if (str.length() <= 0) {
    return false;
  }

  switch (str.c_str()[0]) {
  case ' ':
    return true;
  case '\n':
    (*current_linenumber)++;
    return true;
  case '\t':
    return true;
  default:
    break;
  }

  return false;
}

lexer_t::lexer_t() { this->current_linenumber = 1; }

int lexer::lexer_t::get_linenumber() { return this->current_linenumber; }

std::vector<token_t *> lexer_t::lex(std::string source) {
  size_t pos = 0;
  std::vector<token_t *> tokens;

  while (pos < source.length()) {
    std::string rest = remainder(source, pos);

    // remove whitespace
    if (starts_with_whitespace(rest, &(this->current_linenumber))) {
      pos++;
      continue;
    }

    // parse token
    bool token_found = false;
    for (std::pair<token_e, std::regex> pair : token_regex) {
      std::smatch match;
      std::regex reg = pair.second;
      if (std::regex_search(rest, match, reg,
                            std::regex_constants::match_continuous)) {
        if (pair.first != tok_comment) {
          tokens.push_back(
              create_token_t(pair.first, match.str(), this->get_linenumber()));
        }
        pos += match.length(0);
        token_found = true;
        break;
      }
    }
    if (!token_found) {
      std::cout << "no token found: " << rest << std::endl;
    }
  }

  tokens.push_back(new token_t(tok_eof, "", this->get_linenumber()));
  return tokens;
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

} // namespace compiler::lexer
