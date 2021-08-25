#include "common_tests_utils.h"
#include "../stack/stack.h"
#include "../common/defs.h"

#define DEBUG_char(A, B, C) \
    if (debug) { \
        printf(C); \
        stack__debug(A, (void (*)(elem_t))operator_debug_char); \
        stack__debug(B, (void (*)(elem_t))operator_debug_char); \
    }

#define DEBUG_i32(A, B, C) \
    if (debug) { \
        printf(C); \
        stack__debug(A, (void (*)(elem_t))operator_debug_i32); \
        stack__debug(B, (void (*)(elem_t))operator_debug_i32); \
    }

////////////////////////////////////////////////////////////////////
///     TEST SUITE
////////////////////////////////////////////////////////////////////

static char test_stack__empty_copy_enabled(char debug)
{
    printf("%s... ", __func__);

    char result;
    Stack s = stack__empty_copy_enabled((copy_operator_t)operator_copy, (delete_operator_t)operator_delete);

    if (debug) stack__debug(s, (void (*)(elem_t))operator_debug_i32);

    result = s ? TEST_SUCCESS : TEST_FAILURE;

    stack__free(s);
    return result;
}

static char test_stack__empty_copy_disabled(char debug)
{
    printf("%s... ", __func__);

    char result;
    Stack s = stack__empty_copy_disabled();

    if (debug) stack__debug(s, (void (*)(elem_t))operator_debug_i32);

    result = s ? TEST_SUCCESS : TEST_FAILURE;

    stack__free(s);
    return result;
}

static char test_stack__is_copy_enabled(void)
{
    printf("%s... ", __func__);

    char result;
    Stack s = stack__empty_copy_enabled((copy_operator_t)operator_copy, (delete_operator_t)operator_delete);
    Stack t = stack__empty_copy_disabled();

    result = (stack__is_copy_enabled(s) && !stack__is_copy_enabled(t)) ? TEST_SUCCESS : TEST_FAILURE;

    stack__free(s);
    stack__free(t);
    return result;
}

static char test_stack__size(void)
{
    printf("%s... ", __func__);

    char result;
    Stack s = stack__empty_copy_enabled((copy_operator_t)operator_copy, (delete_operator_t)operator_delete);
    Stack t = stack__empty_copy_disabled();

    stack__push(s, NULL);
    stack__push(s, NULL);
    stack__push(t, NULL);
    stack__push(t, NULL);

    result = (stack__size(s) == 2 && stack__size(t) == 2) ? TEST_SUCCESS : TEST_FAILURE;

    stack__free(s);
    stack__free(t);
    return result;
}

static char test_stack__is_empty_on_empty_stack(void)
{
    printf("%s... ", __func__);

    char result;
    Stack s = stack__empty_copy_enabled((copy_operator_t)operator_copy, (delete_operator_t)operator_delete);
    Stack t = stack__empty_copy_disabled();

    result = (stack__is_empty(s) && stack__is_empty(t)) ? TEST_SUCCESS : TEST_FAILURE;

    stack__free(s);
    stack__free(t);
    return result;
}

static char test_stack__is_empty_on_non_empty_stack(void)
{
    printf("%s... ", __func__);

    char result;
    Stack s = stack__empty_copy_enabled((copy_operator_t)operator_copy, (delete_operator_t)operator_delete);
    Stack t = stack__empty_copy_disabled();

    stack__push(s, NULL);
    stack__push(t, NULL);

    result = (!stack__is_empty(s) && !stack__is_empty(t)) ? TEST_SUCCESS : TEST_FAILURE;

    stack__free(s);
    stack__free(t);
    return result;
}

static char test_stack__push_on_empty_stack(char debug)
{
    printf("%s... ", __func__);

    char result;
    Stack s = stack__empty_copy_enabled((copy_operator_t)operator_copy, (delete_operator_t)operator_delete);
    Stack t = stack__empty_copy_disabled();

    result = (!stack__push(s, NULL)
           && !stack__push(t, NULL)
           && stack__size(s) == 1
           && stack__size(t) == 1) ? TEST_SUCCESS : TEST_FAILURE;

    DEBUG_i32(s, t, "\n\tStacks after push:")

    stack__free(s);
    stack__free(t);
    return result;
}

