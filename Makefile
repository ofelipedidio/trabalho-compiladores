ETAPA=etapa2
CC=gcc
CFLAGS=-I.
DEPS=parser.tab.h
OBJ=lex.yy.o main.o parser.tab.o

all: clean $(ETAPA)
	

lex.yy.c:
	flex scanner.l

parser.tab.c:
	bison -d parser.y

parser.tab.h:
	bison -d parser.y

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

$(ETAPA): $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS)

run: $(ETAPA)
	./$(ETAPA)

clean:
	rm -f lex.yy.* *.o etapa* parser.tab.*
	rm -rf entrega

entrega: clean
	mkdir entrega
	cp *.c *.h *.l *.y Makefile entrega
	tar cvzf $(ETAPA).tgz -C entrega .
	rm -rf entrega

test: clean $(ETAPA)
	cp $(ETAPA) test/program
	cd test; ./test_positive.sh
	cd test; ./test_negative.sh
