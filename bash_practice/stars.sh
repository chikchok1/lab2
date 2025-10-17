#!/bin/bash
# 별(*)을 줄여가며 출력하는 스크립트

for ((i=5; i>=1; i--))   # i=5부터 1까지 감소
do
    for ((j=1; j<=i; j++))  # j는 1부터 i까지 증가
    do
        echo -n "*"    # 줄바꿈 없이 별 출력
    done
    echo ""            # 한 줄 끝나면 줄바꿈
done

