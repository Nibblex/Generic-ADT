#include "common_tests_utils.h"
#include "../queue/queue.h"
#include "../common/defs.h"

#define QUEUE_CREATE(A, B) \
    Queue A = NULL, B = NULL; \
    A = queue__empty_copy_enabled(operator_copy, operator_delete); \
    B = queue__empty_copy_disabled()

#define QUEUE_FROM_ARRAY(N, __elems, A, B) \
    TEST_FROM_ARRAY(queue__enqueue, N, __elems, A, B)

#define QUEUE_DEBUG_char(A, B, C) \
    DEBUG_char(queue__debug, A, B, C)

#define QUEUE_DEBUG_u32(A, B, C) \
    DEBUG_u32(queue__debug, A, B, C)

#define QUEUE_FREE(A, B, C, D) \
    FREE(queue__free, A, B, C, D)

#define TEST_ON_EMPTY_QUEUE(__name, __expr) \
static bool __name(char debug) \
{ \
    printf("%s... ", __func__); \
    bool result = TEST_SUCCESS; \
    QUEUE_CREATE(q, w); \
    __expr \
    bool __empty_assertion = queue__is_empty(q) == 1 && queue__is_empty(w) == 1; \
    QUEUE_DEBUG_u32(q, w, "\n\tQueues after:"); \
    QUEUE_FREE(q, w, NULL, NULL); \
    return result && __empty_assertion; \
}

#define TEST_ON_NON_EMPTY_QUEUE(__name, __rand, __expr) \
static bool __name(char debug) \
{ \
    printf("%s... ", __func__); \
    bool result = TEST_SUCCESS; \
    elem_t *A = NULL, *B = NULL; \
    QUEUE_CREATE(q, w); \
    u32 N = 8; \
    u32 *elems = malloc(sizeof(u32) * N); \
    for (u32 i = 0; i < N; i++) { \
        elems[i] = __rand ? (u32)rand() % 20 : i; \
    } \
    QUEUE_FROM_ARRAY(N, elems, q, w); \
    QUEUE_DEBUG_u32(q, w, "\n\tQueues before:"); \
    __expr \
    QUEUE_DEBUG_u32(q, w, "\n\tQueues after:"); \
    if (A) { \
        for (u32 i = 0; i < N; i++) { \
            free(A[i]); \
        } \
        free(A); \
    } \
    free(B); \
    free(elems); \
    QUEUE_FREE(q, w, NULL, NULL); \
    return result; \
}

////////////////////////////////////////////////////////////////////
///     TEST SUITE
////////////////////////////////////////////////////////////////////

static bool test_queue__empty_copy_disabled(char debug)
{
    printf("%s... ", __func__);

    bool result;
    Queue q = queue__empty_copy_disabled();

    result = q ? TEST_SUCCESS : TEST_FAILURE;

    if (debug) queue__debug(q, (void (*)(elem_t))operator_debug_i32);

    QUEUE_FREE(q, NULL, NULL, NULL);
    return result;
}

static bool test_queue__empty_copy_enabled(char debug)
{
    printf("%s... ", __func__);

    bool result;
    Queue q = queue__empty_copy_enabled(operator_copy, operator_delete);

    result = q ? TEST_SUCCESS : TEST_FAILURE;

    if (debug) queue__debug(q, (void (*)(elem_t))operator_debug_i32);

    QUEUE_FREE(q, NULL, NULL, NULL);
    return result;
}

static bool test_queue__is_copy_enabled(void)
{
    printf("%s... ", __func__);

    bool result;
    QUEUE_CREATE(q, w);

    result = (queue__is_copy_enabled(q) && !queue__is_copy_enabled(w)) ? TEST_SUCCESS : TEST_FAILURE;

    QUEUE_FREE(q, w, NULL, NULL);
    return result;
}

