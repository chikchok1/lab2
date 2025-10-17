#!/bin/bash
# for문을 이용한 구구단

for ((i=2; i<=9; i++))
do
    echo "=== $i 단 ==="
    for ((j=1; j<=9; j++))
    do
        echo "$i x $j = $((i*j))"
    done
    echo ""   # 단 구분 공백 줄
done

