#include "argparse/argparse.hpp"
#include "lexer/lexer.h"
#include "tokens.h"
#include <cstdlib>
#include <iostream>
#include <ostream>
#include <string>
#include <vector>

int main(int argc, char *argv[]) {
  // setup argparse
  argparse::ArgumentParser program("lang");

  program.add_argument("source_files")
      .help("source files to compile")
      .remaining();

  // argparse parse the arguments
  try {
    program.parse_args(argc, argv);
  } catch (const std::exception &err) {
    std::cerr << err.what() << std::endl;
    std::cerr << program;
    return 1;
  }

  // Check if no source files provided
  if (!program.is_used("source_files")) {
    std::cout << program;
    return 1;
  }

  // read arguments
  std::vector<std::string> source_files =
      program.get<std::vector<std::string>>("source_files");

  // start lexing
  for (std::string source_path : source_files) {
    std::vector<lexer::token_t *> tokens = lexer::lex_file(source_path);

    std::cout << source_path << std::endl;
    for (lexer::token_t *tok : tokens) {
      lexer::debug_print(tok);
    }
    std::cout << std::endl;
  }

  return 0;
}