static char test_stack__push_on_non_empty_stack(char debug)
{
    printf("%s... ", __func__);

    char result;
    u32 N = 5;
    Stack s = stack__empty_copy_enabled((copy_operator_t)operator_copy, (delete_operator_t)operator_delete);
    Stack t = stack__empty_copy_disabled();

    for (u32 i = 0; i < N; i++) {
        stack__push(s, &i);
        stack__push(t, &i);
    }

    result = (stack__size(s) == N && stack__size(t) == N) ? TEST_SUCCESS : TEST_FAILURE;

    DEBUG_i32(s, t, "\n\tStacks after push:")

    stack__free(s);
    stack__free(t);
    return result;
}

static char test_stack__pop_on_empty_stack(char debug)
{
    printf("%s... ", __func__);

    char result;
    Stack s = stack__empty_copy_enabled((copy_operator_t)operator_copy, (delete_operator_t)operator_delete);
    Stack t = stack__empty_copy_disabled();

    result = (stack__pop(s, NULL)
           && stack__pop(t, NULL)
           && stack__is_empty(s)
           && stack__is_empty(t)) ? TEST_SUCCESS : TEST_FAILURE;

    DEBUG_i32(s, t, "\n\tStacks after pop:")

    stack__free(s);
    stack__free(t);
    return result;
}

static char test_stack__pop_on_non_empty_stack(char debug)
{
    printf("%s... ", __func__);

    char result;
    u32 N = 5;
    elem_t top_s = NULL;
    elem_t top_t = NULL;
    Stack s = stack__empty_copy_enabled((copy_operator_t)operator_copy, (delete_operator_t)operator_delete);
    Stack t = stack__empty_copy_disabled();

    for (u32 i = 0; i < N; i++) {
        stack__push(s, &i);
        stack__push(t, &i);
    }

    DEBUG_i32(s, t, "\n\tStacks before pop:")

    result = TEST_SUCCESS;
    for (u32 i = 0; i < N; i++) {
        result |= (char)(stack__pop(s, &top_s)
                      || stack__pop(t, &top_t)
                      || stack__size(s) != N-i-1
                      || stack__size(t) != N-i-1
                      || *(u32 *)top_s != N-i-1
                      || *(u32 *)top_t != N);
        free(top_s);
    }

    DEBUG_i32(s, t, "\n\tStacks after pop:")

    stack__free(s);
    stack__free(t);
    return result;
}

static char test_stack__push_and_pop_on_multiple_elements(char debug)
{
    printf("%s... ", __func__);

    char result;
    Stack s = stack__empty_copy_enabled((copy_operator_t)operator_copy, (delete_operator_t)operator_delete);
    Stack t = stack__empty_copy_disabled();

    for (u32 i = 0; i < 60; i++) {
        stack__push(s, &i);
        stack__push(t, &i);
    }

    result = (stack__size(s) == 60 && stack__size(t) == 60) ? TEST_SUCCESS : TEST_FAILURE;

    DEBUG_i32(s, t, "\nAdded 60 elements, expected capacity of 64 and size of 60\n")

    for (u32 i = 0; i < 40; i++) {
        stack__pop(s, NULL);
        stack__pop(t, NULL);
    }

    result |= (char)(stack__size(s) != 20 || stack__size(t) != 20);

    DEBUG_i32(s, t, "\nRemoved 40 elements, expected capacity of 32 and size of 20\n")

    for (u32 i = 0; i < 20; i++) {
        stack__push(s, &i);
        stack__push(t, &i);
    }

    result |= (char)(stack__size(s) != 40 || stack__size(t) != 40);

    DEBUG_i32(s, t, "\nAdded 20 elements, expected capacity of 64 and size of 40\n")

    stack__free(s);
    stack__free(t);
    return result;
}

static char test_stack__clean_NULL_on_empty_stack(char debug)
{
    printf("%s... ", __func__);

    char result;
    Stack s = stack__empty_copy_enabled((copy_operator_t)operator_copy, (delete_operator_t)operator_delete);
    Stack t = stack__empty_copy_disabled();

    stack__clean_NULL(s);
    stack__clean_NULL(t);

    result = (stack__is_empty(s) && stack__is_empty(t)) ? TEST_SUCCESS : TEST_FAILURE;

    DEBUG_i32(s, t, "\n\tStacks after clean_NULL:")

    stack__free(s);
    stack__free(t);
    return result;
}

