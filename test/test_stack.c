#include "common_tests_utils.h"
#include "../stack/stack.h"
#include "../common/defs.h"

#define STACK_CREATE(A, B) \
    Stack A = NULL, B = NULL; \
    A = stack__empty_copy_enabled(operator_copy, operator_delete); \
    B = stack__empty_copy_disabled()

#define STACK_PUSH_u32(N, __elems, A, B) \
    ADD_u32(stack__push, N, __elems, A, B)

#define STACK_DEBUG_char(A, B, C) \
    DEBUG_char(stack__debug, A, B, C)

#define STACK_DEBUG_u32(A, B, C) \
    DEBUG_u32(stack__debug, A, B, C)

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
    STACK_DEBUG_u32(s, t, "\n\tStacks after:"); \
    STACK_FREE(s, t, NULL, NULL); \
    return result && __empty_assertion; \
}

#define TEST_ON_NON_EMPTY_STACK(__name, __rand, __expr) \
static bool __name(char debug) \
{ \
    printf("%s... ", __func__); \
    bool result = TEST_SUCCESS; \
    elem_t *A = NULL, *B = NULL; \
    STACK_CREATE(s, t); \
    u32 N = 8; \
    u32 *elems = malloc(sizeof(u32) * N); \
    for (u32 i = 0; i < N; i++) { \
        elems[i] = __rand ? (u32)rand() % 20 : i; \
    } \
    STACK_PUSH_u32(N, elems, s, t); \
    STACK_DEBUG_u32(s, t, "\n\tStacks before:"); \
    __expr \
    STACK_DEBUG_u32(s, t, "\n\tStacks after:"); \
    if (A) { \
        for (u32 i = 0; i < N; i++) { \
            free(A[i]); \
        } \
        free(A); \
    } \
    free(B); \
    free(elems); \
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

/* SIZE */
TEST_ON_NON_EMPTY_STACK (
    test_stack__size, true,
    result = (stack__size(s) == N && stack__size(t) == N) ? TEST_SUCCESS : TEST_FAILURE;
)

/* IS_EMPTY */
TEST_ON_EMPTY_STACK(
    test_stack__is_empty_on_empty_stack,
    ;
)

TEST_ON_NON_EMPTY_STACK (
    test_stack__is_empty_on_non_empty_stack, true,
    result = (!stack__is_empty(s) && !stack__is_empty(t)) ? TEST_SUCCESS : TEST_FAILURE;
)

/* PUSH */
TEST_ON_NON_EMPTY_STACK (
    test_stack__push, false,
    result &= (stack__size(s) == N && stack__size(t) == N) ? TEST_SUCCESS : TEST_FAILURE;
    result &= COMPARE2(stack__peek_nth, stack__size(s), elems, s, true);
    result &= COMPARE2(stack__peek_nth, stack__size(t), elems, t, false);
)

/* POP */
TEST_ON_EMPTY_STACK(
    test_stack__pop_on_empty_stack,
    result = (stack__pop(s, NULL) == -1 && stack__pop(t, NULL) == -1) ? TEST_SUCCESS : TEST_FAILURE;
)

TEST_ON_NON_EMPTY_STACK (
    test_stack__pop_on_non_empty_stack, false,
    elem_t top_s = NULL;
    elem_t top_t = NULL;
    for (u32 i = 0; i < N; i++) {
        result &= !stack__pop(s, &top_s)
               && !stack__pop(t, &top_t)
               && stack__size(s) == N-i-1
               && stack__size(t) == N-i-1
               && *(u32 *)top_s == N-i-1
               && *(u32 *)top_t == N-i-1;
        free(top_s);
    }
)

/* PEEK_TOP */
TEST_ON_EMPTY_STACK (
    test_stack__peek_top_on_empty_stack,
    result = (stack__peek_top(s, NULL) == -1 && stack__peek_top(t, NULL) == -1) ? TEST_SUCCESS : TEST_FAILURE;
)

TEST_ON_NON_EMPTY_STACK (
    test_stack__peek_top_on_non_empty_stack, false,
    elem_t top_s = NULL;
    elem_t top_t = NULL;
    result = (!stack__peek_top(s, &top_s)
           && !stack__peek_top(t, &top_t)
           && !stack__is_empty(s)
           && !stack__is_empty(t)
           && *(u32 *)top_s == N-1
           && *(u32 *)top_t == N-1) ? TEST_SUCCESS : TEST_FAILURE;
    free(top_s);
)

/* PEEK_NTH */
TEST_ON_EMPTY_STACK (
    test_stack__peek_nth_on_empty_stack,
    result = (stack__peek_nth(s, 0, NULL) == -1 && stack__peek_nth(t, 0, NULL) == -1) ? TEST_SUCCESS : TEST_FAILURE;
)

TEST_ON_NON_EMPTY_STACK (
    test_stack__peek_nth_on_non_empty_stack, false,
    elem_t nth_s = NULL;
    elem_t nth_t = NULL;
    result = (!stack__peek_nth(s, N>>1, &nth_s)
           && !stack__peek_nth(t, N>>1, &nth_t)
           && !stack__is_empty(s)
           && !stack__is_empty(t)
           && *(u32 *)nth_s == (N>>1)
           && *(u32 *)nth_t == (N>>1)) ? TEST_SUCCESS : TEST_FAILURE;
    free(nth_s);
)

/* SWAP */
TEST_ON_EMPTY_STACK (
    test_stack__swap_on_empty_stack,
    stack__swap(s, 2, 5);
    stack__swap(t, 2, 5);
)

TEST_ON_NON_EMPTY_STACK (
    test_stack__swap_on_non_empty_stack, false,
    elem_t pre_s1;
    elem_t pre_s2;
    elem_t post_s1;
    elem_t post_s2;
    elem_t pre_t1;
    elem_t pre_t2;
    elem_t post_t1;
    elem_t post_t2;
    stack__peek_nth(s, 2, &pre_s1);
    stack__peek_nth(s, 5, &pre_s2);
    stack__peek_nth(t, 2, &pre_t1);
    stack__peek_nth(t, 5, &pre_t2);
    result &= !stack__swap(s, 2, 5);
    result &= !stack__swap(t, 2, 5);
    stack__peek_nth(s, 2, &post_s1);
    stack__peek_nth(s, 5, &post_s2);
    stack__peek_nth(t, 2, &post_t1);
    stack__peek_nth(t, 5, &post_t2);

    result = *(u32*)pre_s1 == *(u32*)post_s2 && *(u32*)pre_s2 == *(u32*)post_s1;
    result = *(u32*)pre_t1 == *(u32*)post_t2 && *(u32*)pre_t2 == *(u32*)post_t1;
    free(pre_s1);
    free(pre_s2);
    free(post_s1);
    free(post_s2);
)

static bool test_stack__from_array(char debug)
{
    printf("%s... ", __func__);

    bool result;
    char A[5] = {'a', 'b', 'c', 'd', 'e'};
    char B[5] = {'f', 'g', 'h', 'i', 'j'};
    u32 C[5] = {1, 2, 3, 4, 5};
    STACK_CREATE(s_char, t_char);
    STACK_CREATE(s_u32, t_u32);

    result = (stack__from_array(s_char, A, 5, sizeof(char))
           && (t_char = stack__from_array(t_char, A, 5, sizeof(char)))
           && stack__from_array(s_char, B, 5, sizeof(char))
           && stack__from_array(t_char, B, 5, sizeof(char))
           && stack__from_array(s_u32, C, 5, sizeof(u32))
           && (t_u32 = stack__from_array(t_u32, C, 5, sizeof(u32)))
           && stack__size(s_char) == 10
           && stack__size(t_char) == 10
           && stack__size(s_u32) == 5
           && stack__size(t_u32) == 5) ? TEST_SUCCESS : TEST_FAILURE;

    result &= COMPARE2(stack__peek_nth, stack__size(s_u32), C, s_u32, true);
    result &= COMPARE2(stack__peek_nth, stack__size(t_u32), C, t_u32, false);

    STACK_DEBUG_char(s_char, t_char, "\n\tStacks after from_array:");
    STACK_DEBUG_u32(s_u32, t_u32, " ");

    STACK_FREE(s_char, t_char, s_u32, t_u32);
    return result;
}

/* DUMP */
TEST_ON_EMPTY_STACK (
    test_stack__dump_on_empty_stack,
    result = (stack__dump(s) == NULL && stack__dump(t) == NULL) ? TEST_SUCCESS : TEST_FAILURE;
)

TEST_ON_NON_EMPTY_STACK (
    test_stack__dump_on_non_empty_stack, true,
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
    test_stack__to_array_on_non_empty_stack, true,
    A = stack__to_array(s);
    B = stack__to_array(t);

    result = (stack__size(s) == N && stack__size(t) == N) ? TEST_SUCCESS : TEST_FAILURE;

    for (u32 i = 0; i < N; i++) {
        result |= *(u32 *)A[i] != i || *(u32 *)B[i] != N;
    }
)

/* COPY AND CMP */
TEST_ON_EMPTY_STACK (
    test_stack__copy_and_cmp_on_empty_stack,
    Stack u = stack__copy(s);
    Stack v = stack__copy(t);

    result = (stack__cmp(u, s, operator_compare)
           && stack__cmp(v, t, operator_compare)
           && COMPARE(stack__peek_nth, stack__size(s), u, s, true)
           && COMPARE(stack__peek_nth, stack__size(t), v, t, false)) ? TEST_SUCCESS : TEST_FAILURE;
    STACK_FREE(u, v, NULL, NULL);
)

TEST_ON_NON_EMPTY_STACK (
    test_stack__copy_and_cmp_on_non_empty_stack, false,
    Stack u = stack__copy(s);
    Stack v = stack__copy(t);

    result = (stack__cmp(u, s, operator_compare)
           && stack__cmp(v, t, operator_compare)
           && COMPARE(stack__peek_nth, stack__size(s), u, s, true)
           && COMPARE(stack__peek_nth, stack__size(t), v, t, false)) ? TEST_SUCCESS : TEST_FAILURE;
    STACK_FREE(u, v, NULL, NULL);
)

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

    STACK_DEBUG_u32(s, t, "\n\tStacks before clean_NULL:");

    stack__clean_NULL(s);
    stack__clean_NULL(t);

    result = (stack__size(s) == N>>1 && stack__size(t) == N>>1) ? TEST_SUCCESS : TEST_FAILURE;

    STACK_DEBUG_u32(s, t, "\n\tStacks after clean_NULL:");

    STACK_FREE(s, t, NULL, NULL);
    return result;
}

