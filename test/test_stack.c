#include "common_tests_utils.h"
#include "../stack/stack.h"
#include "../common/defs.h"

#define STACK_CREATE(A, B) \
    Stack A = stack__empty_copy_enabled((copy_operator_t)operator_copy, (delete_operator_t)operator_delete); \
    Stack B = stack__empty_copy_disabled();

#define STACK_PUSH_u32(M, A, B) \
    ADD_u32(stack__push, M, A, B)

#define STACK_PUSH_char(M, A, B) \
    ADD_char(stack__push, M, A, B)

#define STACK_DEBUG_char(A, B, C) \
    DEBUG_char(stack__debug, A, B, C)

#define STACK_DEBUG_i32(A, B, C) \
    DEBUG_i32(stack__debug, A, B, C)

#define STACK_FREE(A, B, C, D) \
    FREE(stack__free, A, B, C, D)

////////////////////////////////////////////////////////////////////
///     TEST SUITE
////////////////////////////////////////////////////////////////////

static bool test_stack__empty_copy_disabled(char debug)
{
    printf("%s... ", __func__);

    bool result;
    Stack s = stack__empty_copy_disabled();

    result = s ? TEST_SUCCESS : TEST_FAILURE;

    if (debug) stack__debug(s, (void (*)(elem_t))operator_debug_i32);

    STACK_FREE(s, NULL, NULL, NULL)
    return result;
}

static bool test_stack__empty_copy_enabled(char debug)
{
    printf("%s... ", __func__);

    bool result;
    Stack s = stack__empty_copy_enabled((copy_operator_t)operator_copy, (delete_operator_t)operator_delete);

    result = s ? TEST_SUCCESS : TEST_FAILURE;

    if (debug) stack__debug(s, (void (*)(elem_t))operator_debug_i32);

    STACK_FREE(s, NULL, NULL, NULL)
    return result;
}

static bool test_stack__is_copy_enabled(void)
{
    printf("%s... ", __func__);

    bool result;
    STACK_CREATE(s, t)

    result = (stack__is_copy_enabled(s) && !stack__is_copy_enabled(t)) ? TEST_SUCCESS : TEST_FAILURE;

    STACK_FREE(s, t, NULL, NULL)
    return result;
}

static bool test_stack__size(void)
{
    printf("%s... ", __func__);

    bool result;
    STACK_CREATE(s, t)

    u32 N = 5;
    STACK_PUSH_u32(N, s, t)

    result = (stack__size(s) == N && stack__size(t) == N) ? TEST_SUCCESS : TEST_FAILURE;

    STACK_FREE(s, t, NULL, NULL)
    return result;
}

static bool test_stack__is_empty_on_empty_stack(void)
{
    printf("%s... ", __func__);

    bool result;
    STACK_CREATE(s, t)

    result = (stack__is_empty(s) && stack__is_empty(t)) ? TEST_SUCCESS : TEST_FAILURE;

    STACK_FREE(s, t, NULL, NULL)
    return result;
}

static bool test_stack__is_empty_on_non_empty_stack(void)
{
    printf("%s... ", __func__);

    bool result;
    STACK_CREATE(s, t)

    STACK_PUSH_u32(1, s, t)

    result = (!stack__is_empty(s) && !stack__is_empty(t)) ? TEST_SUCCESS : TEST_FAILURE;

    STACK_FREE(s, t, NULL, NULL)
    return result;
}

static bool test_stack__push_on_empty_stack(char debug)
{
    printf("%s... ", __func__);

    bool result;
    STACK_CREATE(s, t)

    result = (!stack__push(s, NULL)
           && !stack__push(t, NULL)
           && stack__size(s) == 1
           && stack__size(t) == 1) ? TEST_SUCCESS : TEST_FAILURE;

    STACK_DEBUG_i32(s, t, "\n\tStacks after push:")

    STACK_FREE(s, t, NULL, NULL)
    return result;
}

static bool test_stack__push_on_non_empty_stack(char debug)
{
    printf("%s... ", __func__);

    bool result;
    STACK_CREATE(s, t)

    u32 N = 5;
    STACK_PUSH_u32(N, s, t)

    result = (stack__size(s) == N && stack__size(t) == N) ? TEST_SUCCESS : TEST_FAILURE;

    STACK_DEBUG_i32(s, t, "\n\tStacks after push:")

    STACK_FREE(s, t, NULL, NULL)
    return result;
}

