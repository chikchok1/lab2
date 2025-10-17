#!/bin/bash
# while문을 이용한 구구단

i=2
while [ $i -le 9 ]
do
    echo "=== $i 단 ==="
    j=1
    while [ $j -le 9 ]
    do
        echo "$i x $j = $((i*j))"
        ((j++))
    done
    echo ""
    ((i++))
done

