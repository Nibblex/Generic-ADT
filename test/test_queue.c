#include "common_tests_utils.h"
#include "../queue/queue.h"
#include "../common/defs.h"

#define QUEUE_CREATE(A, B) \
    Queue A = NULL, B = NULL; \
    A = queue__empty_copy_enabled(operator_copy, operator_delete); \
    B = queue__empty_copy_disabled()

#define QUEUE_ENQUEUE_u32(N, __elems, A, B) \
    ADD_u32(queue__enqueue, N, __elems, A, B)

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
    QUEUE_ENQUEUE_u32(N, elems, q, w); \
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
    test_queue__size, true,
    result = (queue__size(q) == N && queue__size(w) == N) ? TEST_SUCCESS : TEST_FAILURE;
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
    result = (queue__size(q) == N && queue__size(w) == N) ? TEST_SUCCESS : TEST_FAILURE;
    result &= COMPARE2(queue__peek_nth, queue__size(q), elems, q, true);
    result &= COMPARE2(queue__peek_nth, queue__size(w), elems, w, false);
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
               && queue__size(q) == N-i-1
               && queue__size(w) == N-i-1
               && *(u32 *)front_q == i
               && *(u32 *)front_w == i;
        free(front_q);
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
    result = (queue__peek_nth(q, NULL, 0) == -1 && queue__peek_nth(w, NULL, 0) == -1) ? TEST_SUCCESS : TEST_FAILURE;
)

TEST_ON_NON_EMPTY_QUEUE (
    test_queue__peek_nth_on_non_empty_queue, false,
    elem_t nth_q = NULL;
    elem_t nth_w = NULL;
    result = (!queue__peek_nth(q, &nth_q, N>>1)
           && !queue__peek_nth(w, &nth_w, N>>1)
           && !queue__is_empty(q)
           && !queue__is_empty(w)
           && *(u32 *)nth_q == N>>1
           && *(u32 *)nth_w == N>>1) ? TEST_SUCCESS : TEST_FAILURE;
    free(nth_q);
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
           && queue__size(q_char) == 10
           && queue__size(w_char) == 10
           && queue__size(q_u32) == 5
           && queue__size(w_u32) == 5) ? TEST_SUCCESS : TEST_FAILURE;

    result &= COMPARE2(queue__peek_nth, queue__size(q_u32), C, q_u32, true);
    result &= COMPARE2(queue__peek_nth, queue__size(w_u32), C, w_u32, false);

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

    result = (queue__size(q) == N && queue__size(w) == N) ? TEST_SUCCESS : TEST_FAILURE;

    for (u32 i = 0; i < N; i++) {
        result |= *(u32 *)A[i] != i || *(u32 *)B[i] != N;
    }
)

/* COPY AND CMP */
TEST_ON_EMPTY_QUEUE (
    test_queue__copy_and_cmp_on_empty_queue,
    Queue u = queue__copy(q);
    Queue v = queue__copy(w);

    result = (queue__cmp(u, q, operator_compare)
           && queue__cmp(v, w, operator_compare)
           && COMPARE(queue__peek_nth, queue__size(q), u, q, true)
           && COMPARE(queue__peek_nth, queue__size(w), v, w, false)) ? TEST_SUCCESS : TEST_FAILURE;
    QUEUE_FREE(u, v, NULL, NULL);
)

TEST_ON_NON_EMPTY_QUEUE (
    test_queue__copy_and_cmp_on_non_empty_queue, false,
    Queue u = queue__copy(q);
    Queue v = queue__copy(w);

    result = (queue__cmp(u, q, operator_compare)
           && queue__cmp(v, w, operator_compare)
           && COMPARE(queue__peek_nth, queue__size(q), u, q, true)
           && COMPARE(queue__peek_nth, queue__size(w), v, w, false)) ? TEST_SUCCESS : TEST_FAILURE;
    QUEUE_FREE(u, v, NULL, NULL);
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

    result = (queue__size(q) == N>>1 && queue__size(w) == N>>1) ? TEST_SUCCESS : TEST_FAILURE;

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

    result &= COMPARE3(queue__peek_nth, queue__size(q), q, value, true);
    result &= COMPARE3(queue__peek_nth, queue__size(w), w, value, false);
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

    result &= IS_REVERSE(queue__peek_nth, queue__size(q), q, true);
    result &= IS_REVERSE(queue__peek_nth, queue__size(w), w, false);
)

