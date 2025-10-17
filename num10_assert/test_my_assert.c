/*
 * test_my_assert.c
 */

#include <stdio.h>
#include <stdlib.h>
#include "my_assert.h"

void check_value(int num)
{
    // assert()를 my_assert()로 대체
    my_assert(num >= 0 && num <= 100);
    printf("✅ check_value(): num = %d\n", num);
}

int main(int argc, char *argv[])
{
    int num;

    if (argc < 2) {
        fprintf(stderr, "Usage: %s <number: 0~100>\n", argv[0]);
        exit(1);
    }

    num = atoi(argv[1]);
    check_value(num);

    printf("프로그램이 정상적으로 종료되었습니다.\n");
    return 0;
}
