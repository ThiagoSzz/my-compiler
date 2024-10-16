ETAPA = 2
DELIVERABLE_FILES = main.c parser.y scanner.l Makefile README.md


all: bison flex main
	gcc main.o lex.yy.o parser.tab.o -o etapa$(ETAPA)

bison:
	bison -d parser.y -Wcounterexamples
	gcc -c parser.tab.c

flex:
	flex scanner.l
	gcc -c lex.yy.c

main:
	gcc -c main.c

deliverable:
	mkdir etapa$(ETAPA)
	cp ${DELIVERABLE_FILES} ./etapa$(ETAPA)
	tar cvzf etapa$(ETAPA).tgz etapa$(ETAPA)
	rm -rf etapa$(ETAPA)

clean:
	rm -rf *.o *.yy.* *.tab.* etapa$(ETAPA).* ./etapa$(ETAPA)