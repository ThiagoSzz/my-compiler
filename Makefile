# Run main step
etapa1: target compile_scanner compile_main
	gcc -o etapa1 target/lex.yy.o target/main.o

# Compile main file
compile_main:
	gcc -c src/main.c -I include/ -o target/main.o

# Compile scanner file
compile_scanner: run_flex
	gcc -c target/lex.yy.c -I include/ -o target/lex.yy.o

# Run flex to generate scanner
run_flex:
	flex -o target/lex.yy.c src/scanner.l

# Create target directory
target:
	mkdir -p target

# Remove generated files and folders
clean:
	rm -rf target etapa1