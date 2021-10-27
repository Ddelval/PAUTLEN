
mkdir -p testOut
mkdir -p test
test_count=$(( $(ls testSrc/test*.txt|wc -w) ))


for ((i=1; i<=$test_count; i++)); do
awk -v number="$i" 'BEGIN{name[0]="entrada"; name[1]="salida"; name[2]="error";n=0}/----------?/{n++}{print >"test/"name[n] number".txt"}' testSrc/test$i.txt
done

for file in $(ls test/*);do
    sed -i '/^----------/d' "$file"
    sed -i '/^##########/d' "$file"
done

printf  "\e[34;4mRunning $test_count tests\e[39;0m\n"

for ((i=1; i<=$test_count; i++)); do
./pruebaMorfo test/entrada${i}.txt testOut/salida${i}.txt 2>testOut/error${i}.txt
out_dif=$(diff -Bb test/salida$i.txt testOut/salida$i.txt)
err_dif=$(diff -Bb test/error$i.txt testOut/error$i.txt)
count_dif=$(echo $out_dif $err_dif |wc -w)

if [[ $(( $count_dif )) -eq 0 ]]; then
printf  "\e[32;1mTest $i OK\e[39;0m\n"
else
printf  "\e[31;1mTest $i ERROR\e[39;0m\n"
printf  "\e[34;4mDifferences in the errors\e[39;0m\n"
echo $err_dif
printf  "\e[34;4mDifferences in the output\e[39;0m\n"
echo $out_dif

fi

done
