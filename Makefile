ETAPA=etapa1
CC=gcc
CFLAGS=-I.
DEPS=tokens.h
OBJ=lex.yy.o main.o

lex.yy.c:
	flex scanner.l

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

$(ETAPA): $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS)

run: $(ETAPA)
	./$(ETAPA)

clean:
	rm -f lex.yy.* main.o etapa*
	rm -rf entrega

entrega: clean
	mkdir entrega
	cp *.c *.h *.l Makefile entrega
	tar cvzf $(ETAPA).tgz -C entrega .
	rm -rf entrega

