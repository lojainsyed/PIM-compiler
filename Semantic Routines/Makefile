all: compiler.exe

compiler.exe: parser.c scanner.c main.c
	gcc main.c scanner.c parser.c -o compiler.exe

parser.c token.h: parser.bison
	bison --defines=token.h --output=parser.c parser.bison -v

scanner.c: scanner.flex token.h
	flex -o scanner.c scanner.flex

clean:
	rm -f compiler.exe parser.c token.h scanner.c parser.output parser.tab.bison