/* CLEAR */
TEST_ON_EMPTY_STACK (
    test_stack__clear_on_empty_stack,
    stack__clear(s);
    stack__clear(t);
)

TEST_ON_NON_EMPTY_STACK (
    test_stack__clear_on_non_empty_stack, true,
    stack__clear(s);
    stack__clear(t);

    result = (stack__is_empty(s) && stack__is_empty(t)) ? TEST_SUCCESS : TEST_FAILURE;
)

/* FOREACH */
TEST_ON_EMPTY_STACK (
    test_stack__foreach_on_empty_stack,
    int value = 1;
    stack__foreach(s, plus_op, &value);
    stack__foreach(t, plus_op, &value);
)

TEST_ON_NON_EMPTY_STACK (
    test_stack__foreach_on_non_empty_stack, false,
    u32 value = 1;
    stack__foreach(s, plus_op, &value);
    stack__foreach(t, plus_op, &value);

    result &= COMPARE3(stack__peek_nth, stack__size(s), s, value, true);
    result &= COMPARE3(stack__peek_nth, stack__size(t), t, value, false);
)

/* FILTER AND ALL */
TEST_ON_EMPTY_STACK (
    test_stack__filter_and_all_on_empty_stack,
    u32 value = 2;
    stack__filter(s, predicate, &value);
    stack__filter(t, predicate, &value);

    result = stack__all(s, predicate, &value) && stack__all(t, predicate, &value);
)