static bool test_stack__pop_on_empty_stack(char debug)
{
    printf("%s... ", __func__);

    bool result;
    STACK_CREATE(s, t)

    result = (stack__pop(s, NULL) == -1
           && stack__pop(t, NULL) == -1
           && stack__is_empty(s)
           && stack__is_empty(t)) ? TEST_SUCCESS : TEST_FAILURE;

    STACK_DEBUG_i32(s, t, "\n\tStacks after pop:")

    STACK_FREE(s, t, NULL, NULL)
    return result;
}

static bool test_stack__pop_on_non_empty_stack(char debug)
{
    printf("%s... ", __func__);

    bool result;
    elem_t top_s = NULL;
    elem_t top_t = NULL;
    STACK_CREATE(s, t)

    u32 N = 5;
    STACK_PUSH_u32(N, s, t)

    STACK_DEBUG_i32(s, t, "\n\tStacks before pop:")

    result = TEST_SUCCESS;
    for (u32 i = 0; i < N; i++) {
        result |= stack__pop(s, &top_s)
               || stack__pop(t, &top_t)
               || stack__size(s) != N-i-1
               || stack__size(t) != N-i-1
               || *(u32 *)top_s != N-i-1
               || *(u32 *)top_t != N;
        free(top_s);
    }

    STACK_DEBUG_i32(s, t, "\n\tStacks after pop:")

    STACK_FREE(s, t, NULL, NULL)
    return result;
}

static bool test_stack__push_and_pop_on_multiple_elements(char debug)
{
    printf("%s... ", __func__);

    bool result;
    STACK_CREATE(s, t)

    STACK_PUSH_u32(60, s, t)

    result = (stack__size(s) == 60 && stack__size(t) == 60) ? TEST_SUCCESS : TEST_FAILURE;

    STACK_DEBUG_i32(s, t, "\nAdded 60 elements, expected capacity of 64 and size of 60\n")

    for (u32 i = 0; i < 40; i++) {
        stack__pop(s, NULL);
        stack__pop(t, NULL);
    }

    result |= stack__size(s) != 20 || stack__size(t) != 20;

    STACK_DEBUG_i32(s, t, "\nRemoved 40 elements, expected capacity of 32 and size of 20\n")

    STACK_PUSH_u32(20, s, t)

    result |= stack__size(s) != 40 || stack__size(t) != 40;

    STACK_DEBUG_i32(s, t, "\nAdded 20 elements, expected capacity of 64 and size of 40\n")

    STACK_FREE(s, t, NULL, NULL)
    return result;
}

static bool test_stack__clean_NULL_on_empty_stack(char debug)
{
    printf("%s... ", __func__);

    bool result;
    STACK_CREATE(s, t)

    stack__clean_NULL(s);
    stack__clean_NULL(t);

    result = (stack__is_empty(s) && stack__is_empty(t)) ? TEST_SUCCESS : TEST_FAILURE;

    STACK_DEBUG_i32(s, t, "\n\tStacks after clean_NULL:")

    STACK_FREE(s, t, NULL, NULL)
    return result;
}

static bool test_stack__clean_NULL_on_non_empty_stack(char debug)
{
    printf("%s... ", __func__);

    bool result;
    STACK_CREATE(s, t)

    u32 N = 8;
    for (u32 i = 0; i < N; i++) {
        stack__push(s, !(i%2) ? &i : NULL);
        stack__push(t, !(i%2) ? &i : NULL);
    }

    STACK_DEBUG_i32(s, t, "\n\tStacks before clean_NULL:")

    stack__clean_NULL(s);
    stack__clean_NULL(t);

    result = (stack__size(s) == N>>1 && stack__size(t) == N>>1) ? TEST_SUCCESS : TEST_FAILURE;

    STACK_DEBUG_i32(s, t, "\n\tStacks after clean_NULL:")

    STACK_FREE(s, t, NULL, NULL)
    return result;
}

static bool test_stack__clear_on_empty_stack(char debug)
{
    printf("%s... ", __func__);

    bool result;
    STACK_CREATE(s, t)

    stack__clear(s);
    stack__clear(t);

    result = (stack__is_empty(s) && stack__is_empty(t)) ? TEST_SUCCESS : TEST_FAILURE;

    STACK_DEBUG_i32(s, t, "\n\tStacks after clear:")

    STACK_FREE(s, t, NULL, NULL)
    return result;
}

