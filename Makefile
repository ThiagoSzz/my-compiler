all: bison flex main
	gcc main.o lex.yy.o parser.tab.o -o etapa2

bison:
	bison -d parser.y -Wcounterexamples
	gcc -c parser.tab.c

flex:
	flex scanner.l
	gcc -c lex.yy.c

main:
	gcc -c main.c

clean:
	rm -f *.o *.yy.* *.tab.* etapa2