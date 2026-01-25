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
#include <fstream>
#include <iostream>
#include <ostream>
#include <sstream>
#include <string>
#include <vector>

using namespace compiler;

void setup_spdlog() { spdlog::set_level(spdlog::level::debug); }

void setup_argpars(int argc, char *argv[], argparse::ArgumentParser *program) {
  // setup argparse
  (*program)
      .add_description("A compiler for the lang programming language")
      .add_epilog("Examples:\n"
                  "  lang program.lang --emit-llvm -o output.ll\n"
                  "  lang program.lang --emit-tokens --emit-ast\n"
                  "  lang program.lang --emit-llvm > output.ll");

  (*program)
      .add_argument("source_files")
      .help("source files to compile")
      .remaining();

  (*program)
      .add_argument("--emit-tokens")
      .help("emit tokenized output")
      .default_value(false)
      .implicit_value(true);

  (*program)
      .add_argument("--emit-ast")
      .help("emit abstract syntax tree")
      .default_value(false)
      .implicit_value(true);

  (*program)
      .add_argument("--emit-llvm")
      .help("emit LLVM IR")
      .default_value(false)
      .implicit_value(true);

  (*program)
      .add_argument("-o", "--output")
      .help("output file (default: stdout)")
      .default_value(std::string(""));

  (*program)
      .add_argument("--print-source")
      .help("print source code before compilation")
      .default_value(false)
      .implicit_value(true);

  (*program)
      .add_argument("-v", "--verbose")
      .help("enable verbose output with section headers")
      .default_value(false)
      .implicit_value(true);

  // argparse parse the arguments
  try {
    (*program).parse_args(argc, argv);
  } catch (const std::exception &err) {
    std::cerr << (*program) << std::endl;
    spdlog::critical("Error parsing arguments:\n\t{0}", err.what());
    exit(1);
  }

  // Check if no source files provided
  if (!(*program).is_used("source_files")) {
    std::cerr << (*program) << std::endl;
    spdlog::info("No source files provided");
    exit(1);
  }

  // If no emit flags are set, default to emit-llvm
  bool emit_tokens = (*program).get<bool>("--emit-tokens");
  bool emit_ast = (*program).get<bool>("--emit-ast");
  bool emit_llvm = (*program).get<bool>("--emit-llvm");

  if (!emit_tokens && !emit_ast && !emit_llvm) {
    spdlog::debug("No emit flags specified, defaulting to --emit-llvm");
    // Note: We can't modify the parsed args, so we'll handle this in
    // run_compiler
  }
}

void debug_print_tokens(std::vector<token_t *> tokens, std::ostream &out) {
  int current_linenumber = 1;

  out << current_linenumber << ":\t";
  for (token_t *tok : tokens) {
    if (current_linenumber != tok->linenumber) {
      out << std::endl;
      current_linenumber = tok->linenumber;
      out << current_linenumber << ":\t";
    }
    std::string s = debug_tok(tok);
    out << s << " ";
  }
  out << std::endl;
}

void debug_print_ast(parser::ast::abstract_syntax_tree_t *program_tree,
                     std::ostream &out) {
  out << program_tree->debug_print() << std::endl;
}

void print_source(std::string t_source_path, std::ostream &out) {
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

  out << source;
}

void run_compiler(std::vector<std::string> source_files,
                  argparse::ArgumentParser &program) {
  // Get command-line options
  bool emit_tokens = program.get<bool>("--emit-tokens");
  bool emit_ast = program.get<bool>("--emit-ast");
  bool emit_llvm = program.get<bool>("--emit-llvm");
  bool print_src = program.get<bool>("--print-source");
  bool verbose = program.get<bool>("--verbose");
  std::string output_file = program.get<std::string>("--output");

  // Default to emit-llvm if nothing specified
  if (!emit_tokens && !emit_ast && !emit_llvm) {
    emit_llvm = true;
  }

  // Determine output stream
  std::ofstream file_stream;
  std::ostream *out = &std::cout;

  if (!output_file.empty()) {
    file_stream.open(output_file);
    if (!file_stream.is_open()) {
      spdlog::critical("Failed to open output file: {}", output_file);
      exit(1);
    }
    out = &file_stream;
    spdlog::info("Writing output to: {}", output_file);
  }

  // start compiling
  for (std::string source_path : source_files) {
    if (print_src) {
      if (verbose)
        *out << "========== SOURCE ==========" << std::endl;
      print_source(source_path, *out);
      if (verbose)
        *out << std::endl;
    }

    // tokenize
    std::vector<token_t *> tokens = lexer::lex_file(source_path);

    if (emit_tokens) {
      if (verbose)
        *out << "========== LEXER ==========" << std::endl;
      debug_print_tokens(tokens, *out);
      if (verbose)
        *out << std::endl;
    }

    // parse
    std::vector<parser::ast::abstract_syntax_tree_t *> *trees =
        parser::parse_tokens(tokens);

    if (emit_ast) {
      if (verbose)
        *out << "========== PARSER ==========" << std::endl;
      for (parser::ast::abstract_syntax_tree_t *tree : *trees) {
        debug_print_ast(tree, *out);
      }
      if (verbose)
        *out << std::endl;
    }

    // code generation
    if (emit_llvm) {
      if (verbose)
        *out << "========== LLVMIR CODE GENERATION ==========" << std::endl;

      ast::codegen_visitor_t visitor = ast::codegen_visitor_t();
      for (parser::ast::abstract_syntax_tree_t *tree : *trees) {
        tree->p_head->accept_visitor(&visitor);
      }

      // Capture IR output
      std::stringstream ir_stream;
      std::streambuf *old_cout = std::cout.rdbuf(ir_stream.rdbuf());
      visitor.dump_ir();
      std::cout.rdbuf(old_cout);

      *out << ir_stream.str();
    }
  }

  if (file_stream.is_open()) {
    file_stream.close();
    spdlog::info("Output written successfully");
  }
}

int main(int argc, char *argv[]) {
  setup_spdlog();

  argparse::ArgumentParser program = argparse::ArgumentParser("lang");
  setup_argpars(argc, argv, &program);

  // read arguments
  std::vector<std::string> source_files =
      program.get<std::vector<std::string>>("source_files");

  run_compiler(source_files, program);

  return 0;
}