/* SHUFFLE */
TEST_ON_EMPTY_QUEUE (
    test_queue__shuffle_on_empty_queue,
    queue__shuffle(q);
    queue__shuffle(w);
)

TEST_ON_NON_EMPTY_QUEUE (
    test_queue__shuffle_on_non_empty_queue, true,
    queue__shuffle(q);
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

    result &= IS_SORTED(queue__peek_nth, queue__size(q), q, true);
    result &= IS_SORTED(queue__peek_nth, queue__size(w), w, false);
)

TEST_ON_NON_EMPTY_QUEUE (
    test_queue__sort_on_non_empty_queue, true,
    queue__sort(q, operator_compare);
    queue__sort(w, operator_compare);

    result &= IS_SORTED(queue__peek_nth, queue__size(q), q, true);
    result &= IS_SORTED(queue__peek_nth, queue__size(w), w, false);
)


int main(void)
{
    int nb_success = 0;
    int nb_tests = 0;
    printf("----------- TEST QUEUE -----------\n");

    print_test_result(test_queue__empty_copy_disabled(false), &nb_success, &nb_tests);
    print_test_result(test_queue__empty_copy_enabled(false), &nb_success, &nb_tests);
    print_test_result(test_queue__is_copy_enabled(), &nb_success, &nb_tests);
    print_test_result(test_queue__size(false), &nb_success, &nb_tests);
    print_test_result(test_queue__is_empty_on_empty_queue(false), &nb_success, &nb_tests);
    print_test_result(test_queue__is_empty_on_non_empty_queue(false), &nb_success, &nb_tests);

    print_test_result(test_queue__enqueue(false), &nb_success, &nb_tests);
    print_test_result(test_queue__dequeue_on_empty_queue(false), &nb_success, &nb_tests);
    print_test_result(test_queue__dequeue_on_non_empty_queue(false), &nb_success, &nb_tests);
    print_test_result(test_queue__peek_front_on_empty_queue(false), &nb_success, &nb_tests);
    print_test_result(test_queue__peek_front_on_non_empty_queue(false), &nb_success, &nb_tests);
    print_test_result(test_queue__peek_back_on_empty_queue(false), &nb_success, &nb_tests);
    print_test_result(test_queue__peek_back_on_non_empty_queue(false), &nb_success, &nb_tests);
    print_test_result(test_queue__peek_nth_on_empty_queue(false), &nb_success, &nb_tests);
    print_test_result(test_queue__peek_nth_on_non_empty_queue(false), &nb_success, &nb_tests);

    print_test_result(test_queue__from_array(false), &nb_success, &nb_tests);
    print_test_result(test_queue__dump_on_empty_queue(false), &nb_success, &nb_tests);
    print_test_result(test_queue__dump_on_non_empty_queue(false), &nb_success, &nb_tests);
    print_test_result(test_queue__to_array_on_empty_queue(false), &nb_success, &nb_tests);
    print_test_result(test_queue__to_array_on_non_empty_queue(false), &nb_success, &nb_tests);
    print_test_result(test_queue__copy_and_cmp_on_empty_queue(false), &nb_success, &nb_tests);
    print_test_result(test_queue__copy_and_cmp_on_non_empty_queue(false), &nb_success, &nb_tests);
    print_test_result(test_queue__clean_NULL_on_empty_queue(false), &nb_success, &nb_tests);
    print_test_result(test_queue__clean_NULL_on_non_empty_queue(false), &nb_success, &nb_tests);
    print_test_result(test_queue__clear_on_empty_queue(false), &nb_success, &nb_tests);
    print_test_result(test_queue__clear_on_non_empty_queue(false), &nb_success, &nb_tests);

    print_test_result(test_queue__foreach_on_empty_queue(false), &nb_success, &nb_tests);
    print_test_result(test_queue__foreach_on_non_empty_queue(false), &nb_success, &nb_tests);
    print_test_result(test_queue__reverse_on_empty_queue(false), &nb_success, &nb_tests);
    print_test_result(test_queue__reverse_on_non_empty_queue(false), &nb_success, &nb_tests);
    print_test_result(test_queue__shuffle_on_empty_queue(false), &nb_success, &nb_tests);
    print_test_result(test_queue__shuffle_on_non_empty_queue(false), &nb_success, &nb_tests);
    print_test_result(test_queue__sort_on_empty_queue(false), &nb_success, &nb_tests);
    print_test_result(test_queue__sort_on_non_empty_queue(false), &nb_success, &nb_tests);

    print_test_summary(nb_success, nb_tests);

    return TEST_SUCCESS;
}
