#include "lexer/lexer.h"
#include "tokens.h"
#include <gtest/gtest.h>
#include <iostream>
#include <ostream>
#include <string>
#include <vector>

using namespace compiler;

// Example unit test for tokens module
class lexer_modul_test : public ::testing::Test {
protected:
  void SetUp() override {
    // Setup code here
  }

  void TearDown() override {
    // Cleanup code here
  }
};

// Test token creation
TEST_F(lexer_modul_test, lex_file) {
  std::vector<token_t *> tokens =
      lexer::lex_file("../test/moduletest/lexer_test_file.lang");

  std::string expected = "int (int)\n"
                         "id (main)\n"
                         "lparen (()\n"
                         "int (int)\n"
                         "id (argc)\n"
                         "comma (,)\n"
                         "char (char)\n"
                         "star (*)\n"
                         "id (argv)\n"
                         "lbracket ([)\n"
                         "rbracket (])\n"
                         "rparen ())\n"
                         "lbrace ({)\n"
                         "int (int)\n"
                         "id (n_times)\n"
                         "assign (=)\n"
                         "number (5)\n"
                         "semicolon (;)\n"
                         "id (for)\n"
                         "lparen (()\n"
                         "int (int)\n"
                         "id (i)\n"
                         "assign (=)\n"
                         "number (0)\n"
                         "semicolon (;)\n"
                         "id (i)\n"
                         "lt (<)\n"
                         "id (n_times)\n"
                         "semicolon (;)\n"
                         "id (i)\n"
                         "plus (+)\n"
                         "plus (+)\n"
                         "rparen ())\n"
                         "lbrace ({)\n"
                         "id (printf)\n"
                         "lparen (()\n"
                         "string (\"%d. Hello World\\n\")\n"
                         "comma (,)\n"
                         "id (i)\n"
                         "plus (+)\n"
                         "number (1)\n"
                         "rparen ())\n"
                         "semicolon (;)\n"
                         "rbrace (})\n"
                         "return (return)\n"
                         "number (0)\n"
                         "semicolon (;)\n"
                         "rbrace (})\n"
                         "eof ()\n";

  std::string token_string;
  for (token_t *tok : tokens) {
    token_string = token_string + tok->type_name() + " (" + tok->t_val + ")\n";
  }

  std::cout << token_string << std::endl;
  std::cout << expected << std::endl;

  EXPECT_EQ(expected, token_string);
}
