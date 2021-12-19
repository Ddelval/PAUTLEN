#!/bin/bash

sed="sed -i"
if [[ $OSTYPE == 'darwin'* ]]; then
  sed="sed -i .back "
fi

rm testOut/*
mkdir -p testOut
mkdir -p test

test_count=$(($(ls testSrc/test*.txt | wc -w)))

for t in $(ls testSrc/test*.txt); do
  i=$(echo "$t" | grep -o '[0-9]*')
  awk -v number="$i" 'BEGIN{name[0]="entrada"; name[1]="salida"; name[2]="error";n=0}/----------?/{n++}{print >"test/"name[n] number".txt"}' testSrc/test"$i".txt
done

for file in $(ls testOut/*); do
  $sed '/^----------/d' "$file"
  $sed '/^##########/d' "$file"
done
exit 1
printf "\e[34;4mRunning $test_count tests\e[39;0m\n"

for t in $(ls testSrc/test*.txt); do
  i=$(echo "$t" | grep -o '[0-9]*')
  ./alfa "test${i}.txt" "test${i}.asm" >testOut/error${i}.txt&&
    nasm -g -o output.o -f elf32 "test${i}.asm" &&
    gcc -Wall -g -m32 -o "test${i}" output.o alfalib.o

  ./"test${i}" <test/entrada${i}.txt 1>testOut/salida${i}.txt

  out_dif=$(diff -Bb test/salida$i.txt testOut/salida$i.txt)
  err_dif=$(diff -Bb test/error$i.txt testOut/error$i.txt)
  count_dif=$(echo $out_dif $err_dif | wc -w)

  if [[ $(($count_dif)) -eq 0 ]]; then
    printf "\e[32;1mTest $i OK\e[39;0m\n"
  else
    printf "\e[31;1mTest $i ERROR\e[39;0m\n"
    printf "\e[34;4mDifferences in the errors\e[39;0m\n"
    echo $err_dif
    printf "\e[34;4mDifferences in the output\e[39;0m\n"
    echo $out_dif

  fi

done
