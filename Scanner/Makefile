CC=gcc
FLEX=flex

all: scanner

scanner: lex.yy.c
	$(CC) lex.yy.c -o scanner

lex.yy.c: scanner.flex
	$(FLEX) scanner.flex

clean:
	rm -f lex.yy.c scanner
