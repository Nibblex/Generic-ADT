#include "common_tests_utils.h"
#include "../queue/queue.h"
#include "../common/defs.h"

#define QUEUE_CREATE(A, B) \
    Queue A = NULL, B = NULL; \
    A = queue__empty_copy_enabled(operator_copy, operator_delete); \
    B = queue__empty_copy_disabled()

#define QUEUE_ENQUEUE_u32(N, A, B) \
    ADD_u32(queue__enqueue, N, A, B)

#define QUEUE_ENQUEUE_i32_RAND(N, A, B) \
    ADD_i32_RAND(queue__enqueue, N, A, B)

#define QUEUE_DEBUG_char(A, B, C) \
    DEBUG_char(queue__debug, A, B, C)

#define QUEUE_DEBUG_i32(A, B, C) \
    DEBUG_i32(queue__debug, A, B, C)

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
    QUEUE_DEBUG_i32(q, w, "\n\tQueues after:"); \
    QUEUE_FREE(q, w, NULL, NULL); \
    return result && __empty_assertion; \
}

#define TEST_ON_NON_EMPTY_QUEUE(__name, __expr) \
static bool __name(char debug) \
{ \
    printf("%s... ", __func__); \
    bool result; \
    elem_t *A = NULL, *B = NULL; \
    Queue q = queue__empty_copy_enabled(operator_copy, operator_delete); \
    Queue w = queue__empty_copy_disabled(); \
    u32 N = 8; \
    QUEUE_ENQUEUE_i32_RAND(N, q, w); \
    QUEUE_DEBUG_i32(q, w, "\n\tQueues before:"); \
    __expr \
    QUEUE_DEBUG_i32(q, w, "\n\tQueues after:"); \
    for (u32 i = 0; i < N; i++) { \
        free(A[i]); \
    } \
    free(A); \
    free(B); \
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

static bool test_queue__size(void)
{
    printf("%s... ", __func__);

    bool result;
    QUEUE_CREATE(q, w);

    u32 N = 8;
    QUEUE_ENQUEUE_u32(N, q, w);

    result = (queue__size(q) == N && queue__size(w) == N) ? TEST_SUCCESS : TEST_FAILURE;

    QUEUE_FREE(q, w, NULL, NULL);
    return result;
}

static bool test_queue__is_empty_on_empty_queue(void)
{
    printf("%s... ", __func__);

    bool result;
    QUEUE_CREATE(q, w);

    result = (queue__is_empty(q) && queue__is_empty(w)) ? TEST_SUCCESS : TEST_FAILURE;

    QUEUE_FREE(q, w, NULL, NULL);
    return result;
}

static bool test_queue__is_empty_on_non_empty_queue(void)
{
    printf("%s... ", __func__);

    bool result;
    QUEUE_CREATE(q, w);

    QUEUE_ENQUEUE_u32(1, q, w);

    result = (!queue__is_empty(q) && !queue__is_empty(w)) ? TEST_SUCCESS : TEST_FAILURE;

    QUEUE_FREE(q, w, NULL, NULL);
    return result;
}

static bool test_queue__enqueue_on_empty_queue(char debug)
{
    printf("%s... ", __func__);

    bool result;
    QUEUE_CREATE(q, w);

    result = (!queue__enqueue(q, NULL)
           && !queue__enqueue(w, NULL)
           && queue__size(q) == 1
           && queue__size(w) == 1) ? TEST_SUCCESS : TEST_FAILURE;

    QUEUE_DEBUG_i32(q, w, "\n\tQueues after enqueue:");

    QUEUE_FREE(q, w, NULL, NULL);
    return result;
}

static bool test_queue__enqueue_on_non_empty_queue(char debug)
{
    printf("%s... ", __func__);

    bool result;
    QUEUE_CREATE(q, w);

    u32 N = 8;
    QUEUE_ENQUEUE_u32(N, q, w);

    result = (queue__size(q) == N && queue__size(w) == N) ? TEST_SUCCESS : TEST_FAILURE;

    QUEUE_DEBUG_i32(q, w, "\n\tQueues after enqueue:");

    QUEUE_FREE(q, w, NULL, NULL);
    return result;
}

TEST_ON_EMPTY_QUEUE (
    test_queue__dequeue_on_empty_queue,
    result = (queue__dequeue(q, NULL) == -1 && queue__dequeue(w, NULL) == -1) ? TEST_SUCCESS : TEST_FAILURE;
)

static bool test_queue__dequeue_on_non_empty_queue(char debug)
{
    printf("%s... ", __func__);

    bool result;
    elem_t front_q = NULL;
    elem_t front_w = NULL;
    QUEUE_CREATE(q, w);

    u32 N = 8;
    QUEUE_ENQUEUE_u32(N, q, w);

    QUEUE_DEBUG_i32(q, w, "\n\tQueues before dequeue:");

    result = TEST_SUCCESS;
    for (u32 i = 0; i < N; i++) {
        result |= queue__dequeue(q, &front_q)
               || queue__dequeue(w, &front_w)
               || queue__size(q) != N-i-1
               || queue__size(w) != N-i-1
               || *(u32 *)front_q != i
               || *(u32 *)front_w != N;
        free(front_q);
    }

    QUEUE_DEBUG_i32(q, w, "\n\tQueues after dequeue:");

    QUEUE_FREE(q, w, NULL, NULL);
    return result;
}

static bool test_queue__enqueue_and_dequeue_on_multiple_elements(char debug)
{
    printf("%s... ", __func__);

    bool result;
    QUEUE_CREATE(q, w);

    QUEUE_ENQUEUE_u32(60, q, w);

    result = (queue__size(q) == 60 && queue__size(w) == 60) ? TEST_SUCCESS : TEST_FAILURE;

    QUEUE_DEBUG_i32(q, w, "\nAdded 60 elements, expected capacity of 64 and size of 60\n");

    for (u32 i = 0; i < 40; i++) {
        queue__dequeue(q, NULL);
        queue__dequeue(w, NULL);
    }

    result |= queue__size(q) != 20 || queue__size(w) != 20;

    QUEUE_DEBUG_i32(q, w, "\nRemoved 40 elements, expected capacity of 32 and size of 20\n");

    QUEUE_ENQUEUE_u32(20, q, w);

    result |= queue__size(q) != 40 || queue__size(w) != 40;

    QUEUE_DEBUG_i32(q, w, "\nAdded 20 elements, expected capacity of 64 and size of 40\n");

    QUEUE_FREE(q, w, NULL, NULL);
    return result;
}

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

    QUEUE_DEBUG_i32(q, w, "\n\tQueues before clean_NULL:");

    queue__clean_NULL(q);
    queue__clean_NULL(w);

    result = (queue__size(q) == N>>1 && queue__size(w) == N>>1) ? TEST_SUCCESS : TEST_FAILURE;

    QUEUE_DEBUG_i32(q, w, "\n\tQueues after clean_NULL:");

    QUEUE_FREE(q, w, NULL, NULL);
    return result;
}

