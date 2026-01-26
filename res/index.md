# Language Compiler Documentation

Welcome to the documentation for the Language Compiler project - a complete compiler implementation featuring lexical analysis, recursive descent parsing with Pratt parsing for expressions, and LLVM-based code generation.

## Overview

This compiler transforms source code written in a custom language into LLVM IR (Intermediate Representation). The compilation process consists of three main stages:

1. **Lexical Analysis** - Tokenization of source code using regex-based pattern matching
2. **Parsing** - Construction of Abstract Syntax Trees (AST) using recursive descent parsing with Pratt parsing for expressions
3. **Code Generation** - LLVM IR generation using the visitor pattern

## Key Features

- **Type System**: Support for `int`, `float`, `char`, `bool`, and `void` types
- **Control Flow**: Full support for `if/else`, `for`, `while`, and `return` statements
- **Expression Handling**: Pratt parsing for correct operator precedence and associativity
- **Modular Architecture**: Clean separation between lexer, parser, and code generation components
- **Visitor Pattern**: Flexible AST traversal for code generation and analysis

## Getting Started

### Quick Build

```bash
make clean build
./lang-compiler <source-file>
```

### Basic Usage

```bash
# Compile and emit LLVM IR
./lang-compiler --emit-llvm program.lang

# Show all compilation stages
./lang-compiler --emit-tokens --emit-ast --emit-llvm -v program.lang

# Save output to file
./lang-compiler --emit-llvm -o output.ll program.lang
```

## Architecture

### Core Components

- **@ref compiler::lexer::lexer_t** - Tokenizes source code into lexical tokens
- **@ref compiler::parser::parse_tokens** - Parses token streams into AST structures
- **@ref compiler::ast::codegen_visitor_t** - Generates LLVM IR from AST nodes

### Module Structure

```
include/
├── code_generation/     # LLVM IR generation and AST visitors
├── lexer/              # Tokenization and lexical analysis
└── parser/             # AST definition and parsing logic
```

## Documentation Sections

### Lexer
The lexer module performs tokenization of source code. See @ref compiler::lexer for details on:
- Token recognition using regex patterns
- File-based lexical analysis
- Line number tracking

### Parser
The parser constructs Abstract Syntax Trees from token streams. See @ref compiler::parser for:
- Recursive descent parsing
- Pratt parsing for expressions
- AST node definitions and structure

### Code Generation
The code generation module produces LLVM IR. See @ref compiler::ast for:
- Visitor pattern implementation
- LLVM IR generation for all language constructs
- Type conversion and management

## Example Programs

The `examples/` directory contains sample programs demonstrating language features:

- `arithmetic_operations.lang` - Basic arithmetic and operators
- `chained_calculations.lang` - Complex expression evaluation
- `for_loop.lang` - Loop constructs
- `function_division.lang` - Function definitions and calls

## Development

### Requirements

**Arch Linux:**
```bash
pacman -S llvm cmake clang make doxygen
```

**Debian/Ubuntu:**
```bash
apt install llvm clang cmake make zlib1g-dev libzstd-dev libcurl4-openssl-dev libedit-dev doxygen
```

### Building from Source

```bash
mkdir build && cd build
cmake ..
make
```

### Running Tests

```bash
make test
```

## Dependencies

- **LLVM** 19+ (Debian) or 21+ (Arch) - Code generation backend
- **C++17** - Modern C++ features
- **CMake** 3.14+ - Build system
- **spdlog** - Logging framework (fetched automatically)
- **GoogleTest** - Testing framework (fetched automatically)
- **argparse** - Command-line argument parsing (fetched automatically)

## Project Structure

| Directory | Description |
|-----------|-------------|
| `include/` | Public header files |
| `src/` | Implementation files |
| `test/` | Unit, module, and integration tests |
| `examples/` | Example programs in the language |
| `doc/` | Additional documentation |
| `res/` | Resources including Doxygen configuration |

## Additional Resources

- **README.md** - Project overview and quick start guide
- **language-production.md** - Formal grammar specification
- **LICENSE.md** - Licensing information

## Contributing

When contributing to this project, please ensure:
- Code follows the existing style conventions
- New features include appropriate unit tests
- Documentation is updated for API changes
- Commit messages are clear and descriptive

---

**License**: See LICENSE.md
