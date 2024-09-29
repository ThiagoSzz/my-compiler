# Compiler Project

Project for a compiler made for a C-like language using [flex](https://github.com/westes/flex) and the C programming language.

## Features

### Step 1

- Counts line number
- Returns `ERROR` token for invalid entries
- Ignores single-line comments (e.g `// comment`)
- Returns tokens for reserved identifiers (e.g `int`, `float`), compound operators (e.g `<=`, `!=`) and special characters (e.g `!`, `+`)
- Returns tokens for identifiers and literals

## How to execute

### Build

To build this program, use `make`. First, navigate to the directory containing the Makefile, then run the following command:

```bash
make
```

### Run

To run the program, execute the following command in terminal:

```bash
./etapa1
```

### Run with input file

If you want to run the program with an input file (e.g `input.txt`), use the following command:

```bash
./etapa1 < input.txt
```

### Clean

If you want to clean up your environment (delete `target` folder and executable), you can use the following command:

```bash
make clean
```