TEST_ON_EMPTY_QUEUE (
    test_queue__clear_on_empty_queue,
    queue__clear(q);
    queue__clear(w);
)

static bool test_queue__clear_on_non_empty_queue(char debug)
{
    printf("%s... ", __func__);

    bool result;
    QUEUE_CREATE(q, w);

    QUEUE_ENQUEUE_u32(5, q, w);

    QUEUE_DEBUG_i32(q, w, "\n\tQueues before clear:");

    queue__clear(q);
    queue__clear(w);

    result = (queue__is_empty(q) && queue__is_empty(w)) ? TEST_SUCCESS : TEST_FAILURE;

    QUEUE_DEBUG_i32(q, w, "\n\tQueues after clear:");

    QUEUE_FREE(q, w, NULL, NULL);
    return result;
}

TEST_ON_EMPTY_QUEUE (
    test_queue__peek_front_on_empty_queue,
    result = (queue__peek_front(q, NULL) == -1 && queue__peek_front(w, NULL) == -1) ? TEST_SUCCESS : TEST_FAILURE;
)

static bool test_queue__peek_front_on_non_empty_queue(char debug)
{
    printf("%s... ", __func__);

    bool result;
    int value1 = 5;
    int value2 = 7;
    elem_t front_q = NULL;
    elem_t front_w = NULL;
    QUEUE_CREATE(q, w);

    queue__enqueue(q, &value1);
    queue__enqueue(w, &value1);
    queue__enqueue(q, &value2);
    queue__enqueue(w, &value2);

    QUEUE_DEBUG_i32(q, w, "\n\tQueues before peek_front:");

    result = (!queue__peek_front(q, &front_q)
           && !queue__peek_front(w, &front_w)
           && !queue__is_empty(q)
           && !queue__is_empty(w)
           && *(int *)front_q == 5
           && *(int *)front_w == 5) ? TEST_SUCCESS : TEST_FAILURE;

    QUEUE_DEBUG_i32(q, w, "\n\tQueues after peek_front:");

    free(front_q);
    QUEUE_FREE(q, w, NULL, NULL);
    return result;
}

TEST_ON_EMPTY_QUEUE (
    test_queue__peek_back_on_empty_queue,
    result = (queue__peek_back(q, NULL) == -1 && queue__peek_back(w, NULL) == -1) ? TEST_SUCCESS : TEST_FAILURE;
)

