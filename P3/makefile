#.PHONY: test
all:  pruebaSintactico
CC = gcc
pruebaSintactico: pruebaSintactico.o lex.yy.o y.tab.o
	$(CC) -g -o $@ $^

pruebaSintactico.o: pruebaSintactico.c
	$(CC) -Wall -ansi -pedantic -c -g -o $@ $^

lex.yy.o: alfa.l
	flex $^
	$(CC) -c -g -o lex.yy.o lex.yy.c

y.tab.o: alfa.y
	bison -dyv $^
	$(CC) -c -g -o y.tab.o y.tab.c
#test: pruebaSintactico 
#	./check_tests.sh

clean:
	rm pruebaSintactico *.o lex.yy.c 
#	rm -r test testOut