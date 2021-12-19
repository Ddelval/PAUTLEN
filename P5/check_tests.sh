#!/bin/bash

sed="sed -i"
if [[ $OSTYPE == 'darwin'* ]]; then
  sed="sed -i .back "
fi

rm -f testOut/*
rm -f testAsm/*
rm -f test/*
mkdir -p testOut
mkdir -p test
mkdir -p testAsm

test_count=$(($(ls testSrc/test*.txt | wc -w)))

for t in $(ls testSrc/test*.txt); do
  i=$(echo "$t" | grep -o '[0-9]*')
  sections=$(cat testSrc/test"$i".txt | grep -e '----------' | wc -l)
  test_cases=$((($sections - 1) / 2))
  echo $sections
  echo $test_cases
  out_files='name[0]="entrada"; name[1]="error";'
  out_indeces='inde[0]="0"; inde[1]="0";'
  name_index=2
  for t in $(seq $(( test_cases + 1 ))); do
    out_files="$out_files name[$name_index]=\"in\"; name[$(( name_index + 1 ))]=\"out\";"
    out_indeces="$out_indeces inde[$name_index]=\"$t\"; inde[$(( name_index + 1 ))]=\"$t\";"
    name_index=$(( name_index + 2 ))
  done
  code="BEGIN{$out_indeces $out_files n=0}"'/----------?/{n++}{print >"test/test" number "_" name[n]  "_" inde[n] ".txt"}'
  echo $code
  awk -v number="$i" "$code" testSrc/test"$i".txt
done

exit 1;
for file in $(ls test/*); do
  $sed '/^----------/d' "$file"
  $sed '/^##########/d' "$file"
done
printf "\e[34;4mRunning $test_count tests\e[39;0m\n"

for t in $(ls testSrc/test*_entrada_0.txt); do
  i=$(echo "$t" | cut -d "_" -f 1 - | grep -o '[0-9]*')
  ./alfa "test/entrada${i}.txt" "testAsm/test${i}.asm" >testOut/error${i}.txt 2>/dev/null &&
    nasm -g -o output.o -f elf32 "testAsm/test${i}.asm" &&
    gcc -Wall -g -m32 -o a.out output.o alfalib.o

  err_dif=$(diff -Bb test/error$i.txt testOut/error$i.txt)

  for tcase in $(ls test/test${i}_in_*.txt); do
    n=$(echo "$tcase" | cut -d "_" -f 3 - | grep -o '[0-9]*')

    ./a.out <test/${i}.txt 1>testOut/salida${i}.txt

    out_dif=$(diff -Bb test/salida$i.txt testOut/salida$i.txt)
    count_dif=$(echo $out_dif $err_dif | wc -w)

    test_name=$(head -n 1 testSrc/test$i.txt | cut -c11-)
    if [[ $(($count_dif)) -eq 0 ]]; then
      printf "\e[32;1mTest $i OK ($test_name )\e[39;0m\n"
    else
      printf "\e[31;1mTest $i ERROR ($test_name )\e[39;0m\n"
      printf "\e[34;4mDifferences in the errors\e[39;0m\n"
      echo $err_dif
      printf "\e[34;4mDifferences in the output\e[39;0m\n"
      echo $out_dif
    fi
  done;

  ./a.out <test/stdin${i}.txt 1>testOut/salida${i}.txt

  out_dif=$(diff -Bb test/salida$i.txt testOut/salida$i.txt)
  err_dif=$(diff -Bb test/error$i.txt testOut/error$i.txt)
  count_dif=$(echo $out_dif $err_dif | wc -w)

  test_name=$(head -n 1 testSrc/test$i.txt | cut -c11-)
  if [[ $(($count_dif)) -eq 0 ]]; then
    printf "\e[32;1mTest $i OK ($test_name )\e[39;0m\n"
  else
    printf "\e[31;1mTest $i ERROR ($test_name )\e[39;0m\n"
    printf "\e[34;4mDifferences in the errors\e[39;0m\n"
    echo $err_dif
    printf "\e[34;4mDifferences in the output\e[39;0m\n"
    echo $out_dif
  fi

done
