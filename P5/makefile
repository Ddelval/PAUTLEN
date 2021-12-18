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

tests: $(TEST_EXEC)
	@printf  "\e[32mTests compiled successfully\e[39;0m\n"

test_compilation: alfa
	./alfa test2.txt test2.asm
	nasm -g -o output.o -f elf32 test2.asm
	gcc -Wall -g -m32 -o test2 output.o alfalib.o
	-./test2

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