# Compiler Project

Project for a compiler made for a C-like language using [flex](https://github.com/westes/flex), [bison](https://github.com/akimd/bison) and the C programming language with [gcc](https://github.com/gcc-mirror/gcc).

For a detailed overview of what was added in each iterate, take a look at the closed [Pull Requests](https://github.com/ThiagoSzz/my-compiler/pulls?q=is%3Apr+is%3Aclosed).

## Features

### Iterate 1: Lexical Analysis

- Counts line number
- Returns `ERROR` token for invalid entries
- Ignores single-line comments (e.g `// comment`)
- Returns tokens for reserved identifiers (e.g `int`, `float`), compound operators (e.g `<=`, `!=`) and special characters (e.g `!`, `+`)
- Returns tokens for identifiers and literals

### Iterate 2: Syntax Analysis

- Displays syntax error cause
- Implements rules for variable declaration, variable assignment, function call and more
- Implements rules for expressions
- Implements rules for function definition, command blocks
- Implements rules for unary and binary operations like negation and comparison (e.g `a >= b`, `a == b`)

### Iterate 3: Abstract Syntax Tree

- Creates the AST data structure
- Builds the AST in the parser
- Creates the LexicalValue data structure
- Associates the values in the scanner
- Prints the AST recursively using DFS

### Iterate 4: Semantic Analysis

- Creates the Symbol Table data structure
- Builds the Symbol Table stack in the parser
- Throws `ERR_UNDECLARED` (failed to use undeclared variable/function), `ERR_DECLARED` (failed to re-declare variable/function), `ERR_VARIABLE` (failed to use variable as a function call) and `ERR_FUNCTION` (failed to use function call as a variable) errors
- Infers data types on expressions and variable assignments

### Iterate 5: Code Generation

- Creates the Operation List data structure
- Builds the Operation List in the parser
- Implements variable declaration address logic (rfp, rbss)
- Generates code for if statements (with/without else), while statements, unary expressions (negation, logical not), comparison expressions (==, !=, <, >, <=, >=), arithmetic expressions (+, -, *, /), loading identifiers (global/local) and storing identifiers (global/local)

## How to execute

### Build

To build the compiler, use `make`. First, navigate to the directory containing the Makefile, then run the following command:

```bash
make
```
> [!IMPORTANT]
> You should have both flex and bison generators, as well as the gcc compiler installed previously for this step to work as expected.

### Run

To run the compiler, execute the following command in terminal:

```bash
./etapaX
```
> [!TIP]
> You can run the compiler with an input file by using `./etapaX < filename` (e.g `./etapaX < input.txt`).

### Clean

If you want to clean up your environment (delete generated files and executable), you can use the following command:

```bash
make clean
```

### Create deliverable

To generate the deliverable `etapaX.tgz` folder, run the following command:

```bash
make deliverable
```

This will move all important files to the `etapaX` directory and then compress it into `etapaX.tgz`. Then, unzip it wherever you want, open the terminal, navigate to the unzipped folder, run and execute using the previous steps.
