#!/bin/bash

for t in $(ls test*.txt); do
    echo $t
    num=$(echo $t | grep -oE -e '\d' -f -)
    echo ${num}
    echo "test${num}.txt"

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

