# Run main step
etapa1: target compile_scanner compile_main
	gcc -o etapa1 target/lex.yy.o target/main.o

# Run test step
etapa1_test: target compile_scanner compile_test_main
	gcc -o etapa1 target/lex.yy.o target/test_main.o

# Create deliverable etapa1
etapa1_final:
	mkdir -p etapa1
	cp -r ./src ./etapa1
	cp -r ./include ./etapa1
	cp Makefile ./etapa1
	tar cvzf etapa1.tgz ./etapa1
	rm -rf etapa1

# Compile main file
compile_main:
	gcc -c src/main.c -I include/ -o target/main.o

# Compile test main file
compile_test_main:
	gcc -c tests/test_main.c -I include/ -o target/test_main.o

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