static char test_stack__clean_NULL_on_non_empty_stack(char debug)
{
    printf("%s... ", __func__);

    char result;
    u32 N = 8;
    Stack s = stack__empty_copy_enabled((copy_operator_t)operator_copy, (delete_operator_t)operator_delete);
    Stack t = stack__empty_copy_disabled();

    for (u32 i = 0; i < N; i++) {
        stack__push(s, !(i%2) ? &i : NULL);
        stack__push(t, !(i%2) ? &i : NULL);
    }

    DEBUG_i32(s, t, "\n\tStacks before clean_NULL:")

    stack__clean_NULL(s);
    stack__clean_NULL(t);

    result = (stack__size(s) == N>>1 && stack__size(t) == N>>1) ? TEST_SUCCESS : TEST_FAILURE;

    DEBUG_i32(s, t, "\n\tStacks after clean_NULL:")

    stack__free(s);
    stack__free(t);
    return result;
}

static char test_stack__clear_on_empty_stack(char debug)
{
    printf("%s... ", __func__);

    char result;
    Stack s = stack__empty_copy_enabled((copy_operator_t)operator_copy, (delete_operator_t)operator_delete);
    Stack t = stack__empty_copy_disabled();

    stack__clear(s);
    stack__clear(t);

    result = (stack__is_empty(s) && stack__is_empty(t)) ? TEST_SUCCESS : TEST_FAILURE;

    DEBUG_i32(s, t, "\n\tStacks after clear:")

    stack__free(s);
    stack__free(t);
    return result;
}

static char test_stack__clear_on_non_empty_stack(char debug)
{
    printf("%s... ", __func__);

    char result;
    u32 N = 5;
    Stack s = stack__empty_copy_enabled((copy_operator_t)operator_copy, (delete_operator_t)operator_delete);
    Stack t = stack__empty_copy_disabled();

    for (u32 i = 0; i < N; i++) {
        stack__push(s, &i);
        stack__push(t, &i);
    }

    DEBUG_i32(s, t, "\n\tStacks before clear:")

    stack__clear(s);
    stack__clear(t);

    result = (stack__is_empty(s) && stack__is_empty(t)) ? TEST_SUCCESS : TEST_FAILURE;

    DEBUG_i32(s, t, "\n\tStacks after clear:")

    stack__free(s);
    stack__free(t);
    return result;
}

static char test_stack__top_on_empty_stack(char debug)
{
    printf("%s... ", __func__);

    char result;
    Stack s = stack__empty_copy_enabled((copy_operator_t)operator_copy, (delete_operator_t)operator_delete);
    Stack t = stack__empty_copy_disabled();

    result = (stack__top(s, NULL)
           && stack__top(t, NULL)
           && stack__is_empty(s)
           && stack__is_empty(t)) ? TEST_SUCCESS : TEST_FAILURE;

    DEBUG_i32(s, t, "\n\tStacks after top:")

    stack__free(s);
    stack__free(t);
    return result;
}

static char test_stack__top_on_non_empty_stack(char debug)
{
    printf("%s... ", __func__);

    char result;
    int value1 = 5;
    int value2 = 7;
    elem_t top_s = NULL;
    elem_t top_t = NULL;
    Stack s = stack__empty_copy_enabled((copy_operator_t)operator_copy, (delete_operator_t)operator_delete);
    Stack t = stack__empty_copy_disabled();

    stack__push(s, &value1);
    stack__push(t, &value1);
    stack__push(s, &value2);
    stack__push(t, &value2);

    DEBUG_i32(s, t, "\n\tStacks before top:")

    result = (!stack__top(s, &top_s)
           && !stack__top(t, &top_t)
           && !stack__is_empty(s)
           && !stack__is_empty(t)
           && *(int *)top_s == 7
           && *(int *)top_t == 7) ? TEST_SUCCESS : TEST_FAILURE;

    DEBUG_i32(s, t, "\n\tStacks after top:")

    free(top_s);
    stack__free(s);
    stack__free(t);
    return result;
}

static char test_stack__from_array(char debug)
{
    printf("%s... ", __func__);

    char result;
    char A[5] = {'a', 'b', 'c', 'd', 'e'};
    char B[5] = {'f', 'g', 'h', 'i', 'j'};
    int C[5] = {1, 2, 3, 4, 5};
    Stack s_char = stack__empty_copy_enabled((copy_operator_t)operator_copy, (delete_operator_t)operator_delete);
    Stack t_char = stack__empty_copy_disabled();
    Stack s_int = stack__empty_copy_enabled((copy_operator_t)operator_copy, (delete_operator_t)operator_delete);
    Stack t_int = stack__empty_copy_disabled();

    result = (stack__from_array(s_char, (elem_t *)A, 5, CHAR)
           && stack__from_array(t_char, (elem_t *)A, 5, CHAR)
           && stack__from_array(s_char, (elem_t *)B, 5, CHAR)
           && stack__from_array(t_char, (elem_t *)B, 5, CHAR)
           && stack__from_array(s_int, (elem_t *)C, 5, INT)
           && stack__from_array(t_int, (elem_t *)C, 5, INT)
           && stack__size(s_char) == 10
           && stack__size(t_char) == 10
           && stack__size(s_int) == 5
           && stack__size(t_int) == 5) ? TEST_SUCCESS : TEST_FAILURE;

    DEBUG_char(s_char, t_char, "\n\tStacks after from_array:")
    DEBUG_i32(s_int, t_int, " ")

    stack__free(s_char);
    stack__free(t_char);
    stack__free(s_int);
    stack__free(t_int);
    return result;
}

