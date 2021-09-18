#include "common_tests_utils.h"
#include "../stack/stack.h"
#include "../common/defs.h"

#define STACK_CREATE(A, B) \
    Stack A = NULL, B = NULL; \
    A = stack__empty_copy_enabled(operator_copy, operator_delete); \
    B = stack__empty_copy_disabled()

#define STACK_PUSH_u32(N, A, B) \
    ADD_u32(stack__push, N, A, B)

#define STACK_PUSH_i32_RAND(N, A, B) \
    ADD_i32_RAND(stack__push, N, A, B)

#define STACK_DEBUG_char(A, B, C) \
    DEBUG_char(stack__debug, A, B, C)

#define STACK_DEBUG_i32(A, B, C) \
    DEBUG_i32(stack__debug, A, B, C)

#define STACK_FREE(A, B, C, D) \
    FREE(stack__free, A, B, C, D)

#define TEST_ON_EMPTY_STACK(__name, __expr) \
static bool __name(char debug) \
{ \
    printf("%s... ", __func__); \
    bool result = TEST_SUCCESS; \
    STACK_CREATE(s, t); \
    __expr \
    bool __empty_assertion = stack__is_empty(s) == 1 && stack__is_empty(t) == 1; \
    STACK_DEBUG_i32(s, t, "\n\tStacks after:"); \
    STACK_FREE(s, t, NULL, NULL); \
    return result && __empty_assertion; \
}

#define TEST_ON_NON_EMPTY_STACK(__name, __expr) \
static bool __name(char debug) \
{ \
    printf("%s... ", __func__); \
    bool result; \
    elem_t *A = NULL, *B = NULL; \
    Stack s = stack__empty_copy_enabled(operator_copy, operator_delete); \
    Stack t = stack__empty_copy_disabled(); \
    u32 N = 8; \
    STACK_PUSH_i32_RAND(N, s, t); \
    STACK_DEBUG_i32(s, t, "\n\tStacks before:"); \
    __expr \
    STACK_DEBUG_i32(s, t, "\n\tStacks after:"); \
    for (u32 i = 0; i < N; i++) { \
        free(A[i]); \
    } \
    free(A); \
    free(B); \
    STACK_FREE(s, t, NULL, NULL); \
    return result; \
}

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

    STACK_FREE(s, NULL, NULL, NULL);
    return result;
}

static bool test_stack__empty_copy_enabled(char debug)
{
    printf("%s... ", __func__);

    bool result;
    Stack s = stack__empty_copy_enabled(operator_copy, operator_delete);

    result = s ? TEST_SUCCESS : TEST_FAILURE;

    if (debug) stack__debug(s, (void (*)(elem_t))operator_debug_i32);

    STACK_FREE(s, NULL, NULL, NULL);
    return result;
}

static bool test_stack__is_copy_enabled(void)
{
    printf("%s... ", __func__);

    bool result;
    STACK_CREATE(s, t);;

    result = (stack__is_copy_enabled(s) && !stack__is_copy_enabled(t)) ? TEST_SUCCESS : TEST_FAILURE;

    STACK_FREE(s, t, NULL, NULL);
    return result;
}

static bool test_stack__size(void)
{
    printf("%s... ", __func__);

    bool result;
    STACK_CREATE(s, t);;

    u32 N = 8;
    STACK_PUSH_u32(N, s, t);;

    result = (stack__size(s) == N && stack__size(t) == N) ? TEST_SUCCESS : TEST_FAILURE;

    STACK_FREE(s, t, NULL, NULL);
    return result;
}

static bool test_stack__is_empty_on_empty_stack(void)
{
    printf("%s... ", __func__);

    bool result;
    STACK_CREATE(s, t);

    result = (stack__is_empty(s) && stack__is_empty(t)) ? TEST_SUCCESS : TEST_FAILURE;

    STACK_FREE(s, t, NULL, NULL);
    return result;
}

static bool test_stack__is_empty_on_non_empty_stack(void)
{
    printf("%s... ", __func__);

    bool result;
    STACK_CREATE(s, t);

    STACK_PUSH_u32(1, s, t);

    result = (!stack__is_empty(s) && !stack__is_empty(t)) ? TEST_SUCCESS : TEST_FAILURE;

    STACK_FREE(s, t, NULL, NULL);
    return result;
}

