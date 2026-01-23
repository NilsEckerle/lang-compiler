#ifndef VISITOR_H
#define VISITOR_H

class ast_visitor_t {
public:
  virtual void handle_node(compiler::parser::ast::node::node_t node) = 0;
};

#endif // !VISITOR_H
