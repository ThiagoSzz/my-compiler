# /*---------------------------*/
# /* -->>     Grupo I     <<-- */
# /*        Thiago Haab        */
# /*      Henrique Vazatta     */
# /*---------------------------*/

ETAPA = 4
DELIVERABLE_FILES = main.c ast.c ast.h lexical_value.c lexical_value.h symbol_table.c symbol_table.h common_types.h errors.h parser.y scanner.l Makefile README.md

all: bison flex main ast lexical_value symbol_table
	gcc main.o lex.yy.o parser.tab.o lexical_value.o ast.o symbol_table.o -o etapa$(ETAPA)

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

symbol_table:
	gcc -c symbol_table.c

deliverable:
	make clean
	tar --exclude='.*' -cvzf etapa$(ETAPA).tgz ${DELIVERABLE_FILES}

clean:
	rm -rf *.o *.yy.* *.tab.* etapa$(ETAPA).* ./etapa$(ETAPA)