static char test_stack__to_array_on_empty_stack(char debug)
{
    printf("%s... ", __func__);

    char result;
    Stack s = stack__empty_copy_enabled((copy_operator_t)operator_copy, (delete_operator_t)operator_delete);
    Stack t = stack__empty_copy_disabled();

    result = (!stack__to_array(s)
           && !stack__to_array(t)
           && stack__is_empty(s)
           && stack__is_empty(t)) ? TEST_SUCCESS : TEST_FAILURE;

    DEBUG_i32(s, t, "\n\tStacks after to_array:")

    stack__free(s);
    stack__free(t);
    return result;
}

static char test_stack__to_array_on_non_empty_stack(char debug)
{
    printf("%s... ", __func__);

    char result;
    u32 N = 5;
    int **A = NULL;
    int **B = NULL;
    Stack s = stack__empty_copy_enabled((copy_operator_t)operator_copy, (delete_operator_t)operator_delete);
    Stack t = stack__empty_copy_disabled();

    for (u32 i = 0; i < N; i++) {
        stack__push(s, &i);
        stack__push(t, &i);
    }

    DEBUG_i32(s, t, "\n\tStacks before to_array:")

    A = (int **)stack__to_array(s);
    B = (int **)stack__to_array(t);

    result = (stack__size(s) == N && stack__size(t) == N) ? TEST_SUCCESS : TEST_FAILURE;

    DEBUG_i32(s, t, "\n\tStacks after to_array:")

    for (u32 i = 0; i < N; i++) {
        result |= (char)(*A[i] != (int)i || *B[i] != (int)N);
    }

    for (u32 i = 0; i < N; i++) {
        free(A[i]);
    }
    free(A);
    free(B);
    stack__free(s);
    stack__free(t);
    return result;
}

static char test_stack__foreach_on_empty_stack(char debug)
{
    printf("%s... ", __func__);

    char result;
    int value = 1;
    Stack s = stack__empty_copy_enabled((copy_operator_t)operator_copy, (delete_operator_t)operator_delete);
    Stack t = stack__empty_copy_disabled();

    stack__foreach(s, (applying_func_t) plus_op, &value);
    stack__foreach(t, (applying_func_t) plus_op, &value);

    result = (stack__is_empty(s) && stack__is_empty(t)) ? TEST_SUCCESS : TEST_FAILURE;

    DEBUG_i32(s, t, "\n\tStacks after foreach:")

    stack__free(s);
    stack__free(t);
    return result;
}

static char test_stack__foreach_on_non_empty_stack(char debug)
{
    printf("%s... ", __func__);

    char result;
    u32 N = 8;
    u32 M = 2;
    int value = 1;
    int array[8] = {-5, 0, 2, 7, 11, 1, 3, 10};
    elem_t *A = NULL, *B = NULL, *C = NULL, *D = NULL;
    Stack s = stack__empty_copy_enabled((copy_operator_t)operator_copy, (delete_operator_t)operator_delete);
    Stack t = stack__empty_copy_disabled();

    stack__from_array(s, (elem_t *)array, N, INT);
    stack__from_array(t, (elem_t *)array, N, INT);

    for (u32 i = 0; i < N>>1; i++) {
        stack__pop(s, NULL);
        stack__pop(t, NULL);
    }

    for (u32 i = N>>1; i < ((N>>1) + M); i++) {
        stack__push(s, &array[i]);
        stack__push(t, &array[i]);
    }

    A = stack__to_array(s);
    B = stack__to_array(t);

    DEBUG_i32(s, t, "\n\tStacks before foreach:")

    stack__foreach(s, (applying_func_t) plus_op, &value);
    stack__foreach(t, (applying_func_t) plus_op, &value);

    C = stack__to_array(s);
    D = stack__to_array(t);

    result = TEST_SUCCESS;
    for (u32 i = 0; i < N-M; i++) {
        result |= (char)(*(int *)A[i] + value != *(int *)C[i] || *(int *)B[i] != *(int *)D[i]);
    }

    DEBUG_i32(s, t, "\n\tStacks after foreach:")

    for (u32 i = 0; i < N-M; i++) {
        free(A[i]);
        free(C[i]);
    }
    free(A);
    free(B);
    free(C);
    free(D);
    stack__free(s);
    stack__free(t);
    return result;
}

