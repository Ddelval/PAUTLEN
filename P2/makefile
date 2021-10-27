.PHONY: test
all:  pruebaMorfo

pruebaMorfo: pruebaMorfo.o lex.yy.o
	gcc -g -o $@ $^

pruebaMorfo.o: pruebaMorfo.c
	gcc -Wall -ansi -pedantic -c -g -o $@ $^

lex.yy.o: alpha.l
	flex $^
	gcc -c -g -o lex.yy.o lex.yy.c

test: pruebaMorfo 
	./check_tests.sh

clean:
	rm pruebaMorfo *.o 
	rm -r test testOut