/* SIZE */
TEST_ON_NON_EMPTY_QUEUE (
    test_queue__length, true,
    result = (queue__length(q) == N && queue__length(w) == N) ? TEST_SUCCESS : TEST_FAILURE;
)

/* IS_EMPTY */
TEST_ON_EMPTY_QUEUE(
    test_queue__is_empty_on_empty_queue,
    ;
)

TEST_ON_NON_EMPTY_QUEUE (
    test_queue__is_empty_on_non_empty_queue, true,
    result = (!queue__is_empty(q) && !queue__is_empty(w)) ? TEST_SUCCESS : TEST_FAILURE;
)

/* ENQUEUE */
TEST_ON_NON_EMPTY_QUEUE (
    test_queue__enqueue, false,
    result = (queue__length(q) == N && queue__length(w) == N) ? TEST_SUCCESS : TEST_FAILURE;
    result &= COMPARE2(queue__peek_nth, queue__length(q), elems, q, true);
    result &= COMPARE2(queue__peek_nth, queue__length(w), elems, w, false);
)

/* DEQUEUE */
TEST_ON_EMPTY_QUEUE (
    test_queue__dequeue_on_empty_queue,
    result = (queue__dequeue(q, NULL) == -1 && queue__dequeue(w, NULL) == -1) ? TEST_SUCCESS : TEST_FAILURE;
)

TEST_ON_NON_EMPTY_QUEUE (
    test_queue__dequeue_on_non_empty_queue, false,
    elem_t front_q = NULL;
    elem_t front_w = NULL;
    for (u32 i = 0; i < N; i++) {
        result &= !queue__dequeue(q, &front_q)
               && !queue__dequeue(w, &front_w)
               && queue__length(q) == N-i-1
               && queue__length(w) == N-i-1
               && *(u32 *)front_q == i
               && *(u32 *)front_w == i;
        free(front_q);
    }
)

/* REMOVE_NTH */
TEST_ON_EMPTY_QUEUE (
    test_queue__remove_nth_on_empty_queue,
    result &= queue__remove_nth(q, 0) == -1;
    result &= queue__remove_nth(w, 0) == -1;
)

TEST_ON_NON_EMPTY_QUEUE (
    test_queue__remove_nth_on_non_empty_queue, true,
    elem_t tmp = NULL;
    for (u32 i = 0; i < N; i++) {
        result &= queue__remove_nth(q, i) == 0;
        result &= queue__remove_nth(w, i) == 0;
        queue__peek_nth(q, i, &tmp);
        result &= tmp == NULL;
        queue__peek_nth(w, i, &tmp);
        result &= tmp == NULL;
    }
)

/* PEEK_FRONT */
TEST_ON_EMPTY_QUEUE (
    test_queue__peek_front_on_empty_queue,
    result = (queue__peek_front(q, NULL) == -1 && queue__peek_front(w, NULL) == -1) ? TEST_SUCCESS : TEST_FAILURE;
)
TEST_ON_NON_EMPTY_QUEUE (
    test_queue__peek_front_on_non_empty_queue, false,
    elem_t front_q = NULL;
    elem_t front_w = NULL;
    result = (!queue__peek_front(q, &front_q)
           && !queue__peek_front(w, &front_w)
           && !queue__is_empty(q)
           && !queue__is_empty(w)
           && *(u32 *)front_q == 0
           && *(u32 *)front_w == 0) ? TEST_SUCCESS : TEST_FAILURE;
    free(front_q);
)

/* PEEK_BACK */
TEST_ON_EMPTY_QUEUE (
    test_queue__peek_back_on_empty_queue,
    result = (queue__peek_back(q, NULL) == -1 && queue__peek_back(w, NULL) == -1) ? TEST_SUCCESS : TEST_FAILURE;
)

