# Language Grammar {#grammar}

This page documents the formal grammar for the language compiler. The grammar defines the syntactic structure of valid programs.

## Production Rules

The language is defined by the following context-free grammar productions:

### Program Structure

```
program -> defines
         | function
         | ε
```

A program consists of either global variable definitions or function definitions.

### Statements

```
stmts -> stmt ; stmts
       | ε

stmt -> block
      | if ( expr ) stmt
      | else stmt
      | while ( expr ) stmt
      | do stmt while ( expr )
      | for ( define ; relation ; expr ) stmt
      | return expr ;
      | id ( parameters ) ;    // function call
      | define ;

block -> { stmts }
```

Statements include control flow constructs (if, else, while, do-while, for), return statements, function calls, and variable definitions.

### Expressions

```
expr -> ...     ***Handle with Pratt Parser

relation -> ... ***Handle with Pratt Parser
```

Expressions and relational operators are parsed using a Pratt parser to handle operator precedence correctly.

### Functions

```
function -> type id ( defines ) block
```

Functions have a return type, identifier, parameter list, and body block.

### Types

```
type -> type pl

pl -> * pl
    | ε
```

Types support pointer levels through the pointer level (`pl`) production.

### Definitions

```
defines -> define ; defines
         | ε

define -> type id assign

assign -> = expr
         | ε
```

Variable definitions consist of a type, identifier, and optional initialization expression.

## Notes

- **ε** (epsilon) represents the empty string/production
- Expression parsing delegates to a Pratt parser for proper precedence handling