TEST_ON_NON_EMPTY_STACK (
    test_stack__filter_and_all_on_non_empty_stack, false,
    u32 value1 = 2;
    u32 value2 = 3;
    stack__filter(s, predicate, &value1);
    stack__filter(t, predicate, &value1);

    result &= stack__all(s, predicate, &value1) && stack__all(t, predicate, &value1);
    result &= !stack__all(s, predicate, &value2) && !stack__all(t, predicate, &value2);
)

/* ANY */
TEST_ON_EMPTY_STACK (
    test_stack__any_on_empty_stack,
    u32 value = 2;

    result = !stack__any(s, predicate, &value) && !stack__any(t, predicate, &value);
)

TEST_ON_NON_EMPTY_STACK (
    test_stack__any_on_non_empty_stack, false,
    u32 value1 = 2;
    u32 value2 = 3;

    result &= stack__any(s, predicate, &value1) && stack__any(t, predicate, &value1);
    result &= stack__any(s, predicate, &value2) && stack__any(t, predicate, &value2);
)

/* REVERSE */
TEST_ON_EMPTY_STACK (
    test_stack__reverse_on_empty_stack,
    stack__reverse(s);
    stack__reverse(t);
)

TEST_ON_NON_EMPTY_STACK (
    test_stack__reverse_on_non_empty_stack, false,
    stack__reverse(s);
    stack__reverse(t);

    result &= IS_REVERSE(stack__peek_nth, stack__size(s), s, true);
    result &= IS_REVERSE(stack__peek_nth, stack__size(t), t, false);
)

/* SHUFFLE */
TEST_ON_EMPTY_STACK (
    test_stack__shuffle_on_empty_stack,
    stack__shuffle(s, 1);
    stack__shuffle(t, 1);
)

