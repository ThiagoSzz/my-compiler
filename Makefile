# /*---------------------------*/
# /* -->>     Grupo I     <<-- */
# /*        Thiago Haab        */
# /*      Henrique Vazatta     */
# /*---------------------------*/

ETAPA = 3
DELIVERABLE_FILES = main.c parser.y scanner.l Makefile README.md lexical_value.h lexical_value.c ast.h ast.c

all: bison flex main ast lexical_value
	gcc main.o lex.yy.o parser.tab.o lexical_value.o ast.o -o etapa$(ETAPA)

bison:
	bison -d parser.y
	gcc -c parser.tab.c

flex:
	flex scanner.l
	gcc -c lex.yy.c

main:
	gcc -c main.c

lexical_value:
	gcc -c lexical_value.c

ast:
	gcc -c ast.c

deliverable:
	make clean
	tar cvzf etapa$(ETAPA).tgz ${DELIVERABLE_FILES}

clean:
	rm -rf *.o *.yy.* *.tab.* etapa$(ETAPA).* ./etapa$(ETAPA)