static bool test_queue__peek_back_on_non_empty_queue(char debug)
{
    printf("%s... ", __func__);

    bool result;
    int value1 = 5;
    int value2 = 7;
    elem_t back_q = NULL;
    elem_t back_w = NULL;
    QUEUE_CREATE(q, w);

    queue__enqueue(q, &value1);
    queue__enqueue(w, &value1);
    queue__enqueue(q, &value2);
    queue__enqueue(w, &value2);

    QUEUE_DEBUG_i32(q, w, "\n\tQueues before peek_back:");

    result = (!queue__peek_back(q, &back_q)
           && !queue__peek_back(w, &back_w)
           && !queue__is_empty(q)
           && !queue__is_empty(w)
           && *(int *)back_q == 7
           && *(int *)back_w == 7) ? TEST_SUCCESS : TEST_FAILURE;

    QUEUE_DEBUG_i32(q, w, "\n\tQueues after peek_back:");

    free(back_q);
    QUEUE_FREE(q, w, NULL, NULL);
    return result;
}

TEST_ON_EMPTY_QUEUE (
    test_queue__peek_nth_on_empty_queue,
    result = (queue__peek_nth(q, NULL, 0) == -1 && queue__peek_nth(w, NULL, 0) == -1) ? TEST_SUCCESS : TEST_FAILURE;
)

static bool test_queue__peek_nth_on_non_empty_queue(char debug)
{
    printf("%s... ", __func__);

    bool result;
    int value1 = 5;
    int value2 = 7;
    elem_t nth_s = NULL;
    elem_t nth_t = NULL;
    QUEUE_CREATE(q, w);

    queue__enqueue(q, &value1);
    queue__enqueue(w, &value1);
    queue__enqueue(q, &value2);
    queue__enqueue(w, &value2);

    QUEUE_DEBUG_i32(q, w, "\n\tQueues before peek_nth:");

    result = (!queue__peek_nth(q, &nth_s, 0)
           && !queue__peek_nth(w, &nth_t, 0)
           && !queue__is_empty(q)
           && !queue__is_empty(w)
           && *(int *)nth_s == 5
           && *(int *)nth_t == 5) ? TEST_SUCCESS : TEST_FAILURE;
    free(nth_s);

    result &= (!queue__peek_nth(q, &nth_s, 1)
            && !queue__peek_nth(w, &nth_t, 1)
            && !queue__is_empty(q)
            && !queue__is_empty(w)
            && *(int *)nth_s == 7
            && *(int *)nth_t == 7) ? TEST_SUCCESS : TEST_FAILURE;
    free(nth_s);

    QUEUE_DEBUG_i32(q, w, "\n\tQueues after peek_nth:");

    QUEUE_FREE(q, w, NULL, NULL);
    return result;
}

static bool test_queue__from_array(char debug)
{
    printf("%s... ", __func__);

    bool result;
    char A[5] = {'a', 'b', 'c', 'd', 'e'};
    char B[5] = {'f', 'g', 'h', 'i', 'j'};
    int C[5] = {1, 2, 3, 4, 5};
    elem_t *D = NULL;
    QUEUE_CREATE(q_char, w_char);
    QUEUE_CREATE(q_int, w_int);

    result = (queue__from_array(q_char, A, 5, sizeof(char))
           && (w_char = queue__from_array(w_char, A, 5, sizeof(char)))
           && queue__from_array(q_char, B, 5, sizeof(char))
           && queue__from_array(w_char, B, 5, sizeof(char))
           && queue__from_array(q_int, C, 5, sizeof(int))
           && (w_int = queue__from_array(w_int, C, 5, sizeof(int)))
           && queue__size(q_char) == 10
           && queue__size(w_char) == 10
           && queue__size(q_int) == 5
           && queue__size(w_int) == 5) ? TEST_SUCCESS : TEST_FAILURE;

    D = queue__to_array(w_int);

    for (u32 i = 0; i < 5; i++) {
        result |= C[i] != *(int *)D[i];
    }

    QUEUE_DEBUG_char(q_char, w_char, "\n\tQueues after from_array:");
    QUEUE_DEBUG_i32(q_int, w_int, " ");

    free(D);
    QUEUE_FREE(q_char, w_char, q_int, w_int);
    return result;
}

TEST_ON_EMPTY_QUEUE (
    test_queue__dump_on_empty_queue,
    result = (queue__dump(q) == NULL && queue__dump(w) == NULL) ? TEST_SUCCESS : TEST_FAILURE;
)

TEST_ON_NON_EMPTY_QUEUE (
    test_queue__dump_on_non_empty_queue,
    A = queue__dump(q);
    B = queue__dump(w);

    result = (queue__is_empty(q) && queue__is_empty(w)) ? TEST_SUCCESS : TEST_FAILURE;

    for (u32 i = 0; i < N; i++) {
        result |= *(u32 *)A[i] != i || *(u32 *)B[i] != N;
    }
)