static bool test_stack__push_on_empty_stack(char debug)
{
    printf("%s... ", __func__);

    bool result;
    STACK_CREATE(s, t);

    result = (!stack__push(s, NULL)
           && !stack__push(t, NULL)
           && stack__size(s) == 1
           && stack__size(t) == 1) ? TEST_SUCCESS : TEST_FAILURE;

    STACK_DEBUG_i32(s, t, "\n\tStacks after push:");

    STACK_FREE(s, t, NULL, NULL);
    return result;
}

static bool test_stack__push_on_non_empty_stack(char debug)
{
    printf("%s... ", __func__);

    bool result;
    STACK_CREATE(s, t);

    u32 N = 8;
    STACK_PUSH_u32(N, s, t);

    result = (stack__size(s) == N && stack__size(t) == N) ? TEST_SUCCESS : TEST_FAILURE;

    STACK_DEBUG_i32(s, t, "\n\tStacks after push:");

    STACK_FREE(s, t, NULL, NULL);
    return result;
}

TEST_ON_EMPTY_STACK (
    test_stack__pop_on_empty_stack,
    result = (stack__pop(s, NULL) == -1 && stack__pop(t, NULL) == -1) ? TEST_SUCCESS : TEST_FAILURE;
)

static bool test_stack__pop_on_non_empty_stack(char debug)
{
    printf("%s... ", __func__);

    bool result;
    elem_t top_s = NULL;
    elem_t top_t = NULL;
    STACK_CREATE(s, t);

    u32 N = 8;
    STACK_PUSH_u32(N, s, t);

    STACK_DEBUG_i32(s, t, "\n\tStacks before pop:");

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

    STACK_DEBUG_i32(s, t, "\n\tStacks after pop:");

    STACK_FREE(s, t, NULL, NULL);
    return result;
}

static bool test_stack__push_and_pop_on_multiple_elements(char debug)
{
    printf("%s... ", __func__);

    bool result;
    STACK_CREATE(s, t);

    STACK_PUSH_u32(60, s, t);

    result = (stack__size(s) == 60 && stack__size(t) == 60) ? TEST_SUCCESS : TEST_FAILURE;

    STACK_DEBUG_i32(s, t, "\nAdded 60 elements, expected capacity of 64 and size of 60\n");

    for (u32 i = 0; i < 40; i++) {
        stack__pop(s, NULL);
        stack__pop(t, NULL);
    }

    result |= stack__size(s) != 20 || stack__size(t) != 20;

    STACK_DEBUG_i32(s, t, "\nRemoved 40 elements, expected capacity of 32 and size of 20\n");

    STACK_PUSH_u32(20, s, t);

    result |= stack__size(s) != 40 || stack__size(t) != 40;

    STACK_DEBUG_i32(s, t, "\nAdded 20 elements, expected capacity of 64 and size of 40\n");

    STACK_FREE(s, t, NULL, NULL);
    return result;
}

TEST_ON_EMPTY_STACK (
    test_stack__clean_NULL_on_empty_stack,
    stack__clean_NULL(s);
    stack__clean_NULL(t);
)

static bool test_stack__clean_NULL_on_non_empty_stack(char debug)
{
    printf("%s... ", __func__);

    bool result;
    STACK_CREATE(s, t);

    u32 N = 8;
    for (u32 i = 0; i < N; i++) {
        stack__push(s, !(i%2) ? &i : NULL);
        stack__push(t, !(i%2) ? &i : NULL);
    }

    STACK_DEBUG_i32(s, t, "\n\tStacks before clean_NULL:");

    stack__clean_NULL(s);
    stack__clean_NULL(t);

    result = (stack__size(s) == N>>1 && stack__size(t) == N>>1) ? TEST_SUCCESS : TEST_FAILURE;

    STACK_DEBUG_i32(s, t, "\n\tStacks after clean_NULL:");

    STACK_FREE(s, t, NULL, NULL);
    return result;
}

TEST_ON_EMPTY_STACK (
    test_stack__clear_on_empty_stack,
    stack__clear(s);
    stack__clear(t);
)

static bool test_stack__clear_on_non_empty_stack(char debug)
{
    printf("%s... ", __func__);

    bool result;
    STACK_CREATE(s, t);

    STACK_PUSH_u32(5, s, t);

    STACK_DEBUG_i32(s, t, "\n\tStacks before clear:");

    stack__clear(s);
    stack__clear(t);

    result = (stack__is_empty(s) && stack__is_empty(t)) ? TEST_SUCCESS : TEST_FAILURE;

    STACK_DEBUG_i32(s, t, "\n\tStacks after clear:");

    STACK_FREE(s, t, NULL, NULL);
    return result;
}

