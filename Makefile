all: bison flex main
	gcc main.o lex.yy.o parser.tab.o -o etapa2

bison:
	bison -d parser.y -Wcounterexamples
	gcc -c parser.tab.c

flex:
	flex scanner.l
	gcc -c lex.yy.c

main:
	gcc -c main.c -o main.o

clean:
	rm -f etapa2 *.o lex.yy.c parser.tab.c parser.tab.h etapa2.tgz