static bool test_stack__clear_on_non_empty_stack(char debug)
{
    printf("%s... ", __func__);

    bool result;
    STACK_CREATE(s, t)

    STACK_PUSH_u32(5, s, t)

    STACK_DEBUG_i32(s, t, "\n\tStacks before clear:")

    stack__clear(s);
    stack__clear(t);

    result = (stack__is_empty(s) && stack__is_empty(t)) ? TEST_SUCCESS : TEST_FAILURE;

    STACK_DEBUG_i32(s, t, "\n\tStacks after clear:")

    STACK_FREE(s, t, NULL, NULL)
    return result;
}

static bool test_stack__top_on_empty_stack(char debug)
{
    printf("%s... ", __func__);

    bool result;
    STACK_CREATE(s, t)

    result = (stack__top(s, NULL)
           && stack__top(t, NULL)
           && stack__is_empty(s)
           && stack__is_empty(t)) ? TEST_SUCCESS : TEST_FAILURE;

    STACK_DEBUG_i32(s, t, "\n\tStacks after top:")

    STACK_FREE(s, t, NULL, NULL)
    return result;
}

static bool test_stack__top_on_non_empty_stack(char debug)
{
    printf("%s... ", __func__);

    bool result;
    int value1 = 5;
    int value2 = 7;
    elem_t top_s = NULL;
    elem_t top_t = NULL;
    STACK_CREATE(s, t)

    stack__push(s, &value1);
    stack__push(t, &value1);
    stack__push(s, &value2);
    stack__push(t, &value2);

    STACK_DEBUG_i32(s, t, "\n\tStacks before top:")

    result = (!stack__top(s, &top_s)
           && !stack__top(t, &top_t)
           && !stack__is_empty(s)
           && !stack__is_empty(t)
           && *(int *)top_s == 7
           && *(int *)top_t == 7) ? TEST_SUCCESS : TEST_FAILURE;

    STACK_DEBUG_i32(s, t, "\n\tStacks after top:")

    free(top_s);
    STACK_FREE(s, t, NULL, NULL)
    return result;
}

static int test_stack__from_array(char debug)
{
    printf("%s... ", __func__);

    bool result;
    char A[5] = {'a', 'b', 'c', 'd', 'e'};
    char B[5] = {'f', 'g', 'h', 'i', 'j'};
    int C[5] = {1, 2, 3, 4, 5};
    elem_t *D = NULL;
    STACK_CREATE(s_char, t_char)
    STACK_CREATE(s_int, t_int)

    result = (stack__from_array(s_char, A, 5, sizeof(char))
           && (t_char = stack__from_array(t_char, A, 5, sizeof(char)))
           && stack__from_array(s_char, B, 5, sizeof(char))
           && stack__from_array(t_char, B, 5, sizeof(char))
           && stack__from_array(s_int, C, 5, sizeof(int))
           && (t_int = stack__from_array(t_int, C, 5, sizeof(int)))
           && stack__size(s_char) == 10
           && stack__size(t_char) == 10
           && stack__size(s_int) == 5
           && stack__size(t_int) == 5) ? TEST_SUCCESS : TEST_FAILURE;

    D = stack__to_array(t_int);

    for (u32 i = 0; i < 5; i++) {
        result |= C[i] != *(int *)D[i];
    }

    STACK_DEBUG_char(s_char, t_char, "\n\tStacks after from_array:")
    STACK_DEBUG_i32(s_int, t_int, " ")

    free(D);
    STACK_FREE(s_char, t_char, s_int, t_int)
    return result;
}

static bool test_stack__to_array_on_empty_stack(char debug)
{
    printf("%s... ", __func__);

    bool result;
    STACK_CREATE(s, t)

    result = (!stack__to_array(s)
           && !stack__to_array(t)
           && stack__is_empty(s)
           && stack__is_empty(t)) ? TEST_SUCCESS : TEST_FAILURE;

    STACK_DEBUG_i32(s, t, "\n\tStacks after to_array:")

    STACK_FREE(s, t, NULL, NULL)
    return result;
}