TEST_ON_EMPTY_STACK (
    test_stack__peek_top_on_empty_stack,
    result = (stack__peek_top(s, NULL) == -1 && stack__peek_top(t, NULL) == -1) ? TEST_SUCCESS : TEST_FAILURE;
)

static bool test_stack__peek_top_on_non_empty_stack(char debug)
{
    printf("%s... ", __func__);

    bool result;
    int value1 = 5;
    int value2 = 7;
    elem_t top_s = NULL;
    elem_t top_t = NULL;
    STACK_CREATE(s, t);

    stack__push(s, &value1);
    stack__push(t, &value1);
    stack__push(s, &value2);
    stack__push(t, &value2);

    STACK_DEBUG_i32(s, t, "\n\tStacks before peek_top:");

    result = (!stack__peek_top(s, &top_s)
           && !stack__peek_top(t, &top_t)
           && !stack__is_empty(s)
           && !stack__is_empty(t)
           && *(int *)top_s == 7
           && *(int *)top_t == 7) ? TEST_SUCCESS : TEST_FAILURE;

    STACK_DEBUG_i32(s, t, "\n\tStacks after peek_top:");

    free(top_s);
    STACK_FREE(s, t, NULL, NULL);
    return result;
}

TEST_ON_EMPTY_STACK (
    test_stack__peek_nth_on_empty_stack,
    result = (stack__peek_nth(s, NULL, 0) == -1 && stack__peek_nth(t, NULL, 0) == -1) ? TEST_SUCCESS : TEST_FAILURE;
)

static bool test_stack__peek_nth_on_non_empty_stack(char debug)
{
    printf("%s... ", __func__);

    bool result;
    int value1 = 5;
    int value2 = 7;
    elem_t nth_s = NULL;
    elem_t nth_t = NULL;
    STACK_CREATE(s, t);

    stack__push(s, &value1);
    stack__push(t, &value1);
    stack__push(s, &value2);
    stack__push(t, &value2);

    STACK_DEBUG_i32(s, t, "\n\tStacks before peek_nth:");

    result = (!stack__peek_nth(s, &nth_s, 0)
           && !stack__peek_nth(t, &nth_t, 0)
           && !stack__is_empty(s)
           && !stack__is_empty(t)
           && *(int *)nth_s == 5
           && *(int *)nth_t == 5) ? TEST_SUCCESS : TEST_FAILURE;
    free(nth_s);

    result &= (!stack__peek_nth(s, &nth_s, 1)
            && !stack__peek_nth(t, &nth_t, 1)
            && !stack__is_empty(s)
            && !stack__is_empty(t)
            && *(int *)nth_s == 7
            && *(int *)nth_t == 7) ? TEST_SUCCESS : TEST_FAILURE;
    free(nth_s);

    STACK_DEBUG_i32(s, t, "\n\tStacks after peek_nth:");

    STACK_FREE(s, t, NULL, NULL);
    return result;
}

static bool test_stack__from_array(char debug)
{
    printf("%s... ", __func__);

    bool result;
    char A[5] = {'a', 'b', 'c', 'd', 'e'};
    char B[5] = {'f', 'g', 'h', 'i', 'j'};
    int C[5] = {1, 2, 3, 4, 5};
    elem_t *D = NULL;
    STACK_CREATE(s_char, t_char);
    STACK_CREATE(s_int, t_int);

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

    STACK_DEBUG_char(s_char, t_char, "\n\tStacks after from_array:");
    STACK_DEBUG_i32(s_int, t_int, " ");

    free(D);
    STACK_FREE(s_char, t_char, s_int, t_int);
    return result;
}

/* DUMP */
TEST_ON_EMPTY_STACK (
    test_stack__dump_on_empty_stack,
    result = (stack__dump(s) == NULL && stack__dump(t) == NULL) ? TEST_SUCCESS : TEST_FAILURE;
)

TEST_ON_NON_EMPTY_STACK (
    test_stack__dump_on_non_empty_stack,
    A = stack__dump(s);
    B = stack__dump(t);

    result = (stack__is_empty(s) && stack__is_empty(t)) ? TEST_SUCCESS : TEST_FAILURE;

    for (u32 i = 0; i < N; i++) {
        result |= *(u32 *)A[i] != i || *(u32 *)B[i] != N;
    }
)

/* TO_ARRAY */
TEST_ON_EMPTY_STACK (
    test_stack__to_array_on_empty_stack,
    result = (stack__to_array(s) == NULL && stack__to_array(t) == NULL) ? TEST_SUCCESS : TEST_FAILURE;
)

