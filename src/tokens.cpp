#include "tokens.h"
#include <string>

namespace lexer {

token_t::token_t(token_e e_tok_type, std::string t_val) {
  this->e_tok_type = e_tok_type;
  this->t_val = t_val;
}

} // namespace lexer
