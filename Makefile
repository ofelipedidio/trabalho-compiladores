# Porto Alegre, Novembro de 2023
# INF01147 - Compiladores
# 
# Grupo B
# Felipe Souza Didio - 00323392
# Pedro Company Beck - 00324055
#

ETAPA=etapa4
CC=gcc
#CFLAGS=-fsanitize=address,leak -g
 CFLAGS=
DEPS=parser.tab.h ast.h lexeme.h semantics.h
OBJ=lex.yy.o main.o parser.tab.o ast.o lexeme.o semantics.o

all: clean $(ETAPA)

scanner.l:

parser.y:

lex.yy.c: scanner.l
	flex scanner.l
parser.tab.c: parser.y
	bison -d parser.y

parser.tab.h: parser.y
	bison -d parser.y

%.o: %.c $(DEPS)
	$(CC) $(CFLAGS) -c -o $@ $<

$(ETAPA): $(OBJ)
	$(CC) $(CFLAGS) -o $@ $^

.PHONY: run clean entrega test

run: $(ETAPA)
	./$(ETAPA)

clean:
	rm -f lex.yy.* *.o etapa* parser.tab.*
	rm -rf entrega

entrega: clean
	mkdir entrega
	cp *.c *.h *.l *.y Makefile entrega 2>/dev/null | true
	tar cvzf $(ETAPA).tgz -C entrega .
	rm -rf entrega

test: clean $(ETAPA)
	cp $(ETAPA) test/program
	cd test; ./test_positive.sh
	cd test; ./test_negative.sh
