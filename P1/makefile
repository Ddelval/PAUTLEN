CC=gcc
COMPILER_FLAGS=-Wall
TESTS=$(patsubst %.c,%,$(wildcard test*.c))
TEST_EXEC=$(foreach t, $(TESTS), bin/$(t))
MODULES=generacion
MODULES_BIN=$(foreach t, $(MODULES),bin/$(t).o)

tests: $(TEST_EXEC)
	@printf  "\e[32mTests compiled successfully\e[39;0m\n"

run_tests: $(TEST_EXEC)

$(TEST_EXEC): bin/%: %.c $(MODULES_BIN)
	mkdir -p bin 
	$(CC) -o $@ $^  $(COMPILER_FLAGS)

$(MODULES_BIN): bin/%.o: %.c
	mkdir -p bin
	$(CC) -o $@ -c $^ $(COMPILER_FLAGS)
