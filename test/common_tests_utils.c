#include "common_tests_utils.h"

///////////////////////////////////////////////////////////////////////////////
///     PRINT FUNCTIONS FOR TEST SUMMARY
///////////////////////////////////////////////////////////////////////////////

void print_test_result(int result, int *nb_success, int *nb_tests)
{
    if (result == TEST_SUCCESS) {
        printf("\x1B[32mSUCCESS\x1B[0m\n");
        (*nb_success)++;
    } else {

        printf("\x1B[31mFAILED\x1B[0m\n");
    }

    (*nb_tests)++;
}

void print_test_summary(int nb_success, int nb_tests)
{
    if (nb_success == nb_tests)
        printf("TESTS SUMMARY: \t\x1B[32m%d\x1B[0m/%d\n", nb_success, nb_tests);
    else
        printf("TESTS SUMMARY: \t\x1B[31m%d\x1B[0m/%d\n", nb_success, nb_tests);
}


///////////////////////////////////////////////////////////////////////////////
///     OPERATOR FUNCTIONS FOR INT
///////////////////////////////////////////////////////////////////////////////

void *operator_copy(void *p_value)
{
    if (p_value == NULL)
        return NULL;

    int *new_int = malloc(sizeof(int));
    if (new_int == NULL) return NULL;
    *new_int = *(int*)p_value;
    return new_int;
}

void operator_delete(void *p_value)
{
    free(p_value);
}

int operator_compare(const void *v1, const void *v2)
{
    if (v1 == NULL || v2 == NULL) {
        printf("NULL value compared");
        exit(EXIT_FAILURE);
    }

    int arg1 = *(*(int **)v1);
    int arg2 = *(*(int **)v2);

    if (arg1 < arg2)
        return -1;
    else if (arg1 == arg2)
        return 0;
    else
        return 1;
}

void operator_debug_i32(const int *p_value)
{
    if (!p_value) {
        printf("NULL ");
    } else {
        printf("%d ", *p_value);
    }
}

void operator_debug_u32(const u32 *p_value)
{
    if (!p_value) {
        printf("NULL ");
    } else {
        printf("%u ", *p_value);
    }
}

void operator_debug_char(const char *p_value)
{
    if (!p_value) {
        printf("NULL ");
    } else {
        printf("%c ", *p_value);
    }
}

void plus_op(void *v, void *user_data)
{
    *(int*)v += *(int *)user_data;
}
