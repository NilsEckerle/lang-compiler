#include "tokens.h"
#include <map>
#include <tuple>

namespace parser {
  namespace pratt_parser {
    static std::map<lexer::token_e, std::tuple<int, int>> presidence_map = {
      {lexer::tok_assign, std::tuple<int, int>{0, 0}},
      // math
      {lexer::tok_plus, std::tuple<int, int>{50, 51}},
      {lexer::tok_minus, std::tuple<int, int>{50, 51}},
      {lexer::tok_star, std::tuple<int, int>{60, 61}},
      {lexer::tok_slash, std::tuple<int, int>{60, 61}},
      {lexer::tok_percent, std::tuple<int, int>{60, 61}},
    };
  }
}