TEST_ON_NON_EMPTY_QUEUE (
    test_queue__peek_back_on_non_empty_queue, false,
    elem_t back_q = NULL;
    elem_t back_w = NULL;
    result = (!queue__peek_back(q, &back_q)
           && !queue__peek_back(w, &back_w)
           && !queue__is_empty(q)
           && !queue__is_empty(w)
           && *(u32 *)back_q == N-1
           && *(u32 *)back_w == N-1) ? TEST_SUCCESS : TEST_FAILURE;
    free(back_q);
)

/* PEEK_NTH */
TEST_ON_EMPTY_QUEUE (
    test_queue__peek_nth_on_empty_queue,
    result = (queue__peek_nth(q, 0, NULL) == -1 && queue__peek_nth(w, 0, NULL) == -1) ? TEST_SUCCESS : TEST_FAILURE;
)

TEST_ON_NON_EMPTY_QUEUE (
    test_queue__peek_nth_on_non_empty_queue, false,
    elem_t nth_q = NULL;
    elem_t nth_w = NULL;
    result = (!queue__peek_nth(q, N>>1, &nth_q)
           && !queue__peek_nth(w, N>>1, &nth_w)
           && !queue__is_empty(q)
           && !queue__is_empty(w)
           && *(u32 *)nth_q == N>>1
           && *(u32 *)nth_w == N>>1) ? TEST_SUCCESS : TEST_FAILURE;
    free(nth_q);
)

/* SWAP */
TEST_ON_EMPTY_QUEUE (
    test_queue__swap_on_empty_queue,
    queue__swap(q, 2, 5);
    queue__swap(w, 2, 5);
)

TEST_ON_NON_EMPTY_QUEUE (
    test_queue__swap_on_non_empty_queue, false,
    elem_t pre_q1;
    elem_t pre_q2;
    elem_t post_q1;
    elem_t post_q2;
    elem_t pre_w1;
    elem_t pre_w2;
    elem_t post_w1;
    elem_t post_w2;
    queue__peek_nth(q, 2, &pre_q1);
    queue__peek_nth(q, 5, &pre_q2);
    queue__peek_nth(w, 2, &pre_w1);
    queue__peek_nth(w, 5, &pre_w2);
    result &= !queue__swap(q, 2, 5);
    result &= !queue__swap(w, 2, 5);
    queue__peek_nth(q, 2, &post_q1);
    queue__peek_nth(q, 5, &post_q2);
    queue__peek_nth(w, 2, &post_w1);
    queue__peek_nth(w, 5, &post_w2);

    result = *(u32*)pre_q1 == *(u32*)post_q2 && *(u32*)pre_q2 == *(u32*)post_q1;
    result = *(u32*)pre_w1 == *(u32*)post_w2 && *(u32*)pre_w2 == *(u32*)post_w1;
    free(pre_q1);
    free(pre_q2);
    free(post_q1);
    free(post_q2);
)

/* COPY AND CMP */
TEST_ON_EMPTY_QUEUE (
    test_queue__copy_and_cmp_on_empty_queue,
    Queue u = queue__copy(q);
    Queue v = queue__copy(w);

    result = (queue__cmp(u, q, operator_match)
           && queue__cmp(v, w, operator_match)
           && COMPARE(queue__peek_nth, queue__length(q), u, q, true)
           && COMPARE(queue__peek_nth, queue__length(w), v, w, false)) ? TEST_SUCCESS : TEST_FAILURE;
    QUEUE_FREE(u, v, NULL, NULL);
)

TEST_ON_NON_EMPTY_QUEUE (
    test_queue__copy_and_cmp_on_non_empty_queue, false,
    Queue u = queue__copy(q);
    Queue v = queue__copy(w);

    result = (queue__cmp(u, q, operator_match)
           && queue__cmp(v, w, operator_match)
           && COMPARE(queue__peek_nth, queue__length(q), u, q, true)
           && COMPARE(queue__peek_nth, queue__length(w), v, w, false)) ? TEST_SUCCESS : TEST_FAILURE;
    QUEUE_FREE(u, v, NULL, NULL);
)

