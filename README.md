# Language Compiler

A compiler implementation featuring lexical analysis, parsing with Pratt parsing for expressions, and LLVM-based code generation.

## Features

- **Lexer**: Regex-based tokenization
- **Parser**: Recursive descent with Pratt parsing for expressions
- **AST**: Visitor pattern for code generation
- **Codegen**: LLVM IR generation
- **Types**: `int`, `float`, `char`, `bool`, `void`
- **Control Flow**: `if/else`, `for`, `while`, `return`

## Requirements

### Arch Linux
```bash
pacman -S llvm cmake clang make
```

### Debian/Ubuntu
```bash
apt install llvm clang cmake make zlib1g-dev libzstd-dev libcurl4-openssl-dev libedit-dev
```

## Build

```bash
make clean build
./lang-compiler <source-file>
```

## Project Structure

```
├── include/           # Header files
│   ├── code_generation/
│   ├── lexer/
│   └── parser/
├── src/              # Implementation
├── test/             # Unit and module tests
└── examples/         # Example programs
```

## Dependencies

- LLVM 19+ (Debian) or 21+ (Arch)
- C++17
- CMake 3.14+
- spdlog (fetched automatically)
- GoogleTest (fetched automatically)