static bool test_stack__to_array_on_non_empty_stack(char debug)
{
    printf("%s... ", __func__);

    bool result;
    int **A = NULL;
    int **B = NULL;
    STACK_CREATE(s, t)

    u32 N = 5;
    STACK_PUSH_u32(N, s, t)

    STACK_DEBUG_i32(s, t, "\n\tStacks before to_array:")

    A = (int **)stack__to_array(s);
    B = (int **)stack__to_array(t);

    result = (stack__size(s) == N && stack__size(t) == N) ? TEST_SUCCESS : TEST_FAILURE;

    for (u32 i = 0; i < N; i++) {
        result |= *A[i] != (int)i || *B[i] != (int)N;
    }

    STACK_DEBUG_i32(s, t, "\n\tStacks after to_array:")

    for (u32 i = 0; i < N; i++) {
        free(A[i]);
    }
    free(A);
    free(B);
    STACK_FREE(s, t, NULL, NULL)
    return result;
}

static bool test_stack__foreach_on_empty_stack(char debug)
{
    printf("%s... ", __func__);

    bool result;
    int value = 1;
    STACK_CREATE(s, t)

    stack__foreach(s, (applying_func_t) plus_op, &value);
    stack__foreach(t, (applying_func_t) plus_op, &value);

    result = (stack__is_empty(s) && stack__is_empty(t)) ? TEST_SUCCESS : TEST_FAILURE;

    STACK_DEBUG_i32(s, t, "\n\tStacks after foreach:")

    STACK_FREE(s, t, NULL, NULL)
    return result;
}

static bool test_stack__foreach_on_non_empty_stack(char debug)
{
    printf("%s... ", __func__);

    bool result;
    int value = 1;
    int array[8] = {-5, 0, 2, 7, 11, 1, 3, 10};
    elem_t *A = NULL, *B = NULL, *C = NULL, *D = NULL;
    STACK_CREATE(s, t)

    u32 N = 8;
    stack__from_array(s, array, N, sizeof(int));
    stack__from_array(t, array, N, sizeof(int));

    A = stack__to_array(s);
    B = stack__to_array(t);

    STACK_DEBUG_i32(s, t, "\n\tStacks before foreach:")

    stack__foreach(s, (applying_func_t) plus_op, &value);
    stack__foreach(t, (applying_func_t) plus_op, &value);

    C = stack__to_array(s);
    D = stack__to_array(t);

    result = TEST_SUCCESS;
    for (u32 i = 0; i < N; i++) {
        result |= *(int *)A[i] + value != *(int *)C[i] || *(int *)B[i] != *(int *)D[i];
    }

    STACK_DEBUG_i32(s, t, "\n\tStacks after foreach:")

    for (u32 i = 0; i < N; i++) {
        free(A[i]);
        free(C[i]);
    }
    free(A);
    free(B);
    free(C);
    free(D);
    STACK_FREE(s, t, NULL, NULL)
    return result;
}

static bool test_stack__sort_on_empty_stack(char debug)
{
    printf("%s... ", __func__);

    bool result;
    STACK_CREATE(s, t)

    stack__sort(s, operator_compare);
    stack__sort(t, operator_compare);

    result = (stack__is_empty(s) && stack__is_empty(t)) ? TEST_SUCCESS : TEST_FAILURE;

    STACK_DEBUG_i32(s, t, "\n\tStacks after sort:")

    STACK_FREE(s, t, NULL, NULL)
    return result;
}

static bool test_stack__sort_on_non_empty_stack(char debug)
{
    printf("%s... ", __func__);

    bool result;
    int unordered[8] = {5, -3, 2, 0, 1, 0, 7, 4};
    int ordered[8] = {-3, 0, 0, 1, 2, 4, 5, 7};
    elem_t *A = NULL;
    elem_t *B = NULL;
    STACK_CREATE(s, t)

    u32 N = 8;
    stack__from_array(s, unordered, N, sizeof(int));
    stack__from_array(t, unordered, N, sizeof(int));

    STACK_DEBUG_i32(s, t, "\n\tStacks before sort:")

    stack__sort(s, operator_compare);
    stack__sort(t, operator_compare);

    A = stack__to_array(s);
    B = stack__to_array(t);

    result = TEST_SUCCESS;
    for (u32 i = 0; i < N; i++) {
        result |= ordered[i] != *(int *)A[i] || ordered[i] != *(int *)B[i];
    }

    STACK_DEBUG_i32(s, t, "\n\tStacks after sort:")

    for (u32 i = 0; i < N; i++) {
        free(A[i]);
    }
    free(A);
    free(B);
    STACK_FREE(s, t, NULL, NULL)
    return result;
}