static bool test_queue__from_array(char debug)
{
    printf("%s... ", __func__);

    bool result;
    char A[5] = {'a', 'b', 'c', 'd', 'e'};
    char B[5] = {'f', 'g', 'h', 'i', 'j'};
    u32 C[5] = {1, 2, 3, 4, 5};
    QUEUE_CREATE(q_char, w_char);
    QUEUE_CREATE(q_u32, w_u32);

    result = (queue__from_array(q_char, A, 5, sizeof(char))
           && (w_char = queue__from_array(w_char, A, 5, sizeof(char)))
           && queue__from_array(q_char, B, 5, sizeof(char))
           && queue__from_array(w_char, B, 5, sizeof(char))
           && queue__from_array(q_u32, C, 5, sizeof(u32))
           && (w_u32 = queue__from_array(w_u32, C, 5, sizeof(u32)))
           && queue__length(q_char) == 10
           && queue__length(w_char) == 10
           && queue__length(q_u32) == 5
           && queue__length(w_u32) == 5) ? TEST_SUCCESS : TEST_FAILURE;

    result &= COMPARE2(queue__peek_nth, queue__length(q_u32), C, q_u32, true);
    result &= COMPARE2(queue__peek_nth, queue__length(w_u32), C, w_u32, false);

    QUEUE_DEBUG_char(q_char, w_char, "\n\tQueues after from_array:");
    QUEUE_DEBUG_u32(q_u32, w_u32, " ");

    QUEUE_FREE(q_char, w_char, q_u32, w_u32);
    return result;
}

/* DUMP */
TEST_ON_EMPTY_QUEUE (
    test_queue__dump_on_empty_queue,
    result = (queue__dump(q) == NULL && queue__dump(w) == NULL) ? TEST_SUCCESS : TEST_FAILURE;
)

TEST_ON_NON_EMPTY_QUEUE (
    test_queue__dump_on_non_empty_queue, true,
    A = queue__dump(q);
    B = queue__dump(w);

    result = (queue__is_empty(q) && queue__is_empty(w)) ? TEST_SUCCESS : TEST_FAILURE;

    for (u32 i = 0; i < N; i++) {
        result |= *(u32 *)A[i] != i || *(u32 *)B[i] != N;
    }
)

/* TO_ARRAY*/
TEST_ON_EMPTY_QUEUE (
    test_queue__to_array_on_empty_queue,
    result = (queue__to_array(q) == NULL && queue__to_array(w) == NULL) ? TEST_SUCCESS : TEST_FAILURE;
)

TEST_ON_NON_EMPTY_QUEUE (
    test_queue__to_array_on_non_empty_queue, true,
    A = queue__to_array(q);
    B = queue__to_array(w);

    result = (queue__length(q) == N && queue__length(w) == N) ? TEST_SUCCESS : TEST_FAILURE;

    for (u32 i = 0; i < N; i++) {
        result |= *(u32 *)A[i] != i || *(u32 *)B[i] != N;
    }
)

/* PTR_SEARCH AND PTR_CONTAINS */
TEST_ON_EMPTY_QUEUE (
    test_queue__ptr_search_and_ptr_contains_on_empty_queue,
    result &= !queue__ptr_contains(q, NULL) && queue__ptr_search(q, NULL) == SIZE_MAX;
    result &= !queue__ptr_contains(w, NULL) && queue__ptr_search(w, NULL) == SIZE_MAX;
)

TEST_ON_NON_EMPTY_QUEUE (
    test_queue__ptr_search_and_ptr_contains_on_non_empty_queue, true,
    result &= !queue__ptr_contains(q, NULL) && queue__ptr_search(q, NULL) == SIZE_MAX;
    result &= !queue__ptr_contains(w, NULL) && queue__ptr_search(w, NULL) == SIZE_MAX;

    for (u32 i = 0; i < N; i++) {
        result &= !queue__ptr_contains(q, elems + i) && queue__ptr_search(q, elems + i) == SIZE_MAX;
        result &= queue__ptr_contains(w, elems + i) && queue__ptr_search(w, elems + i) == i;
    }
)

