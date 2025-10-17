#!/bin/bash
# 현재 디렉토리 파일 개수 출력

count=$(ls -1 | wc -l)
echo "현재 디렉토리의 파일 개수: $count 개"

