#ifndef SYNTAX_TREE_H
#define SYNTAX_TREE_H

#include <string>

namespace parser {

class syntax_tree_t {
public:
  std::string to_prefix_notation();
};

} // namespace parser

#endif // !SYNTAX_TREE_H
