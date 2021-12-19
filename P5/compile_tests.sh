#!/bin/bash

for t in $(ls test*.txt); do
    num=$(echo "$t" | grep -o '[0-9]*')

     echo -e "\nCompiling test ${num}"
     ./alfa "test${num}.txt" "test${num}.asm" && \
     nasm -g -o output.o -f elf32 "test${num}.asm" && \
     gcc -Wall -g -m32 -o "test${num}" output.o alfalib.o


    if [[ $? == 0 ]]; then
      echo "Running test ${num}"
      ./"test${num}"
    else
      echo "Error in compilation"
    fi

done

