#.PHONY: test
all:  alfa
CC = gcc-11
alfa: y.tab.o lex.yy.o syTable.o tableNode.o ht.o generacion.o launcher.o alfaActions.o
	$(CC) -g -o $@ $^

%.o: %.c
	$(CC) -Wall -c -g -o $@ $^

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
	rm alfa *.o lex.yy.c  y.tab.c y.tab.h y.output
#	rm -r test testOut