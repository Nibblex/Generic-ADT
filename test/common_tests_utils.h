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

#define ADD_u32(F, N, A, B) do { \
    for (u32 i = 0; i < N; i++) { \
        F(A, &i); \
        F(B, &i); \
    } \
} while (false)

#define ADD_i32_RAND(F, N, A, B) do { \
    int r; \
    srand((u32)time(NULL)); \
    for (u32 i = 0; i < N; i++) { \
        r = rand() % 20; \
        F(A, &r); \
        F(B, &r); \
    } \
} while (false)

#define DEBUG_char(F, A, B, C) do { \
    if (debug) { \
        printf(C); \
        F(A, (void (*)(elem_t))operator_debug_char); \
        F(B, (void (*)(elem_t))operator_debug_char); \
    } \
} while (false)

#define DEBUG_i32(F, A, B, C) do { \
    if (debug) { \
        printf(C); \
        F(A, (void (*)(elem_t))operator_debug_i32); \
        F(B, (void (*)(elem_t))operator_debug_i32); \
    } \
} while (false)

#define FREE(F, A, B, C, D) \
    F(A); F(B); F(C); F(D)

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

void* operator_copy(const int *p_value);
void operator_delete(int *p_value);
int operator_compare(const void *v1, const void *v2);
void operator_debug_i32(const int *p_value);
void operator_debug_u32(const u32 *p_value);
void operator_debug_char(const char *p_value);
void plus_op(int *v, void *user_data);

#endif
