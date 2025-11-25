#include "lexer/lexer.h"
#include "tokens.h"
#include <gtest/gtest.h>
#include <string>
#include <vector>

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
  std::vector<lexer::token_t *> tokens =
      lexer::lex_file("../test/moduletest/lexer_test_file.lang");

  std::string expected =
      "comment (// TODO: #include must be handled by preprocessor)\n"
      "comment (// TODO: implement preprocessor)\n"
      "comment (//#include <stdio.h> // use c library)\n"
      "int ()\n"
      "id (main)\n"
      "lparen ()\n"
      "int ()\n"
      "id (argc)\n"
      "comma ()\n"
      "char ()\n"
      "star ()\n"
      "id (argv)\n"
      "lbracket ()\n"
      "rbracket ()\n"
      "rparen ()\n"
      "lbrace ()\n"
      "let ()\n"
      "id (n_times)\n"
      "assign ()\n"
      "number (5)\n"
      "semicolon ()\n"
      "comment (// type: const int)\n"
      "for ()\n"
      "lparen ()\n"
      "let ()\n"
      "mut ()\n"
      "id (i)\n"
      "assign ()\n"
      "number (0)\n"
      "semicolon ()\n"
      "id (i)\n"
      "lt ()\n"
      "number (5)\n"
      "semicolon ()\n"
      "id (i)\n"
      "plus ()\n"
      "plus ()\n"
      "rparen ()\n"
      "lbrace ()\n"
      "comment (// type: mutable int)\n"
      "id (printf)\n"
      "lparen ()\n"
      "string (\"%d. Hello World\\n\")\n"
      "comma ()\n"
      "id (i)\n"
      "plus ()\n"
      "number (1)\n"
      "rparen ()\n"
      "semicolon ()\n"
      "id (printf)\n"
      "lparen ()\n"
      "id (f)\n"
      "string (\"{i+1}. Hello World\\n\")\n"
      "rparen ()\n"
      "semicolon ()\n"
      "comment (// possible with basic types like char, char*, short, int, "
      "long, float, int, bool)\n"
      "rbrace ()\n"
      "return ()\n"
      "number (0)\n"
      "semicolon ()\n"
      "rbrace ()\n"
      "eof ()\n";

  std::string token_string;
  for (lexer::token_t *tok : tokens) {
    token_string = token_string + tok->type_name() + " (" + tok->t_val + ")\n";
  }

  EXPECT_TRUE(token_string == expected);
}
