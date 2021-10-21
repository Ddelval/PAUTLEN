.PHONY: test

pruebaMorfo: pruebaMorfo.o lex.yy.o
	gcc -g -o pruebaMorfo $^

pruebaMorfo.o: pruebaMorfo.c
	gcc -Wall -ansi -pedantic -c -g -o $@ $^

lex.yy.o: alpha.l
	flex $^
	gcc -c -g -o lex.yy.o lex.yy.c

test: pruebaMorfo toName
	./pruebaMorfo test/entrada1.txt hola.txt 
toName: toName.cpp
	g++ toName.cpp -o toName
