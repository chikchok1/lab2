/*
 * my_assert.h
 * 표준 assert() 함수의 동작 원리를 직접 구현한 사용자 정의 버전
 */

#include <stdio.h>
#include <stdlib.h>

// NDEBUG가 정의되어 있으면 my_assert를 비활성화
#ifdef NDEBUG
    #define my_assert(expr) ((void)0)
#else
    #define my_assert(expr) \
        do { \
            if (!(expr)) { \
                fprintf(stderr, \
                    "[ASSERT FAIL]\n" \
                    "  ▶ Expression : '%s'\n" \
                    "  ▶ File       : %s\n" \
                    "  ▶ Function   : %s\n" \
                    "  ▶ Line       : %d\n", \
                    #expr, __FILE__, __func__, __LINE__); \
                abort(); \
            } \
        } while (0)
#endif