/* SEARCH AND CONTAINS */
TEST_ON_EMPTY_QUEUE (
    test_queue__search_and_contains_on_empty_queue,
    result &= !queue__contains(q, NULL, operator_match) && queue__search(q, NULL, operator_match) == SIZE_MAX;
    result &= !queue__contains(w, NULL, operator_match) && queue__search(w, NULL, operator_match) == SIZE_MAX;
)

TEST_ON_NON_EMPTY_QUEUE (
    test_queue__search_and_contains_on_non_empty_queue, false,
    result &= !queue__contains(q, NULL, operator_match) && queue__search(q, NULL, operator_match) == SIZE_MAX;
    result &= !queue__contains(w, NULL, operator_match) && queue__search(w, NULL, operator_match) == SIZE_MAX;

    for (u32 i = 0; i < N; i++) {
        result &= queue__contains(q, elems + i, operator_match) && queue__search(q, elems + i, operator_match) == i;
        result &= queue__contains(w, elems + i, operator_match) && queue__search(w, elems + i, operator_match) == i;
    }
)

TEST_ON_EMPTY_QUEUE (
    test_queue__clean_NULL_on_empty_queue,
    queue__clean_NULL(q);
    queue__clean_NULL(w);
)

static bool test_queue__clean_NULL_on_non_empty_queue(char debug)
{
    printf("%s... ", __func__);

    bool result;
    QUEUE_CREATE(q, w);

    u32 N = 8;
    for (u32 i = 0; i < N; i++) {
        queue__enqueue(q, !(i%2) ? &i : NULL);
        queue__enqueue(w, !(i%2) ? &i : NULL);
    }

    QUEUE_DEBUG_u32(q, w, "\n\tQueues before clean_NULL:");

    queue__clean_NULL(q);
    queue__clean_NULL(w);

    result = (queue__length(q) == N>>1 && queue__length(w) == N>>1) ? TEST_SUCCESS : TEST_FAILURE;

    QUEUE_DEBUG_u32(q, w, "\n\tQueues after clean_NULL:");

    QUEUE_FREE(q, w, NULL, NULL);
    return result;
}

/* CLEAR */
TEST_ON_EMPTY_QUEUE (
    test_queue__clear_on_empty_queue,
    queue__clear(q);
    queue__clear(w);
)

TEST_ON_NON_EMPTY_QUEUE (
    test_queue__clear_on_non_empty_queue, true,
    queue__clear(q);
    queue__clear(w);

    result = (queue__is_empty(q) && queue__is_empty(w)) ? TEST_SUCCESS : TEST_FAILURE;
)

/* FOREACH */
TEST_ON_EMPTY_QUEUE (
    test_queue__foreach_on_empty_queue,
    int value = 1;
    queue__foreach(q, plus_op, &value);
    queue__foreach(w, plus_op, &value);
)

TEST_ON_NON_EMPTY_QUEUE (
    test_queue__foreach_on_non_empty_queue, false,
    u32 value = 1;
    queue__foreach(q, plus_op, &value);
    queue__foreach(w, plus_op, &value);

    result &= COMPARE3(queue__peek_nth, queue__length(q), q, value, true);
    result &= COMPARE3(queue__peek_nth, queue__length(w), w, value, false);
)

/* FILTER AND ALL */
TEST_ON_EMPTY_QUEUE (
    test_queue__filter_and_all_on_empty_queue,
    u32 value = 2;
    queue__filter(q, predicate, &value);
    queue__filter(w, predicate, &value);

    result = queue__all(q, predicate, &value) == 1 && queue__all(w, predicate, &value) == 1;
)

