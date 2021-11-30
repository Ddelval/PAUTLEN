.PHONY:clean # test
all:  tablaSimbolos
CC = gcc
tablaSimbolos: tablaSimbolos.o syTable.o ht.o
	$(CC) -g  -o $@ $^

%.o: %.c
	$(CC) -Wall -c -g -o $@ $^

#test: tablaSimbolos 
#	./check_tests.sh

clean:
	rm -rfv tablaSimbolos *.o
#	rm -r test testOut