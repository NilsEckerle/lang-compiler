#include "symbol_table.h"
#include "exceptions.h"
#include "parser/abstract_syntax_tree.h"
#include "tokens.h"
#include <gtest/gtest.h>
#include <tuple>
#include <vector>

using namespace compiler;

class symbol_table_unit_test : public ::testing::Test {
public:
  symbol_table::symbol_table_t *p_st;

protected:
  void SetUp() override { p_st = new symbol_table::symbol_table_t(); }
  void TearDown() override { free(p_st); }
};

TEST_F(symbol_table_unit_test, general_syntax_test) {
  parser::ast::node::variable_t *p_var = new parser::ast::node::variable_t(
      tok_int, 0, "var_name",
      new parser::ast::node::literal_expr_t(tok_int, "5"));

  p_st->add(p_var);

  ASSERT_EQ(1, p_st->table.size());
  symbol_table::data_t *table_data;
  ASSERT_NO_THROW(table_data = p_st->table["var_name"]);
  ASSERT_EQ(tok_int, table_data->type);
  ASSERT_EQ(0, table_data->pointer_level);
  ASSERT_FALSE(table_data->is_static);
  ASSERT_FALSE(table_data->is_const);
  ASSERT_FALSE(table_data->is_function);
  ASSERT_EQ(5, std::get<int>(table_data->get_value()));
}

TEST_F(symbol_table_unit_test, variable_int_test) {
  parser::ast::node::variable_t *p_var = new parser::ast::node::variable_t(
      tok_int, 0, "var_name",
      new parser::ast::node::literal_expr_t(tok_int, "5"));

  p_st->add(p_var);

  ASSERT_EQ(1, p_st->table.size());
  symbol_table::data_t *table_data;
  ASSERT_NO_THROW(table_data = p_st->table["var_name"]);
  ASSERT_EQ(tok_int, table_data->type);
  ASSERT_EQ(0, table_data->pointer_level);
  ASSERT_FALSE(table_data->is_static);
  ASSERT_FALSE(table_data->is_const);
  ASSERT_FALSE(table_data->is_function);
  ASSERT_EQ(5, std::get<int>(table_data->get_value()));
}

TEST_F(symbol_table_unit_test, variable_float_test) {
  parser::ast::node::variable_t *p_var = new parser::ast::node::variable_t(
      tok_float, 0, "another_var_name",
      new parser::ast::node::literal_expr_t(tok_float, "2.8"));

  p_st->add(p_var);

  ASSERT_EQ(1, p_st->table.size());
  symbol_table::data_t *table_data;
  ASSERT_NO_THROW(table_data = p_st->table["another_var_name"]);
  ASSERT_EQ(tok_float, table_data->type);
  ASSERT_EQ(0, table_data->pointer_level);
  ASSERT_FALSE(table_data->is_static);
  ASSERT_FALSE(table_data->is_const);
  ASSERT_FALSE(table_data->is_function);
  ASSERT_EQ(2.8f, std::get<float>(table_data->get_value()));
}

TEST_F(symbol_table_unit_test, variable_char_test) {
  parser::ast::node::variable_t *p_var = new parser::ast::node::variable_t(
      tok_char, 0, "var_name",
      new parser::ast::node::literal_expr_t(tok_char, 'x'));

  p_st->add(p_var);

  ASSERT_EQ(1, p_st->table.size());
  symbol_table::data_t *table_data;
  ASSERT_NO_THROW(table_data = p_st->table["var_name"]);
  ASSERT_EQ(tok_char, table_data->type);
  ASSERT_EQ(0, table_data->pointer_level);
  ASSERT_FALSE(table_data->is_static);
  ASSERT_FALSE(table_data->is_const);
  ASSERT_FALSE(table_data->is_function);
  ASSERT_EQ('x', std::get<char>(table_data->get_value()));
}

TEST_F(symbol_table_unit_test, variable_string_test) {
  parser::ast::node::variable_t *p_var = new parser::ast::node::variable_t(
      tok_string, 0, "var_name",
      new parser::ast::node::literal_expr_t(tok_string, "Hallo Welt!"));

  p_st->add(p_var);

  ASSERT_EQ(1, p_st->table.size());
  symbol_table::data_t *table_data;
  ASSERT_NO_THROW(table_data = p_st->table["var_name"]);
  ASSERT_EQ(tok_string, table_data->type);
  ASSERT_EQ(0, table_data->pointer_level);
  ASSERT_FALSE(table_data->is_static);
  ASSERT_FALSE(table_data->is_const);
  ASSERT_FALSE(table_data->is_function);
  ASSERT_EQ("Hallo Welt!", std::get<std::string>(table_data->get_value()));
}

TEST_F(symbol_table_unit_test, variable_pointer_test) {
  parser::ast::node::variable_t *p_var = new parser::ast::node::variable_t(
      tok_char, 1, "var_name",
      new parser::ast::node::literal_expr_t(tok_char_literal, "hallo"));

  p_st->add(p_var);

  ASSERT_EQ(1, p_st->table.size());
  symbol_table::data_t *table_data;
  ASSERT_NO_THROW(table_data = p_st->table["var_name"]);
  ASSERT_EQ(tok_char, table_data->type);
  ASSERT_EQ(1, table_data->pointer_level);
  ASSERT_FALSE(table_data->is_static);
  ASSERT_FALSE(table_data->is_const);
  ASSERT_FALSE(table_data->is_function);
  ASSERT_EQ("hallo", std::get<char *>(table_data->get_value()));
}