TEST_ON_NON_EMPTY_QUEUE (
    test_queue__filter_and_all_on_non_empty_queue, false,
    u32 value1 = 2;
    u32 value2 = 3;
    queue__filter(q, predicate, &value1);
    queue__filter(w, predicate, &value1);

    result &= queue__all(q, predicate, &value1) == 1 && queue__all(w, predicate, &value1) == 1;
    result &= queue__all(q, predicate, &value2) == 0 && queue__all(w, predicate, &value2) == 0;
)

/* ANY */
TEST_ON_EMPTY_QUEUE (
    test_queue__any_on_empty_queue,
    u32 value = 2;

    result = queue__any(q, predicate, &value) == 0 && queue__any(w, predicate, &value) == 0;
)

TEST_ON_NON_EMPTY_QUEUE (
    test_queue__any_on_non_empty_queue, false,
    u32 value1 = 2;
    u32 value2 = 3;

    result &= queue__any(q, predicate, &value1) == 1 && queue__any(w, predicate, &value1) == 1;
    result &= queue__any(q, predicate, &value2) == 1 && queue__any(w, predicate, &value2) == 1;
)

/* REVERSE */
TEST_ON_EMPTY_QUEUE (
    test_queue__reverse_on_empty_queue,
    queue__reverse(q);
    queue__reverse(w);
)

TEST_ON_NON_EMPTY_QUEUE (
    test_queue__reverse_on_non_empty_queue, false,
    queue__reverse(q);
    queue__reverse(w);

    result &= IS_REVERSE(queue__peek_nth, queue__length(q), q, true);
    result &= IS_REVERSE(queue__peek_nth, queue__length(w), w, false);
)

/* SHUFFLE */
TEST_ON_EMPTY_QUEUE (
    test_queue__shuffle_on_empty_queue,
    queue__shuffle(q, 1);
    queue__shuffle(w, 1);
)

TEST_ON_NON_EMPTY_QUEUE (
    test_queue__shuffle_on_non_empty_queue, true,
    queue__shuffle(q, 1);
    A = queue__to_array(q);

    result = TEST_FAILURE;
    for (u32 i = 0; i < N; i++) {
        result |= *(u32 *)A[i] != i;
    }
)

/* SORT */
TEST_ON_EMPTY_QUEUE (
    test_queue__sort_on_empty_queue,
    queue__sort(q, operator_compare);
    queue__sort(w, operator_compare);

    result &= IS_SORTED(queue__peek_nth, queue__length(q), q, true);
    result &= IS_SORTED(queue__peek_nth, queue__length(w), w, false);
)

TEST_ON_NON_EMPTY_QUEUE (
    test_queue__sort_on_non_empty_queue, true,
    queue__sort(q, operator_compare);
    queue__sort(w, operator_compare);

    result &= IS_SORTED(queue__peek_nth, queue__length(q), q, true);
    result &= IS_SORTED(queue__peek_nth, queue__length(w), w, false);
)


