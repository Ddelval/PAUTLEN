#.PHONY: test
all:  pruebaSintactico
CC = gcc
pruebaSintactico: y.tab.o lex.yy.o pruebaSintactico.o
	$(CC) -g -o $@ $^

pruebaSintactico.o: pruebaSintactico.c
	$(CC) -Wall -ansi -pedantic -c -g -o $@ $^

lex.yy.o: alfa.l
	flex $^
	$(CC) -c -g -o lex.yy.o lex.yy.c

y.tab.o: alfa.y
	bison -dyv $^
	$(CC) -c -g -o y.tab.o y.tab.c

#testSimple: pruebaSintactico
#	./pruebaSintactico test.txt out.txt
#test: pruebaSintactico 
#	./check_tests.sh

clean:
	rm pruebaSintactico *.o lex.yy.c  y.tab.c y.tab.h y.output
#	rm -r test testOut