TEST_ON_NON_EMPTY_STACK (
    test_stack__shuffle_on_non_empty_stack, true,
    stack__shuffle(s, 1);
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

    result &= IS_SORTED(stack__peek_nth, stack__size(s), s, true);
    result &= IS_SORTED(stack__peek_nth, stack__size(t), t, false);
)

TEST_ON_NON_EMPTY_STACK (
    test_stack__sort_on_non_empty_stack, true,
    stack__sort(s, operator_compare);
    stack__sort(t, operator_compare);

    result &= IS_SORTED(stack__peek_nth, stack__size(s), s, true);
    result &= IS_SORTED(stack__peek_nth, stack__size(t), t, false);
)


int main(void)
{
    int nb_success = 0;
    int nb_tests = 0;
    printf("----------- TEST STACK -----------\n");

    print_test_result(test_stack__empty_copy_disabled(false), &nb_success, &nb_tests);
    print_test_result(test_stack__empty_copy_enabled(false), &nb_success, &nb_tests);
    print_test_result(test_stack__is_copy_enabled(), &nb_success, &nb_tests);
    print_test_result(test_stack__size(false), &nb_success, &nb_tests);
    print_test_result(test_stack__is_empty_on_empty_stack(false), &nb_success, &nb_tests);
    print_test_result(test_stack__is_empty_on_non_empty_stack(false), &nb_success, &nb_tests);

    print_test_result(test_stack__push(false), &nb_success, &nb_tests);
    print_test_result(test_stack__pop_on_empty_stack(false), &nb_success, &nb_tests);
    print_test_result(test_stack__pop_on_non_empty_stack(false), &nb_success, &nb_tests);
    print_test_result(test_stack__peek_top_on_empty_stack(false), &nb_success, &nb_tests);
    print_test_result(test_stack__peek_top_on_non_empty_stack(false), &nb_success, &nb_tests);
    print_test_result(test_stack__peek_nth_on_empty_stack(false), &nb_success, &nb_tests);
    print_test_result(test_stack__peek_nth_on_non_empty_stack(false), &nb_success, &nb_tests);
    print_test_result(test_stack__swap_on_empty_stack(false), &nb_success, &nb_tests);
    print_test_result(test_stack__swap_on_non_empty_stack(false), &nb_success, &nb_tests);

    print_test_result(test_stack__from_array(false), &nb_success, &nb_tests);
    print_test_result(test_stack__dump_on_empty_stack(false), &nb_success, &nb_tests);
    print_test_result(test_stack__dump_on_non_empty_stack(false), &nb_success, &nb_tests);
    print_test_result(test_stack__to_array_on_empty_stack(false), &nb_success, &nb_tests);
    print_test_result(test_stack__to_array_on_non_empty_stack(false), &nb_success, &nb_tests);
    print_test_result(test_stack__copy_and_cmp_on_empty_stack(false), &nb_success, &nb_tests);
    print_test_result(test_stack__copy_and_cmp_on_non_empty_stack(false), &nb_success, &nb_tests);
    print_test_result(test_stack__clean_NULL_on_empty_stack(false), &nb_success, &nb_tests);
    print_test_result(test_stack__clean_NULL_on_non_empty_stack(false), &nb_success, &nb_tests);
    print_test_result(test_stack__clear_on_empty_stack(false), &nb_success, &nb_tests);
    print_test_result(test_stack__clear_on_non_empty_stack(false), &nb_success, &nb_tests);

    print_test_result(test_stack__foreach_on_empty_stack(false), &nb_success, &nb_tests);
    print_test_result(test_stack__foreach_on_non_empty_stack(false), &nb_success, &nb_tests);
    print_test_result(test_stack__filter_and_all_on_empty_stack(false), &nb_success, &nb_tests);
    print_test_result(test_stack__filter_and_all_on_non_empty_stack(false), &nb_success, &nb_tests);
    print_test_result(test_stack__any_on_empty_stack(false), &nb_success, &nb_tests);
    print_test_result(test_stack__any_on_non_empty_stack(false), &nb_success, &nb_tests);
    print_test_result(test_stack__reverse_on_empty_stack(false), &nb_success, &nb_tests);
    print_test_result(test_stack__reverse_on_non_empty_stack(false), &nb_success, &nb_tests);
    print_test_result(test_stack__shuffle_on_empty_stack(false), &nb_success, &nb_tests);
    print_test_result(test_stack__shuffle_on_non_empty_stack(false), &nb_success, &nb_tests);
    print_test_result(test_stack__sort_on_empty_stack(false), &nb_success, &nb_tests);
    print_test_result(test_stack__sort_on_non_empty_stack(false), &nb_success, &nb_tests);

    print_test_summary(nb_success, nb_tests);

    return TEST_SUCCESS;
}
