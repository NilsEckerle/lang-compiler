#ifndef EXCEPTIONS_H
#define EXCEPTIONS_H

// #include "spdlog/fmt/fmt.h"
#include "parser/abstract_syntax_tree.h"
#include "spdlog/fmt/bundled/format.h"
#include "tokens.h"
#include <exception>
#include <string>

namespace compiler {
namespace exceptions {

class syntax_error : public std::exception {
public:
  syntax_error(std::string msg) {
    this->msg = msg;
    this->p_tok = nullptr;
    this->formatted_msg = fmt::format("Syntax error: {}", this->msg);
  }
  syntax_error(std::string msg, token_t *p_tok) {
    this->msg = msg;
    this->p_tok = p_tok;
    this->formatted_msg =
        fmt::format("Syntax error at line:{}: {}: {}", this->p_tok->linenumber,
                    this->p_tok->t_val, this->msg);
  }
  virtual const char *what() const noexcept { return formatted_msg.c_str(); }

protected:
  token_t *p_tok;
  std::string msg;
  std::string formatted_msg;
};

class parser_error : public syntax_error {
public:
  parser_error(std::string msg) : syntax_error(msg) {};
  parser_error(std::string msg, token_t *p_tok) : syntax_error(msg, p_tok) {};
};

class variable_not_declared_error : public syntax_error {
public:
  variable_not_declared_error(std::string msg) : syntax_error(msg) {
    this->formatted_msg = fmt::format("Varable is not declared: {}", this->msg);
  };
  variable_not_declared_error(std::string msg, token_t *p_tok)
      : syntax_error(msg, p_tok) {
    this->formatted_msg =
        fmt::format("Varable is not declared: {}: {}: {}",
                    this->p_tok->linenumber, this->p_tok->t_val, this->msg);
  };
};

class variable_already_declared_error : public syntax_error {
public:
  variable_already_declared_error(std::string msg) : syntax_error(msg) {
    this->formatted_msg = fmt::format("Varable is not declared: {}", this->msg);
  };
  variable_already_declared_error(std::string msg, token_t *p_tok)
      : syntax_error(msg, p_tok) {
    this->formatted_msg =
        fmt::format("Varable is not declared: {}: {}: {}",
                    this->p_tok->linenumber, this->p_tok->t_val, this->msg);
  };
};

} // namespace exceptions
} // namespace compiler
#endif // !EXCEPTIONS_H