TEST_ON_NON_EMPTY_STACK (
    test_stack__to_array_on_non_empty_stack,
    A = stack__to_array(s);
    B = stack__to_array(t);

    result = (stack__size(s) == N && stack__size(t) == N) ? TEST_SUCCESS : TEST_FAILURE;

    for (u32 i = 0; i < N; i++) {
        result |= *(u32 *)A[i] != i || *(u32 *)B[i] != N;
    }
)

/* FOREACH */
TEST_ON_EMPTY_STACK (
    test_stack__foreach_on_empty_stack,
    int value = 1;
    stack__foreach(s, plus_op, &value);
    stack__foreach(t, plus_op, &value);
)

TEST_ON_NON_EMPTY_STACK (
    test_stack__foreach_on_non_empty_stack,
    u32 value = 1;
    stack__foreach(s, plus_op, &value);
    stack__foreach(t, plus_op, &value);

    A = stack__to_array(s);
    B = stack__to_array(t);

    result = TEST_SUCCESS;
    for (u32 i = 0; i < N; i++) {
        result |= i + value == *(u32 *)A[i] && N<<1 == *(u32 *)B[i];
    }
)

/* REVERSE */
TEST_ON_EMPTY_STACK (
    test_stack__reverse_on_empty_stack,
    stack__reverse(s);
    stack__reverse(t);
)

TEST_ON_NON_EMPTY_STACK (
    test_stack__reverse_on_non_empty_stack,
    stack__reverse(s);
    A = stack__to_array(s);

    result = TEST_SUCCESS;
    for (u32 i = 0; i < N; i++) {
        result |= *(int *)A[i] != (int)(N-i-1);
    }
)

/* SHUFFLE */
TEST_ON_EMPTY_STACK (
    test_stack__shuffle_on_empty_stack,
    stack__shuffle(s);
    stack__shuffle(t);
)

TEST_ON_NON_EMPTY_STACK (
    test_stack__shuffle_on_non_empty_stack,
    stack__shuffle(s);
    A = stack__to_array(s);

    result = TEST_FAILURE;
    for (u32 i = 0; i < N; i++) {
        result |= *(u32 *)A[i] != i;
    }
)

/* SORT */
TEST_ON_EMPTY_STACK (
    test_stack__sort_on_empty_stack,
    stack__sort(s, operator_compare);
    stack__sort(t, operator_compare);
)

TEST_ON_NON_EMPTY_STACK (
    test_stack__sort_on_non_empty_stack,
    stack__sort(s, operator_compare);
    A = stack__to_array(s);

    result = TEST_SUCCESS;
    for (u32 i = 0; i < N - 1; i++) {
        result |= *(int *)A[i] > *(int *)A[i+1];
    }
)


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
    print_test_result(test_stack__peek_top_on_empty_stack(false), &nb_success, &nb_tests);
    print_test_result(test_stack__peek_top_on_non_empty_stack(false), &nb_success, &nb_tests);
    print_test_result(test_stack__peek_nth_on_empty_stack(false), &nb_success, &nb_tests);
    print_test_result(test_stack__peek_nth_on_non_empty_stack(false), &nb_success, &nb_tests);

    print_test_result(test_stack__from_array(false), &nb_success, &nb_tests);
    print_test_result(test_stack__dump_on_empty_stack(false), &nb_success, &nb_tests);
    print_test_result(test_stack__dump_on_non_empty_stack(false), &nb_success, &nb_tests);
    print_test_result(test_stack__to_array_on_empty_stack(false), &nb_success, &nb_tests);
    print_test_result(test_stack__to_array_on_non_empty_stack(false), &nb_success, &nb_tests);
    print_test_result(test_stack__foreach_on_empty_stack(false), &nb_success, &nb_tests);
    print_test_result(test_stack__foreach_on_non_empty_stack(false), &nb_success, &nb_tests);
    print_test_result(test_stack__reverse_on_empty_stack(false), &nb_success, &nb_tests);
    print_test_result(test_stack__reverse_on_non_empty_stack(false), &nb_success, &nb_tests);
    print_test_result(test_stack__shuffle_on_empty_stack(false), &nb_success, &nb_tests);
    print_test_result(test_stack__shuffle_on_non_empty_stack(false), &nb_success, &nb_tests);
    print_test_result(test_stack__sort_on_empty_stack(false), &nb_success, &nb_tests);
    print_test_result(test_stack__sort_on_non_empty_stack(false), &nb_success, &nb_tests);

    print_test_summary(nb_success, nb_tests);

    return TEST_SUCCESS;
}
