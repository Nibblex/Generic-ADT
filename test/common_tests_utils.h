#ifndef __COMMON_TESTS_UTILS_H__
#define __COMMON_TESTS_UTILS_H__

#ifndef TEST_SUCCESS
#define TEST_SUCCESS 0
#endif
#ifndef TEST_FAILURE
#define TEST_FAILURE 1
#endif

#include <stdio.h>
#include <stdlib.h>

typedef unsigned int u32;

////////////////////////////////////////////////////////////////////
///     COMMON TESTS FUNCTIONS
////////////////////////////////////////////////////////////////////

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

////////////////////////////////////////////////////////////////////
///     OPERATORS FOR ADT TESTS
////////////////////////////////////////////////////////////////////

void* operator_copy(const int *p_value);
void operator_delete(int *p_value);
int operator_compare(const void *v1, const void *v2);
void operator_debug(const int *p_value);
void plus_op(int *v, void *user_data);

#endif