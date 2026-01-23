#include "argparse/argparse.hpp"
#include "ast/print_test_visitor.h"
#include "lexer/lexer.h"
#include "parser/abstract_syntax_tree.h"
#include "parser/parser.h"
#include "spdlog/fmt/bundled/base.h"
#include "spdlog/spdlog.h"
#include "tokens.h"
#include <cstdlib>
#include <iostream>
#include <ostream>
#include <string>
#include <vector>

using namespace compiler;

void setup_spdlog() { spdlog::set_level(spdlog::level::debug); }

void setup_argpars(int argc, char *argv[], argparse::ArgumentParser *program) {
  // setup argparse
  (*program)
      .add_argument("source_files")
      .help("source files to compile")
      .remaining();

  // argparse parse the arguments
  try {
    (*program).parse_args(argc, argv);
  } catch (const std::exception &err) {
    spdlog::critical("Error parsing arguments. Terminating...:\n\t{0}",
                     err.what());
    exit(1);
  }

  // Check if no source files provided
  if (!(*program).is_used("source_files")) {
    spdlog::info("No source files are provided. Terminating...");
    exit(1);
  }
}

void debug_print_tokens(std::vector<token_t *> tokens) {
  int current_linenumber = 1;

  std::cout << current_linenumber << ":\t";
  for (token_t *tok : tokens) {
    if (current_linenumber != tok->linenumber) {
      std::cout << std::endl;
      current_linenumber = tok->linenumber;
      std::cout << current_linenumber << ":\t";
    }
    std::string s = debug_tok(tok);
    std::cout << s << " ";
  }
  std::cout << std::endl;
}

void debug_print_ast(parser::ast::abstract_syntax_tree_t *program_tree) {
  std::cout << program_tree->debug_print() << std::endl;
}

void run_compiler(std::vector<std::string> source_files) {
  // start compiling
  for (std::string source_path : source_files) {
    // tokenize
    fmt::println("========== LEXER ==========");
    std::vector<token_t *> tokens = lexer::lex_file(source_path);
    // spdlog::info("Lexing finished");
    debug_print_tokens(tokens);
    // parse
    fmt::println("========== PARSER ==========");
    std::vector<parser::ast::abstract_syntax_tree_t *> *trees =
        parser::parse_tokens(tokens);

    for (parser::ast::abstract_syntax_tree_t *tree : *trees) {
      debug_print_ast(tree);
    }

    fmt::println("========== LLVMIR CODE GENERATION ==========");
    for (parser::ast::abstract_syntax_tree_t *tree : *trees) {
      ast::print_test_visitor_t visitor = ast::print_test_visitor_t();
      tree->p_head->accept_visitor(&visitor);
    }
    // Code generation finished

    // spdlog::info("Code generation finished");

    // debug print syntax_tree
    // std::cout << tree->to_prefix_notation() << std::endl;
  }
}

int main(int argc, char *argv[]) {
  setup_spdlog();

  argparse::ArgumentParser program = argparse::ArgumentParser("lang");
  setup_argpars(argc, argv, &program);

  // read arguments
  std::vector<std::string> source_files =
      program.get<std::vector<std::string>>("source_files");

  // std::vector<std::string> source_files;
  // source_files.push_back("examples/dynamic_typing.lang");
  run_compiler(source_files);

  return 0;
}
