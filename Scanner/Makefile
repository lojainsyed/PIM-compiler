CC=gcc
CFLAGS=-Wall -Wextra -O2

all: scanner

# Generate lex.yy.c from scanner.l
lex.yy.c: scanner.flex tokens.h
	flex -o lex.yy.c scanner.flex

# Compile the scanner (NO -lfl needed)
scanner: lex.yy.c main.c tokens.h
	$(CC) $(CFLAGS) -o scanner lex.yy.c main.c

clean:
	rm -f scanner lex.yy.c