TEST_F(symbol_table_unit_test, variable_const_test) {
  parser::ast::node::variable_t *p_var = new parser::ast::node::variable_t(
      tok_int, 0, "var_name",
      new parser::ast::node::literal_expr_t(tok_int, 1));

  p_st->add(p_var);

  ASSERT_EQ(1, p_st->table.size());
  symbol_table::data_t *table_data;
  ASSERT_NO_THROW(table_data = p_st->table["var_name"]);
  ASSERT_EQ(tok_char, table_data->type);
  ASSERT_EQ(1, table_data->pointer_level);
  ASSERT_FALSE(table_data->is_static);
  ASSERT_TRUE(table_data->is_const);
  ASSERT_FALSE(table_data->is_function);
  ASSERT_EQ(1, std::get<int>(table_data->get_value()));
  ASSERT_FALSE(table_data->is_function);
}

TEST_F(symbol_table_unit_test, variable_static_test) {
  parser::ast::node::variable_t *p_var = new parser::ast::node::variable_t(
      tok_int, 0, "var_name",
      new parser::ast::node::literal_expr_t(tok_int, 1));

  p_st->add(p_var);

  ASSERT_EQ(1, p_st->table.size());
  symbol_table::data_t *table_data;
  ASSERT_NO_THROW(table_data = p_st->table["var_name"]);
  ASSERT_EQ(tok_char, table_data->type);
  ASSERT_EQ(1, table_data->pointer_level);
  ASSERT_TRUE(table_data->is_static);
  ASSERT_FALSE(table_data->is_const);
  ASSERT_FALSE(table_data->is_function);
  ASSERT_EQ(1, std::get<int>(table_data->get_value()));
}

TEST_F(symbol_table_unit_test, variable_const_static_test) {
  parser::ast::node::variable_t *p_var = new parser::ast::node::variable_t(
      tok_int, 0, "var_name",
      new parser::ast::node::literal_expr_t(tok_int, 1));

  p_st->add(p_var);

  ASSERT_EQ(1, p_st->table.size());
  symbol_table::data_t *table_data;
  ASSERT_NO_THROW(table_data = p_st->table["var_name"]);
  ASSERT_EQ(tok_char, table_data->type);
  ASSERT_EQ(1, table_data->pointer_level);
  ASSERT_TRUE(table_data->is_static);
  ASSERT_TRUE(table_data->is_const);
  ASSERT_FALSE(table_data->is_function);
  ASSERT_EQ(1, std::get<int>(table_data->get_value()));
}

TEST_F(symbol_table_unit_test, variable_double_declare_test) {
  parser::ast::node::variable_t *p_var = new parser::ast::node::variable_t(
      tok_int, 0, "var_name",
      new parser::ast::node::literal_expr_t(tok_int, 1));

  parser::ast::node::variable_t *p_var2 = new parser::ast::node::variable_t(
      tok_string, 0, "var_name",
      new parser::ast::node::literal_expr_t(tok_string, "Moin"));

  p_st->add(p_var);
  ASSERT_THROW(p_st->add(p_var2), exceptions::variable_already_declared_error);
}

TEST_F(symbol_table_unit_test, variable_assign_test) {
  parser::ast::node::variable_t *p_var = new parser::ast::node::variable_t(
      tok_int, 0, "var_name",
      new parser::ast::node::literal_expr_t(tok_int, 1));

  p_st->add(p_var);
  ASSERT_EQ(1, p_st->table.size());
  symbol_table::data_t *table_data;
  ASSERT_NO_THROW(table_data = p_st->table["var_name"]);
  ASSERT_EQ(1, std::get<int>(table_data->get_value()));

  parser::ast::node::assign_expr_t *p_assign =
      new parser::ast::node::assign_expr_t(
          "var_name", new parser::ast::node::literal_expr_t(tok_int, 2));

  p_st->assign(p_assign);
  ASSERT_EQ(1, p_st->table.size());
  ASSERT_NO_THROW(table_data = p_st->table["var_name"]);
  ASSERT_EQ(2, std::get<int>(table_data->get_value()));
}

TEST_F(symbol_table_unit_test, function_test) {
  parser::ast::node::function_t *p_func = new parser::ast::node::function_t(
      tok_int, 0, "main",
      new std::vector<std::tuple<token_e, int>>(
          {std::tuple<token_e, int>(tok_int, 0),
           std::tuple<token_e, int>(tok_int, 0)}),
      new parser::ast::node::block_t());

  p_st->add(p_func);

  ASSERT_EQ(1, p_st->table.size());
  symbol_table::data_t *table_data;
  ASSERT_NO_THROW(table_data = p_st->table["main"]);
  ASSERT_EQ(tok_int, table_data->type);
  ASSERT_EQ(0, table_data->pointer_level);
  ASSERT_FALSE(table_data->is_static);
  ASSERT_FALSE(table_data->is_const);
  ASSERT_TRUE(table_data->is_function);
  ASSERT_EQ(nullptr, std::get<void *>(table_data->get_value()));
}
