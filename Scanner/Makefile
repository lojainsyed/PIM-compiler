CC = gcc
FLEX = flex

scanner: lex.yy.c
	$(CC) lex.yy.c main.c -o scanner

lex.yy.c: scanner.flex
	$(FLEX) scanner.flex

clean:
	rm -f lex.yy.c scanner
