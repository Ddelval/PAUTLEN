#.PHONY: test
all:  alfa
CC = gcc
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

tests: alfa
	./check_tests.sh

docker_tests:
	docker exec -ti "pautlencontainer"   bash -c "cd data; make tests"


test_compilation: alfa
	./compile_tests.sh

docker_test_compilation:
	docker exec -ti "pautlencontainer"   bash -c "cd data; make test_compilation"

#testSimple: pruebaSintactico
#	./pruebaSintactico test.txt out.txt
#test: pruebaSintactico 
#	./check_tests.sh

clean:
	cp alfalib.o alfalib.v
	rm -fr alfa *.o lex.yy.c  y.tab.c y.tab.h y.output
	mv alfalib.v alfalib.o
#	rm -r test testOut