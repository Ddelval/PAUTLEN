LIBS=alfalib.o
.SECONDARY:
run : test2.out
	@printf  "\e[32;1mRunning program:\e[39;0m\n"
	@./$^
	@printf "\n"
%.out: %_asm.o
	gcc -o $@ $^ $(LIBS) -m32

	

%_asm.o: %.asm
	nasm -g -o $@  -f elf $^
# DO NOT DELETE
