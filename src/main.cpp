#include "argparse/argparse.hpp"
#include "code_generation/codegen_visitor.h"
#include "code_generation/print_test_visitor.h"
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

void print_source(std::string t_source_path) {
  FILE *tp_file = fopen(t_source_path.c_str(), "rb");
  if (tp_file == NULL) {
    throw std::runtime_error("Failed to open file: " + t_source_path);
  }

  // get file size
  fseek(tp_file, 0, SEEK_END);
  long file_size = ftell(tp_file);
  fseek(tp_file, 0, SEEK_SET);

  // allocate buffer and read file
  std::string source;
  source.resize(file_size);
  fread(&source[0], 1, file_size, tp_file);
  fclose(tp_file);

  fmt::println("{}", source);
}

void run_compiler(std::vector<std::string> source_files) {
  // start compiling
  for (std::string source_path : source_files) {
    fmt::println("========== SOURCE ==========");
    print_source(source_path);

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

    // code generation
    fmt::println("========== LLVMIR CODE GENERATION ==========");
    ast::codegen_visitor_t visitor = ast::codegen_visitor_t();
    for (parser::ast::abstract_syntax_tree_t *tree : *trees) {
      // ast::print_test_visitor_t visitor = ast::print_test_visitor_t();
      tree->p_head->accept_visitor(&visitor);
    }
    visitor.dump_ir();
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
