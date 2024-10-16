# Compiler Project

Project for a compiler made for a C-like language using [flex](https://github.com/westes/flex), [bison](https://github.com/akimd/bison) and the C programming language with [gcc](https://github.com/gcc-mirror/gcc).

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

## How to execute

### Build

To build this program, use `make`. First, navigate to the directory containing the Makefile, then run the following command:

```bash
make
```
> [!IMPORTANT]
> You should have both flex and bison generators, as well as the gcc compiler installed previously for this step to work as expected.

### Run

To run the program, execute the following command in terminal:

```bash
./etapaX
```
> [!TIP]
> You can run the program with an input file by using `./etapaX < filename` (e.g `./etapaX < input.txt`)

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