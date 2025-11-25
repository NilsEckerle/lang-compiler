#include "lexer/lexer.h"
#include "tokens.h"
#include <cassert>
#include <cstddef>
#include <cstdio>
#include <iostream>
#include <ostream>
#include <regex>
#include <stdexcept>
#include <string>
#include <vector>

namespace lexer {

std::string remainder(std::string str, int pos) { return str.substr(pos); }

bool starts_with_whitespace(std::string str) {
  if (str.length() <= 0) {
    return false;
  }

  switch (str.c_str()[0]) {
  case ' ':
    return true;
  case '\n':
    return true;
  case '\t':
    return true;
  }

  return false;
}

std::vector<token_t *> lexer_t::lex(std::string source) {
  size_t pos = 0;
  std::vector<lexer::token_t *> tokens;

  while (pos < source.length()) {
    std::string rest = remainder(source, pos);

    // remove whitespace
    if (starts_with_whitespace(rest)) {
      pos++;
      continue;
    }

    // parse token
    bool token_found = false;
    for (std::pair<lexer::token_e, std::regex> pair : lexer::token_regex) {
      std::smatch match;
      std::regex reg = pair.second;
      if (std::regex_search(rest, match, reg,
                            std::regex_constants::match_continuous)) {
        tokens.push_back(create_token_t(pair.first, match.str()));
        pos += match.length(0);
        token_found = true;
        break;
      }
    }
    if (!token_found) {
      std::cout << "no token found: " << rest << std::endl;
    }
  }

  tokens.push_back(new token_t(tok_eof, ""));
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

void debug_print(token_t *token) {
  printf("%s (%s)\n", token->type_name().c_str(), token->t_val.c_str());
}

} // namespace lexer
