.PHONY: test
all:  pruebaSintactico

pruebaSintactico: pruebaSintactico.o lex.yy.o y.tab.o
	gcc -g -o $@ $^

pruebaSintactico.o: pruebaSintactico.c
	gcc -Wall -ansi -pedantic -c -g -o $@ $^

lex.yy.o: alfa.l
	flex $^
	gcc -c -g -o lex.yy.o lex.yy.c

y.tab.o: alfa.y
	bison -dyv $^
	gcc -c -g -o y.tab.o y.tab.c
#test: pruebaSintactico 
#	./check_tests.sh

clean:
	rm pruebaSintactico *.o lex.yy.c 
#	rm -r test testOut