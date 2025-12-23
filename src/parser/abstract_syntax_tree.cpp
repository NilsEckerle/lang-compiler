#include "parser/abstract_syntax_tree.h"
#include "tokens.h"
#include <fmt/format.h>
#include <string>

// Helper function to convert variant to string
std::string variant_to_string(
    const std::variant<int, float, double, char, char *, std::string> &v) {
  return std::visit(
      [](auto &&arg) -> std::string {
        using T = std::decay_t<decltype(arg)>;
        if constexpr (std::is_same_v<T, std::string>) {
          return arg;
        } else if constexpr (std::is_same_v<T, char *>) {
          return std::string(arg);
        } else if constexpr (std::is_same_v<T, char>) {
          return std::string(1, arg);
        } else {
          return std::to_string(arg);
        }
      },
      v);
}

namespace compiler::parser::ast {

std::string abstract_syntax_tree_t::debug_print() {
  return this->p_head->debug_print();
}

namespace node {

if_t::if_t(expression_t *p_expr, statement_t *p_stmt) {
  this->p_expr = p_expr;
  this->p_stmt = p_stmt;
}
if_t::~if_t() {
  free(this->p_expr);
  free(this->p_stmt);
}
std::string if_t::debug_print() const {
  return fmt::format("if({0}, {1})", this->p_expr->debug_print(),
                     this->p_stmt->debug_print());
}

else_t::else_t(statement_t *p_stmt) { this->p_stmt = p_stmt; }
else_t::~else_t() { free(this->p_stmt); }
std::string else_t::debug_print() const {
  return fmt::format("else({0})", this->p_stmt->debug_print());
}

while_t::while_t(expression_t *p_expr, statement_t *p_stmt) {
  this->p_expr = p_expr;
  this->p_stmt = p_stmt;
}
while_t::~while_t() {
  free(this->p_expr);
  free(this->p_stmt);
}
std::string while_t::debug_print() const {
  return fmt::format("while({0}, {1})", this->p_expr->debug_print(),
                     this->p_stmt->debug_print());
}

do_t::do_t(expression_t *p_expr, statement_t *p_stmt) {
  this->p_expr = p_expr;
  this->p_stmt = p_stmt;
}
do_t::~do_t() {
  free(this->p_expr);
  free(this->p_stmt);
}
std::string do_t::debug_print() const {
  return fmt::format("do({1}, {0};", this->p_expr->debug_print(),
                     this->p_stmt->debug_print());
}

return_t::return_t(expression_t *p_expr) { this->p_expr = p_expr; }
return_t::~return_t() { free(this->p_expr); }
std::string return_t::debug_print() const {
  return fmt::format("return({0})", this->p_expr->debug_print());
}

function_t::function_t(
    token_e type, int pointer_level, std::string identifier,
    std::vector<std::tuple<token_e, int>> *parameter_type_pointer_level_tuple,
    block_t *block) {
  this->type = type;
  this->pointer_level = pointer_level;
  this->identifier = identifier;
  this->parameter_type_pointer_level_tuple = parameter_type_pointer_level_tuple;
  this->block = block;
}
function_t::~function_t() {
  free(this->parameter_type_pointer_level_tuple);
  free(this->block);
}
std::string function_t::debug_print() const {
  return fmt::format("function({}, {}, {}, {})",
                     create_token_t(this->type, "", -1)->type_name(),
                     this->pointer_level, this->identifier,
                     this->block->debug_print());
}

variable_t::variable_t(token_e type, int pointer_level, std::string id,
                       expression_t *expr) {
  this->type = type;
  this->pointer_level = pointer_level;
  this->identifier = id;
  this->p_expr = expr;
}
variable_t::~variable_t() { free(this->p_expr); }
std::string variable_t::debug_print() const {
  return fmt::format("variable({}, {}, {}, {})",
                     create_token_t(this->type, "", -1)->type_name(),
                     this->pointer_level, this->identifier,
                     this->p_expr->debug_print());
}

std::string binary_expr_t::to_prefix_notation() const {
  std::string s;

  s = fmt::format(
      "({} {} {})", compiler::create_token_t(this->op, "", -1)->type_name(),
      this->left->to_prefix_notation(), this->right->to_prefix_notation());

  return s;
}
std::string binary_expr_t::debug_print() const {
  return fmt::format("binary_expr({})", this->to_prefix_notation());
}

std::string unary_expr_t::to_prefix_notation() const {
  std::string s;

  s = fmt::format("({} {})",
                  compiler::create_token_t(this->op, "", -1)->type_name(),
                  this->operand->to_prefix_notation());

  return s;
}
std::string unary_expr_t::debug_print() const {
  return fmt::format("unary_expr({})", this->to_prefix_notation());
}

std::string literal_expr_t::to_prefix_notation() const {
  std::string s;

  s = fmt::format("({} {})", create_token_t(this->type, "", -1)->type_name(),
                  variant_to_string(this->value));

  return s;
}
std::string literal_expr_t::debug_print() const {
  return fmt::format("literal_expr({})", this->to_prefix_notation());
}

std::string call_expr_t::to_prefix_notation() const {
  std::string s;

  s = fmt::format("(call {})", this->function_name);

  return s;
}
std::string call_expr_t::debug_print() const {
  return fmt::format("call_expr({})", this->to_prefix_notation());
}

std::string subscript_expr_t::to_prefix_notation() const {
  std::string s;

  s = fmt::format("({}[{}])", this->array->to_prefix_notation(),
                  this->index->to_prefix_notation());

  return s;
}
std::string subscript_expr_t::debug_print() const {
  return fmt::format("subscript_expr({})", this->to_prefix_notation());
}

std::string block_t::debug_print() const {
  std::string s = "";
  bool first = true;
  for (statement_t *stmt : this->statements) {
    if (first) {
      s.append(fmt::format("statement({})", stmt->debug_print()));
    } else {
      s.append(fmt::format(", statement({})", stmt->debug_print()));
    }
    first = false;
  }
  return s;
}

} // namespace node

} // namespace compiler::parser::ast
