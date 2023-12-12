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

lex.yy.c:
	flex scanner.l
parser.tab.c:
	bison -d parser.y

parser.tab.h:
	bison -d parser.y

%.o: %.c $(DEPS)
	$(CC) $(CFLAGS) -c -o $@ $<

$(ETAPA): $(OBJ)
	$(CC) $(CFLAGS) -o $@ $^

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
