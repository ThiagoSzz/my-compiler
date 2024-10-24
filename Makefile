# /*---------------------------*/
# /* -->>     Grupo I     <<-- */
# /*        Thiago Haab        */
# /*      Henrique Vazatta     */
# /*---------------------------*/

ETAPA = 2
DELIVERABLE_FILES = main.c parser.y scanner.l Makefile README.md


all: bison flex main
	gcc main.o lex.yy.o parser.tab.o -o etapa$(ETAPA)

bison:
	bison -d parser.y
	gcc -c parser.tab.c

flex:
	flex scanner.l
	gcc -c lex.yy.c

main:
	gcc -c main.c

deliverable:
	make clean
	tar cvzf etapa$(ETAPA).tgz ${DELIVERABLE_FILES}

clean:
	rm -rf *.o *.yy.* *.tab.* etapa$(ETAPA).* ./etapa$(ETAPA)