#!/bin/bash
# until문을 이용한 구구단

i=2
until [ $i -gt 9 ]   # i가 9보다 커질 때까지 반복
do
    echo "=== $i 단 ==="
    j=1
    until [ $j -gt 9 ]   # j가 9보다 커질 때까지 반복
    do
        echo "$i x $j = $((i*j))"
        ((j++))
    done
    echo ""
    ((i++))
done

