#!/bin/bash

tests=$(ls test*.c)


for t in ${tests}; do
t=$(echo $t | sed -e 's/.c//g')
printf "Running $t\n"
./bin/$t asm_gen/$t.asm
nasm -g -o asm_gen/$t.o -f elf32 asm_gen/$t.asm
gcc -Wall -g -m32 -o asm_gen/$t asm_gen/$t.o alfalib.o
done