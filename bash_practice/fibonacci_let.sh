#!/bin/bash
# 피보나치 수열 (let 명령어 사용)

read -p "몇 개의 피보나치 수를 출력할까요? " n

a=0
b=1
count=0

echo "피보나치 수열:"
echo "$a"
echo "$b"

while [ $count -lt $((n-2)) ]
do
    let c=a+b
    echo "$c"
    a=$b
    b=$c
    let count=count+1
done