static char test_stack__sort_on_empty_stack(char debug)
{
    printf("%s... ", __func__);

    char result;
    Stack s = stack__empty_copy_enabled((copy_operator_t)operator_copy, (delete_operator_t)operator_delete);
    Stack t = stack__empty_copy_disabled();

    stack__sort(s, operator_compare);
    stack__sort(t, operator_compare);

    result = (stack__is_empty(s) && stack__is_empty(t)) ? TEST_SUCCESS : TEST_FAILURE;

    DEBUG_i32(s, t, "\n\tStacks after sort:")

    stack__free(s);
    stack__free(t);
    return result;
}

static char test_stack__sort_on_non_empty_stack(char debug)
{
    printf("%s... ", __func__);

    char result;
    u32 N = 8;
    int unordered[8] = {5, -3, 2, 0, 1, 0, 7, 4};
    int ordered[8] = {-3, 0, 0, 1, 2, 4, 5, 7};
    elem_t *A = NULL;
    elem_t *B = NULL;
    Stack s = stack__empty_copy_enabled((copy_operator_t)operator_copy, (delete_operator_t)operator_delete);
    Stack t = stack__empty_copy_disabled();

    stack__from_array(s, (elem_t *)unordered, N, INT);
    stack__from_array(t, (elem_t *)unordered, N, INT);

    DEBUG_i32(s, t, "\n\tStacks before sort:")

    stack__sort(s, operator_compare);
    stack__sort(t, operator_compare);

    A = stack__to_array(s);
    B = stack__to_array(t);

    result = TEST_SUCCESS;
    for (u32 i = 0; i < 8; i++) {
        result |= (char)(ordered[i] != *(int *)A[i] || ordered[i] != *(int *)B[i]);
    }

    DEBUG_i32(s, t, "\n\tStacks after sort:")

    for (u32 i = 0; i < 8; i++) {
        free(A[i]);
    }
    free(A);
    free(B);
    stack__free(s);
    stack__free(t);
    return result;
}

static char test_stack__shuffle_on_empty_stack(char debug)
{
    printf("%s... ", __func__);

    char result;
    Stack s = stack__empty_copy_enabled((copy_operator_t)operator_copy, (delete_operator_t)operator_delete);
    Stack t = stack__empty_copy_disabled();

    stack__shuffle(s);
    stack__shuffle(t);

    result = (stack__is_empty(s) && stack__is_empty(t)) ? TEST_SUCCESS : TEST_FAILURE;

    DEBUG_i32(s, t, "\n\tStacks after shuffle:")

    stack__free(s);
    stack__free(t);
    return result;
}

static char test_stack__shuffle_on_non_empty_stack(char debug)
{
    printf("%s... ", __func__);

    char result;
    u32 N = 8;
    int src[8] = {5, -3, 2, 0, 1, 0, 7, 4};
    elem_t *A = NULL;
    elem_t *B = NULL;
    Stack s = stack__empty_copy_enabled((copy_operator_t)operator_copy, (delete_operator_t)operator_delete);
    Stack t = stack__empty_copy_disabled();

    stack__from_array(s, (elem_t *)src, N, INT);
    stack__from_array(t, (elem_t *)src, N, INT);

    DEBUG_i32(s, t, "\n\tStacks before shuffle:")

    stack__shuffle(s);
    stack__shuffle(t);

    A = stack__to_array(s);
    B = stack__to_array(t);

    result = TEST_SUCCESS;

    DEBUG_i32(s, t, "\n\tStacks after shuffle:")

    for (u32 i = 0; i < N; i++) {
        free(A[i]);
    }
    free(A);
    free(B);
    stack__free(s);
    stack__free(t);
    return result;
}


int main(void)
{
    int nb_success = 0;
    int nb_tests = 0;
    printf("----------- TEST STACK -----------\n");

    print_test_result(test_stack__empty_copy_enabled(false), &nb_success, &nb_tests);
    print_test_result(test_stack__empty_copy_disabled(false), &nb_success, &nb_tests);
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