int main(void)
{
    int nb_success = 0;
    int nb_tests = 0;
    printf("----------- TEST QUEUE -----------\n");

    print_test_result(test_queue__empty_copy_disabled(false), &nb_success, &nb_tests);
    print_test_result(test_queue__empty_copy_enabled(false), &nb_success, &nb_tests);
    print_test_result(test_queue__is_copy_enabled(), &nb_success, &nb_tests);
    print_test_result(test_queue__length(false), &nb_success, &nb_tests);
    print_test_result(test_queue__is_empty_on_empty_queue(false), &nb_success, &nb_tests);
    print_test_result(test_queue__is_empty_on_non_empty_queue(false), &nb_success, &nb_tests);

    print_test_result(test_queue__enqueue(false), &nb_success, &nb_tests);
    print_test_result(test_queue__dequeue_on_empty_queue(false), &nb_success, &nb_tests);
    print_test_result(test_queue__dequeue_on_non_empty_queue(false), &nb_success, &nb_tests);
    print_test_result(test_queue__remove_nth_on_empty_queue(false), &nb_success, &nb_tests);
    print_test_result(test_queue__remove_nth_on_non_empty_queue(false), &nb_success, &nb_tests);
    print_test_result(test_queue__peek_front_on_empty_queue(false), &nb_success, &nb_tests);
    print_test_result(test_queue__peek_front_on_non_empty_queue(false), &nb_success, &nb_tests);
    print_test_result(test_queue__peek_back_on_empty_queue(false), &nb_success, &nb_tests);
    print_test_result(test_queue__peek_back_on_non_empty_queue(false), &nb_success, &nb_tests);
    print_test_result(test_queue__peek_nth_on_empty_queue(false), &nb_success, &nb_tests);
    print_test_result(test_queue__peek_nth_on_non_empty_queue(false), &nb_success, &nb_tests);
    print_test_result(test_queue__swap_on_empty_queue(false), &nb_success, &nb_tests);
    print_test_result(test_queue__swap_on_non_empty_queue(false), &nb_success, &nb_tests);

    print_test_result(test_queue__copy_and_cmp_on_empty_queue(false), &nb_success, &nb_tests);
    print_test_result(test_queue__copy_and_cmp_on_non_empty_queue(false), &nb_success, &nb_tests);
    print_test_result(test_queue__from_array(false), &nb_success, &nb_tests);
    print_test_result(test_queue__dump_on_empty_queue(false), &nb_success, &nb_tests);
    print_test_result(test_queue__dump_on_non_empty_queue(false), &nb_success, &nb_tests);
    print_test_result(test_queue__to_array_on_empty_queue(false), &nb_success, &nb_tests);
    print_test_result(test_queue__to_array_on_non_empty_queue(false), &nb_success, &nb_tests);
    print_test_result(test_queue__ptr_search_and_ptr_contains_on_empty_queue(false), &nb_success, &nb_tests);
    print_test_result(test_queue__ptr_search_and_ptr_contains_on_non_empty_queue(false), &nb_success, &nb_tests);
    print_test_result(test_queue__search_and_contains_on_empty_queue(false), &nb_success, &nb_tests);
    print_test_result(test_queue__search_and_contains_on_non_empty_queue(false), &nb_success, &nb_tests);
    print_test_result(test_queue__clean_NULL_on_empty_queue(false), &nb_success, &nb_tests);
    print_test_result(test_queue__clean_NULL_on_non_empty_queue(false), &nb_success, &nb_tests);
    print_test_result(test_queue__clear_on_empty_queue(false), &nb_success, &nb_tests);
    print_test_result(test_queue__clear_on_non_empty_queue(false), &nb_success, &nb_tests);

    print_test_result(test_queue__foreach_on_empty_queue(false), &nb_success, &nb_tests);
    print_test_result(test_queue__foreach_on_non_empty_queue(false), &nb_success, &nb_tests);
    print_test_result(test_queue__filter_and_all_on_empty_queue(false), &nb_success, &nb_tests);
    print_test_result(test_queue__filter_and_all_on_non_empty_queue(false), &nb_success, &nb_tests);
    print_test_result(test_queue__any_on_empty_queue(false), &nb_success, &nb_tests);
    print_test_result(test_queue__any_on_non_empty_queue(false), &nb_success, &nb_tests);
    print_test_result(test_queue__reverse_on_empty_queue(false), &nb_success, &nb_tests);
    print_test_result(test_queue__reverse_on_non_empty_queue(false), &nb_success, &nb_tests);
    print_test_result(test_queue__shuffle_on_empty_queue(false), &nb_success, &nb_tests);
    print_test_result(test_queue__shuffle_on_non_empty_queue(false), &nb_success, &nb_tests);
    print_test_result(test_queue__sort_on_empty_queue(false), &nb_success, &nb_tests);
    print_test_result(test_queue__sort_on_non_empty_queue(false), &nb_success, &nb_tests);

    print_test_summary(nb_success, nb_tests);

    return TEST_SUCCESS;
}
