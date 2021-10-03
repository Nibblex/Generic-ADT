#ifndef __COMMON_TESTS_UTILS_H__
#define __COMMON_TESTS_UTILS_H__

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#ifndef TEST_SUCCESS
#define TEST_SUCCESS 1
#endif
#ifndef TEST_FAILURE
#define TEST_FAILURE 0
#endif

#define TEST_FROM_ARRAY(F, N, ARR, A, B) \
    for (u32 i = 0; i < N; i++) { \
        F(A, &ARR[i]); \
        F(B, &ARR[i]); \
    }

#define DEBUG_char(F, A, B, C) do { \
    if (debug) { \
        printf(C); \
        F(A, (void (*)(elem_t))operator_debug_char); \
        F(B, (void (*)(elem_t))operator_debug_char); \
    } \
} while (false)

#define DEBUG_u32(F, A, B, C) do { \
    if (debug) { \
        printf(C); \
        F(A, (void (*)(elem_t))operator_debug_u32); \
        F(B, (void (*)(elem_t))operator_debug_u32); \
    } \
} while (false)

#define FREE(F, A, B, C, D) \
    F(A); F(B); F(C); F(D)

#define COMPARE(F, N, A, B, COPY_EN) \
({ \
    int __result = true; \
    elem_t elem_A, elem_B; \
    for (u32 i = 0; i < N; i++) { \
        F(A, i, &elem_A); \
        F(B, i, &elem_B); \
        __result &= *(u32*)elem_A == *(u32*)elem_B; \
        if (COPY_EN) { \
            free(elem_A); \
            free(elem_B); \
        } \
    } \
    __result; \
})

#define COMPARE2(F, N, ARR, A, COPY_EN) \
({ \
    int __result = true; \
    elem_t elem_A; \
    for (u32 i = 0; i < N; i++) { \
        F(A, i, &elem_A); \
        __result &= *(u32*)elem_A == ARR[i]; \
        if (COPY_EN) { \
            free(elem_A); \
        } \
    } \
    __result; \
})

#define COMPARE3(F, N, A, VAL, COPY_EN) \
({ \
    int __result = true; \
    elem_t elem_A; \
    for (u32 i = 0; i < N; i++) { \
        F(A, i, &elem_A); \
        __result &= *(u32*)elem_A == i + VAL; \
        if (COPY_EN) { \
            free(elem_A); \
        } \
    } \
    __result; \
})

#define IS_REVERSE(F, N, A, COPY_EN) \
({ \
    int __result = true; \
    elem_t elem_A; \
    for (u32 i = 0; i < N; i++) { \
        F(A, i, &elem_A); \
        __result &= *(u32*)elem_A == N-i-1; \
        if (COPY_EN) { \
            free(elem_A); \
        } \
    } \
    __result; \
})

#define IS_SORTED(F, N, A, COPY_EN) \
({ \
    int __result = true; \
    elem_t elem_A, elem_B; \
    for (u32 i = 0; i + 1 < N; i++) { \
        F(A, i, &elem_A); \
        F(A, i+1, &elem_B); \
        __result &= *(u32*)elem_A <= *(u32*)elem_B; \
        if (COPY_EN) { \
            free(elem_A); \
            free(elem_B); \
        } \
    } \
    __result; \
})

typedef unsigned int u32;

///////////////////////////////////////////////////////////////////////////////
///     COMMON TESTS FUNCTIONS
///////////////////////////////////////////////////////////////////////////////

/**
 * @brief Print if a test succeeded or not, in color
 * @param result the test result
 * @param nb_success the number of test successes
 * @param nb_tests the number of test executed
 */
void print_test_result(int result, int *nb_success, int *nb_tests);

/**
 * @brief Print tests results
 * @param nb_success number of successes
 * @param nb_tests number of tests
 */
void print_test_summary(int nb_success, int nb_tests);

///////////////////////////////////////////////////////////////////////////////
///     OPERATORS FOR ADT TESTS
///////////////////////////////////////////////////////////////////////////////

void* operator_copy(void *p_value);
void operator_delete(void *p_value);
int operator_compare(const void *v1, const void *v2);
void operator_debug_i32(const int *p_value);
void operator_debug_u32(const u32 *p_value);
void operator_debug_char(const char *p_value);
void plus_op(void *v, void *user_data);
char predicate(void *v, void *user_data);

#endif
