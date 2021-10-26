.PHONY: test

pruebaMorfo: pruebaMorfo.o lex.yy.o
	gcc -g -o pruebaMorfo $^

pruebaMorfo.o: pruebaMorfo.c
	gcc -Wall -ansi -pedantic -c -g -o $@ $^

lex.yy.o: alpha.l
	flex $^
	gcc -c -g -o lex.yy.o lex.yy.c

test: pruebaMorfo 
	./pruebaMorfo test/entrada2.txt hola.txt 
