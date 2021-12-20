#!/bin/bash

sed="sed -i"
if [[ $OSTYPE == 'darwin'* ]]; then
  sed="sed -i .back "
fi

rm -f testOut/*
rm -f testAsm/*
rm -f testBin/*
rm -f test/*
mkdir -p testOut
mkdir -p test
mkdir -p testAsm
mkdir -p testBin

test_count=$(($(ls testSrc/test*.txt | wc -w)))

for t in $(ls testSrc/test*.txt); do
  i=$(echo "$t" | grep -o '[0-9]*')
  sections=$(cat testSrc/test"$i".txt | grep -e '----------' | wc -l)
  test_cases=$((($sections - 1) / 2))

  out_files='name[0]="entrada"; name[1]="error";'
  out_indeces='inde[0]="0"; inde[1]="0";'
  name_index=2

  for t in $(seq $((test_cases + 1))); do
    out_files="$out_files name[$name_index]=\"in\"; name[$((name_index + 1))]=\"out\";"
    out_indeces="$out_indeces inde[$name_index]=\"$t\"; inde[$((name_index + 1))]=\"$t\";"
    name_index=$((name_index + 2))
  done
  code="BEGIN{$out_indeces $out_files n=0}"'/----------?/{n++}{print >"test/test" number "-" name[n]  "-" inde[n] }'
  awk -v number="$i" "$code" testSrc/test"$i".txt
done

for file in $(ls test/*); do
  $sed '/^----------/d' "$file"
  $sed '/^##########/d' "$file"
done
printf "\e[34;4mRunning $test_count tests\e[39;0m\n"

t_n=""
for t in $(ls test/test*-entrada-0); do
  i=$(echo "$t" | cut -d "-" -f 1 - | grep -o '[0-9]*')
  t_n=" $t_n $(echo -e "\n")$i"
done
indeces=$(echo $t_n | grep -o '[0-9]*' | sort -g)

c_ok=0
r_ok=0
c_fail=0
r_fail=0
for i in $indeces; do
  t="test/test$i-entrada-0"

  ./alfa $t "testAsm/test${i}.asm" >testOut/error"${i}" 2>/dev/null &&
    nasm -g -o output.o -f elf32 "testAsm/test${i}.asm" &&
    gcc -Wall -g -m32 -o "testBin/a${i}.out" output.o alfalib.o

  test_name=$(head -n 1 testSrc/test$i.txt | cut -c11-)
  error_file=$(echo $t | sed 's/entrada/error/g')
  err_dif=$(diff -Bb $error_file testOut/error$i)
  count_dif=$(echo $err_dif | wc -w)
  if [[ $((count_dif)) -ne 0 ]]; then
    printf "\e[31;1mTest $i$test_name COMPILATION DIFF  \e[39;0m\n"
    echo $err_dif
    c_fail=$((c_fail + 1))
  else
    printf "\e[32;1mTest $i$test_name COMPILATION OK \e[39;0m\n"
    c_ok=$((c_ok + 1))
  fi

  if [[ -e "testBin/a${i}.out" ]]; then
    for tcase in $(ls test/test${i}-in-*); do
      n=$(echo "$tcase" | cut -d "-" -f 3 - | grep -o '[0-9]*')
      out_file=$(echo $tcase | sed 's/in/out/g')

      out_t_file=$(echo $out_file | sed 's/test\//testOut\//g')

      "testBin/./a${i}.out" <$tcase 1>"$out_t_file"

      out_dif=$(diff -Bb "$out_t_file" "$out_file")
      count_dif=$(echo $out_dif | wc -w)

      if [[ $(($count_dif)) -eq 0 ]]; then
        printf "\e[32;1mTest $i$test_name Case $n OK \e[39;0m\n"
        r_ok=$((r_ok + 1))
      else
        printf "\e[31;1mTest $i$test_name Case $n ERROR \e[39;0m\n"
        r_fail=$((r_fail + 1))
        printf "\e[34;4mDifferences in the output\e[39;0m\n"
        echo $out_dif
      fi
    done
  fi
  echo ""

done
echo "SUMMARY:"
echo -e "\tRuntime:     \e[32;1mok: $r_ok \t  \e[31;1merrors: $r_fail\e[39;0m"
echo -e "\tCompilation: \e[32;1mok: $c_ok \t  \e[31;1merrors: $c_fail\e[39;0m"
