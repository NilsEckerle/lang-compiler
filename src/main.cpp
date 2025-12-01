#include "argparse/argparse.hpp"
#include "lexer/lexer.h"
#include "parser/parser.h"
#include "parser/syntax_tree.h"
#include "spdlog/spdlog.h"
#include "tokens.h"
#include <cstdlib>
#include <iostream>
#include <ostream>
#include <string>
#include <vector>

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

void run_compiler(std::vector<std::string> source_files) {
  // start compiling
  for (std::string source_path : source_files) {
    // tokenize
    std::vector<lexer::token_t *> tokens = lexer::lex_file(source_path);
    spdlog::info("Lexing finished");
    // parse
    parser::syntax_tree_t *tree = parser::parse_tokens(tokens);
    spdlog::info("Parsing finished");
    // Code generation finished

    spdlog::info("Code generation finished");

    //
    // DEBUG PRINTING
    //
    // debug print tokens
    std::cout << source_path << std::endl;
    for (lexer::token_t *tok : tokens) {
      lexer::debug_print(tok);
    }
    std::cout << std::endl;

    // debug print syntax_tree
    std::cout << tree->to_prefix_notation() << std::endl;
  }
}

int main(int argc, char *argv[]) {
  setup_spdlog();
  argparse::ArgumentParser program = argparse::ArgumentParser("lang");
  setup_argpars(argc, argv, &program);

  // read arguments
  std::vector<std::string> source_files =
      program.get<std::vector<std::string>>("source_files");

  run_compiler(source_files);

  return 0;
}