TEST_ON_EMPTY_QUEUE (
    test_queue__to_array_on_empty_queue,
    result = (queue__to_array(q) == NULL && queue__to_array(w) == NULL) ? TEST_SUCCESS : TEST_FAILURE;
)

TEST_ON_NON_EMPTY_QUEUE (
    test_queue__to_array_on_non_empty_queue,
    A = queue__to_array(q);
    B = queue__to_array(w);

    result = (queue__size(q) == N && queue__size(w) == N) ? TEST_SUCCESS : TEST_FAILURE;

    for (u32 i = 0; i < N; i++) {
        result |= *(u32 *)A[i] != i || *(u32 *)B[i] != N;
    }
)

TEST_ON_EMPTY_QUEUE (
    test_queue__foreach_on_empty_queue,
    int value = 1;
    queue__foreach(q, plus_op, &value);
    queue__foreach(w, plus_op, &value);
)

TEST_ON_NON_EMPTY_QUEUE (
    test_queue__foreach_on_non_empty_queue,
    u32 value = 1;
    queue__foreach(q, plus_op, &value);
    queue__foreach(w, plus_op, &value);

    A = queue__to_array(q);
    B = queue__to_array(w);

    result = TEST_SUCCESS;
    for (u32 i = 0; i < N; i++) {
        result |= i + value == *(u32 *)A[i] && N<<1 == *(u32 *)B[i];
    }
)

TEST_ON_EMPTY_QUEUE (
    test_queue__shuffle_on_empty_queue,
    queue__shuffle(q);
    queue__shuffle(w);
)

TEST_ON_NON_EMPTY_QUEUE (
    test_queue__shuffle_on_non_empty_queue,
    queue__shuffle(q);
    A = queue__to_array(q);

    result = TEST_FAILURE;
    for (u32 i = 0; i < N; i++) {
        result |= *(u32 *)A[i] != i;
    }
)

TEST_ON_EMPTY_QUEUE (
    test_queue__sort_on_empty_queue,
    queue__sort(q, operator_compare);
    queue__sort(w, operator_compare);
)

TEST_ON_NON_EMPTY_QUEUE (
    test_queue__sort_on_non_empty_queue,
    queue__sort(q, operator_compare);
    A = queue__to_array(q);

    result = TEST_SUCCESS;
    for (u32 i = 0; i < N - 1; i++) {
        result |= *(int *)A[i] > *(int *)A[i+1];
    }
)


int main(void)
{
    int nb_success = 0;
    int nb_tests = 0;
    printf("----------- TEST QUEUE -----------\n");

    print_test_result(test_queue__empty_copy_disabled(false), &nb_success, &nb_tests);
    print_test_result(test_queue__empty_copy_enabled(false), &nb_success, &nb_tests);
    print_test_result(test_queue__is_copy_enabled(), &nb_success, &nb_tests);
    print_test_result(test_queue__size(), &nb_success, &nb_tests);
    print_test_result(test_queue__is_empty_on_empty_queue(), &nb_success, &nb_tests);
    print_test_result(test_queue__is_empty_on_non_empty_queue(), &nb_success, &nb_tests);

    print_test_result(test_queue__enqueue_on_empty_queue(false), &nb_success, &nb_tests);
    print_test_result(test_queue__enqueue_on_non_empty_queue(false), &nb_success, &nb_tests);
    print_test_result(test_queue__dequeue_on_empty_queue(false), &nb_success, &nb_tests);
    print_test_result(test_queue__dequeue_on_non_empty_queue(false), &nb_success, &nb_tests);
    print_test_result(test_queue__enqueue_and_dequeue_on_multiple_elements(false), &nb_success, &nb_tests);

    print_test_result(test_queue__clean_NULL_on_empty_queue(false), &nb_success, &nb_tests);
    print_test_result(test_queue__clean_NULL_on_non_empty_queue(false), &nb_success, &nb_tests);
    print_test_result(test_queue__clear_on_empty_queue(false), &nb_success, &nb_tests);
    print_test_result(test_queue__clear_on_non_empty_queue(false), &nb_success, &nb_tests);
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
    print_test_result(test_queue__foreach_on_empty_queue(false), &nb_success, &nb_tests);
    print_test_result(test_queue__foreach_on_non_empty_queue(false), &nb_success, &nb_tests);
    print_test_result(test_queue__shuffle_on_empty_queue(false), &nb_success, &nb_tests);
    print_test_result(test_queue__shuffle_on_non_empty_queue(false), &nb_success, &nb_tests);
    print_test_result(test_queue__sort_on_empty_queue(false), &nb_success, &nb_tests);
    print_test_result(test_queue__sort_on_non_empty_queue(false), &nb_success, &nb_tests);

    print_test_summary(nb_success, nb_tests);

    return TEST_SUCCESS;
}