static bool test_stack__shuffle_on_empty_stack(char debug)
{
    printf("%s... ", __func__);

    bool result;
    STACK_CREATE(s, t)

    stack__shuffle(s);
    stack__shuffle(t);

    result = (stack__is_empty(s) && stack__is_empty(t)) ? TEST_SUCCESS : TEST_FAILURE;

    STACK_DEBUG_i32(s, t, "\n\tStacks after shuffle:")

    STACK_FREE(s, t, NULL, NULL)
    return result;
}

static int test_stack__shuffle_on_non_empty_stack(char debug)
{
    printf("%s... ", __func__);

    bool result;
    STACK_CREATE(s_char, t_char)
    STACK_CREATE(s_int, t_int)

    STACK_PUSH_char('i', s_char, t_char)
    STACK_PUSH_u32(8, s_int, t_int)

    STACK_DEBUG_char(s_char, t_char, "\n\tStacks before shuffle:")
    STACK_DEBUG_i32(s_int, t_int, " ")

    stack__shuffle(s_char);
    stack__shuffle(t_char);
    stack__shuffle(s_int);
    stack__shuffle(t_int);

    result = TEST_SUCCESS;

    STACK_DEBUG_char(s_char, t_char, "\n\tStacks after shuffle:")
    STACK_DEBUG_i32(s_int, t_int, " ")

    STACK_FREE(s_char, t_char, s_int, t_int)
    return result;
}


int main(void)
{
    int nb_success = 0;
    int nb_tests = 0;
    printf("----------- TEST STACK -----------\n");

    print_test_result(test_stack__empty_copy_disabled(false), &nb_success, &nb_tests);
    print_test_result(test_stack__empty_copy_enabled(false), &nb_success, &nb_tests);
    print_test_result(test_stack__is_copy_enabled(), &nb_success, &nb_tests);
    print_test_result(test_stack__size(), &nb_success, &nb_tests);
    print_test_result(test_stack__is_empty_on_empty_stack(), &nb_success, &nb_tests);
    print_test_result(test_stack__is_empty_on_non_empty_stack(), &nb_success, &nb_tests);

    print_test_result(test_stack__push_on_empty_stack(false), &nb_success, &nb_tests);
    print_test_result(test_stack__push_on_non_empty_stack(false), &nb_success, &nb_tests);
    print_test_result(test_stack__pop_on_empty_stack(false), &nb_success, &nb_tests);
    print_test_result(test_stack__pop_on_non_empty_stack(false), &nb_success, &nb_tests);
    print_test_result(test_stack__push_and_pop_on_multiple_elements(false), &nb_success, &nb_tests);

    print_test_result(test_stack__clean_NULL_on_empty_stack(false), &nb_success, &nb_tests);
    print_test_result(test_stack__clean_NULL_on_non_empty_stack(false), &nb_success, &nb_tests);
    print_test_result(test_stack__clear_on_empty_stack(false), &nb_success, &nb_tests);
    print_test_result(test_stack__clear_on_non_empty_stack(false), &nb_success, &nb_tests);
    print_test_result(test_stack__top_on_empty_stack(false), &nb_success, &nb_tests);
    print_test_result(test_stack__top_on_non_empty_stack(false), &nb_success, &nb_tests);

    print_test_result(test_stack__from_array(false), &nb_success, &nb_tests);
    print_test_result(test_stack__to_array_on_empty_stack(false), &nb_success, &nb_tests);
    print_test_result(test_stack__to_array_on_non_empty_stack(false), &nb_success, &nb_tests);
    print_test_result(test_stack__foreach_on_empty_stack(false), &nb_success, &nb_tests);
    print_test_result(test_stack__foreach_on_non_empty_stack(false), &nb_success, &nb_tests);
    print_test_result(test_stack__sort_on_empty_stack(false), &nb_success, &nb_tests);
    print_test_result(test_stack__sort_on_non_empty_stack(false), &nb_success, &nb_tests);
    print_test_result(test_stack__shuffle_on_empty_stack(false), &nb_success, &nb_tests);
    print_test_result(test_stack__shuffle_on_non_empty_stack(false), &nb_success, &nb_tests);

    print_test_summary(nb_success, nb_tests);

    return TEST_SUCCESS;
}
