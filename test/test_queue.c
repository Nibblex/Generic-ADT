#include "common_tests_utils.h"
#include "../queue/queue.h"
#include "../common/defs.h"

#define QUEUE_DEBUG_char(A, B, C) \
    DEBUG_char(A, B, C, queue__debug)

#define QUEUE_DEBUG_i32(A, B, C) \
    DEBUG_i32(A, B, C, queue__debug)

#define QUEUE_FREE(A, B, C, D) \
    queue__free(A); \
    queue__free(B); \
    queue__free(C); \
    queue__free(D);

////////////////////////////////////////////////////////////////////
///     TEST SUITE
////////////////////////////////////////////////////////////////////

static bool test_queue__empty_copy_enabled(char debug)
{
    printf("%s... ", __func__);

    bool result;
    Queue q = queue__empty_copy_enabled((copy_operator_t)operator_copy, (delete_operator_t)operator_delete);

    result = q ? TEST_SUCCESS : TEST_FAILURE;

    if (debug) queue__debug(q, (void (*)(elem_t))operator_debug_i32);

    QUEUE_FREE(q, NULL, NULL, NULL)
    return result;
}

static bool test_queue__empty_copy_disabled(char debug)
{
    printf("%s... ", __func__);

    bool result;
    Queue q = queue__empty_copy_disabled();

    result = q ? TEST_SUCCESS : TEST_FAILURE;

    if (debug) queue__debug(q, (void (*)(elem_t))operator_debug_i32);

    QUEUE_FREE(q, NULL, NULL, NULL)
    return result;
}

static bool test_queue__is_copy_enabled(void)
{
    printf("%s... ", __func__);

    bool result;
    Queue q = queue__empty_copy_enabled((copy_operator_t)operator_copy, (delete_operator_t)operator_delete);
    Queue w = queue__empty_copy_disabled();

    result = (queue__is_copy_enabled(q) && !queue__is_copy_enabled(w)) ? TEST_SUCCESS : TEST_FAILURE;

    QUEUE_FREE(q, w, NULL, NULL)
    return result;
}

static bool test_queue__size(void)
{
    printf("%s... ", __func__);

    bool result;
    Queue q = queue__empty_copy_enabled((copy_operator_t)operator_copy, (delete_operator_t)operator_delete);
    Queue w = queue__empty_copy_disabled();

    queue__enqueue(q, NULL);
    queue__enqueue(w, NULL);
    queue__enqueue(q, NULL);
    queue__enqueue(w, NULL);

    result = (queue__size(q) == 2 && queue__size(w) == 2) ? TEST_SUCCESS : TEST_FAILURE;

    QUEUE_FREE(q, w, NULL, NULL)
    return result;
}

static bool test_queue__is_empty_on_empty_queue(void)
{
    printf("%s... ", __func__);

    bool result;
    Queue q = queue__empty_copy_enabled((copy_operator_t)operator_copy, (delete_operator_t)operator_delete);
    Queue w = queue__empty_copy_disabled();

    result = (queue__is_empty(q) && queue__is_empty(w)) ? TEST_SUCCESS : TEST_FAILURE;

    QUEUE_FREE(q, w, NULL, NULL)
    return result;
}

static bool test_queue__is_empty_on_non_empty_queue(void)
{
    printf("%s... ", __func__);

    bool result;
    Queue q = queue__empty_copy_enabled((copy_operator_t)operator_copy, (delete_operator_t)operator_delete);
    Queue w = queue__empty_copy_disabled();

    queue__enqueue(q, NULL);
    queue__enqueue(w, NULL);

    result = (!queue__is_empty(q) && !queue__is_empty(w)) ? TEST_SUCCESS : TEST_FAILURE;

    QUEUE_FREE(q, w, NULL, NULL)
    return result;
}

static bool test_queue__enqueue_on_empty_queue(char debug)
{
    printf("%s... ", __func__);

    bool result;
    Queue q = queue__empty_copy_enabled((copy_operator_t)operator_copy, (delete_operator_t)operator_delete);
    Queue w = queue__empty_copy_disabled();

    result = (!queue__enqueue(q, NULL)
           && !queue__enqueue(w, NULL)
           && queue__size(q) == 1
           && queue__size(w) == 1) ? TEST_SUCCESS : TEST_FAILURE;

    QUEUE_DEBUG_i32(q, w, "\n\tQueues after enqueue:")

    QUEUE_FREE(q, w, NULL, NULL)
    return result;
}

static bool test_queue__enqueue_on_non_empty_queue(char debug)
{
    printf("%s... ", __func__);

    bool result;
    u32 N = 5;
    Queue q = queue__empty_copy_enabled((copy_operator_t)operator_copy, (delete_operator_t)operator_delete);
    Queue w = queue__empty_copy_disabled();

    for (u32 i = 0; i < N; i++) {
        queue__enqueue(q, &i);
        queue__enqueue(w, &i);
    }

    QUEUE_DEBUG_i32(q, w, "\n\tQueues after enqueue:")

    result = (queue__size(q) == N && queue__size(w) == N) ? TEST_SUCCESS : TEST_FAILURE;

    QUEUE_FREE(q, w, NULL, NULL)
    return result;
}

static bool test_queue__dequeue_on_empty_queue(char debug)
{
    printf("%s... ", __func__);

    bool result;
    Queue q = queue__empty_copy_enabled((copy_operator_t)operator_copy, (delete_operator_t)operator_delete);
    Queue w = queue__empty_copy_disabled();

    result = (queue__dequeue(q)
           && queue__dequeue(w)
           && queue__is_empty(q)
           && queue__is_empty(w)) ? TEST_SUCCESS : TEST_FAILURE;

    QUEUE_DEBUG_i32(q, w, "\n\tQueues after dequeue:")

    QUEUE_FREE(q, w, NULL, NULL)
    return result;
}

static bool test_queue__dequeue_on_non_empty_queue(char debug)
{
    printf("%s... ", __func__);

    bool result;
    u32 N = 5;
    Queue q = queue__empty_copy_enabled((copy_operator_t)operator_copy, (delete_operator_t)operator_delete);
    Queue w = queue__empty_copy_disabled();

    for (u32 i = 0; i < N; i++) {
        queue__enqueue(q, &i);
        queue__enqueue(w, &i);
    }

    QUEUE_DEBUG_i32(q, w, "\n\tQueues before dequeue:")

    result = (!queue__dequeue(q)
           && !queue__dequeue(w)
           && queue__size(q) == N-1
           && queue__size(w) == N-1) ? TEST_SUCCESS : TEST_FAILURE;

    QUEUE_DEBUG_i32(q, w, "\n\tQueues after dequeue:")

    QUEUE_FREE(q, w, NULL, NULL)
    return result;
}

static bool test_queue__enqueue_and_dequeue_on_multiple_elements(char debug)
{
    printf("%s... ", __func__);

    bool result;
    Queue q = queue__empty_copy_enabled((copy_operator_t)operator_copy, (delete_operator_t)operator_delete);
    Queue w = queue__empty_copy_disabled();

    for (u32 i = 0; i < 60; i++) {
        queue__enqueue(q, &i);
        queue__enqueue(w, &i);
    }

    result = (queue__size(q) == 60 && queue__size(w) == 60) ? TEST_SUCCESS : TEST_FAILURE;

    QUEUE_DEBUG_i32(q, w, "\nAdded 60 elements, expected capacity of 64 and size of 60\n")

    for (u32 i = 0; i < 40; i++) {
        queue__dequeue(q);
        queue__dequeue(w);
    }

    result |= queue__size(q) != 20 || queue__size(w) != 20;

    QUEUE_DEBUG_i32(q, w, "\nRemoved 40 elements, expected capacity of 32 and size of 20\n")

    for (u32 i = 0; i < 20; i++) {
        queue__enqueue(q, &i);
        queue__enqueue(w, &i);
    }

    result |= queue__size(q) != 40 || queue__size(w) != 40;

    QUEUE_DEBUG_i32(q, w, "\nAdded 20 elements, expected capacity of 64 and size of 40\n")

    QUEUE_FREE(q, w, NULL, NULL)
    return result;
}

static bool test_queue__pop_on_empty_queue(char debug)
{
    printf("%s... ", __func__);

    bool result;
    Queue q = queue__empty_copy_enabled((copy_operator_t)operator_copy, (delete_operator_t)operator_delete);
    Queue w = queue__empty_copy_disabled();

    result = (queue__pop(q, NULL)
           && queue__pop(w, NULL)
           && queue__is_empty(q)
           && queue__is_empty(w)) ? TEST_SUCCESS : TEST_FAILURE;

    QUEUE_DEBUG_i32(q, w, "\n\tQueues after pop:")

    QUEUE_FREE(q, w, NULL, NULL)
    return result;
}

static bool test_queue__pop_on_non_empty_queue(char debug)
{
    printf("%s... ", __func__);

    bool result;
    u32 N = 5;
    elem_t front_q = NULL;
    elem_t front_w = NULL;
    Queue q = queue__empty_copy_enabled((copy_operator_t)operator_copy, (delete_operator_t)operator_delete);
    Queue w = queue__empty_copy_disabled();

    for (u32 i = 0; i < N; i++) {
        queue__enqueue(q, &i);
        queue__enqueue(w, &i);
    }

    QUEUE_DEBUG_i32(q, w, "\n\tQueues before pop:")

    result = TEST_SUCCESS;
    for (u32 i = 0; i < N; i++) {
        result |= queue__pop(q, &front_q)
               || queue__pop(w, &front_w)
               || queue__size(q) != N-i-1
               || queue__size(w) != N-i-1
               || *(u32 *)front_q != i
               || *(u32 *)front_w != N;
        free(front_q);
    }

    QUEUE_DEBUG_i32(q, w, "\n\tQueues after pop:")

    QUEUE_FREE(q, w, NULL, NULL)
    return result;
}

static bool test_queue__clean_NULL_on_empty_queue(char debug)
{
    printf("%s... ", __func__);

    bool result;
    Queue q = queue__empty_copy_enabled((copy_operator_t)operator_copy, (delete_operator_t)operator_delete);
    Queue w = queue__empty_copy_disabled();

    queue__clean_NULL(q);
    queue__clean_NULL(w);

    result = (queue__is_empty(q) && queue__is_empty(w)) ? TEST_SUCCESS : TEST_FAILURE;

    QUEUE_DEBUG_i32(q, w, "\n\tQueues after clean_NULL:")

    QUEUE_FREE(q, w, NULL, NULL)
    return result;
}

static bool test_queue__clean_NULL_on_non_empty_queue(char debug)
{
    printf("%s... ", __func__);

    bool result;
    u32 N = 8;
    Queue q = queue__empty_copy_enabled((copy_operator_t)operator_copy, (delete_operator_t)operator_delete);
    Queue w = queue__empty_copy_disabled();

    for (u32 i = 0; i < N; i++) {
        queue__enqueue(q, !(i%2) ? &i : NULL);
        queue__enqueue(w, !(i%2) ? &i : NULL);
    }

    QUEUE_DEBUG_i32(q, w, "\n\tQueues before clean_NULL:")

    queue__clean_NULL(q);
    queue__clean_NULL(w);

    result = (queue__size(q) == N>>1 && queue__size(w) == N>>1) ? TEST_SUCCESS : TEST_FAILURE;

    QUEUE_DEBUG_i32(q, w, "\n\tQueues after clean_NULL:")

    QUEUE_FREE(q, w, NULL, NULL)
    return result;
}

static bool test_queue__clear_on_empty_queue(char debug)
{
    printf("%s... ", __func__);

    bool result;
    Queue q = queue__empty_copy_enabled((copy_operator_t)operator_copy, (delete_operator_t)operator_delete);
    Queue w = queue__empty_copy_disabled();

    queue__clear(q);
    queue__clear(w);

    result = (queue__is_empty(q) && queue__is_empty(w)) ? TEST_SUCCESS : TEST_FAILURE;

    QUEUE_DEBUG_i32(q, w, "\n\tQueues after clear:")

    QUEUE_FREE(q, w, NULL, NULL)
    return result;
}

static bool test_queue__clear_on_non_empty_queue(char debug)
{
    printf("%s... ", __func__);

    bool result;
    u32 N = 5;
    Queue q = queue__empty_copy_enabled((copy_operator_t)operator_copy, (delete_operator_t)operator_delete);
    Queue w = queue__empty_copy_disabled();

    for (u32 i = 0; i < N; i++) {
        queue__enqueue(q, &i);
        queue__enqueue(w, &i);
    }

    QUEUE_DEBUG_i32(q, w, "\n\tQueues before clear:")

    queue__clear(q);
    queue__clear(w);

    result = (queue__is_empty(q) && queue__is_empty(w)) ? TEST_SUCCESS : TEST_FAILURE;

    QUEUE_DEBUG_i32(q, w, "\n\tQueues after clear:")

    QUEUE_FREE(q, w, NULL, NULL)
    return result;
}

static bool test_queue__front_on_empty_queue(char debug)
{
    printf("%s... ", __func__);

    bool result;
    Queue q = queue__empty_copy_enabled((copy_operator_t)operator_copy, (delete_operator_t)operator_delete);
    Queue w = queue__empty_copy_disabled();

    result = (queue__front(q, NULL)
           && queue__front(w, NULL)
           && queue__is_empty(q)
           && queue__is_empty(w)) ? TEST_SUCCESS : TEST_FAILURE;

    QUEUE_DEBUG_i32(q, w, "\n\tQueues after front:")

    QUEUE_FREE(q, w, NULL, NULL)
    return result;
}

static bool test_queue__front_on_non_empty_queue(char debug)
{
    printf("%s... ", __func__);

    bool result;
    int value = 5;
    elem_t front_q = NULL;
    elem_t front_w = NULL;
    Queue q = queue__empty_copy_enabled((copy_operator_t)operator_copy, (delete_operator_t)operator_delete);
    Queue w = queue__empty_copy_disabled();

    queue__enqueue(q, &value);
    queue__enqueue(w, &value);

    QUEUE_DEBUG_i32(q, w, "\n\tQueues before front:")

    result = (!queue__front(q, &front_q)
           && !queue__front(w, &front_w)
           && !queue__is_empty(q)
           && !queue__is_empty(w)
           && *(int *)front_q == 5
           && *(int *)front_w == 5) ? TEST_SUCCESS : TEST_FAILURE;

    QUEUE_DEBUG_i32(q, w, "\n\tQueues after front:")

    free(front_q);
    QUEUE_FREE(q, w, NULL, NULL)
    return result;
}

static bool test_queue__back_on_empty_queue(char debug)
{
    printf("%s... ", __func__);

    bool result;
    Queue q = queue__empty_copy_enabled((copy_operator_t)operator_copy, (delete_operator_t)operator_delete);
    Queue w = queue__empty_copy_disabled();

    result = (queue__back(q, NULL)
           && queue__back(w, NULL)
           && queue__is_empty(q)
           && queue__is_empty(w)) ? TEST_SUCCESS : TEST_FAILURE;

    QUEUE_DEBUG_i32(q, w, "\n\tQueues after back:")

    QUEUE_FREE(q, w, NULL, NULL)
    return result;
}

static bool test_queue__back_on_non_empty_queue(char debug)
{
    printf("%s... ", __func__);

    bool result;
    int value1 = 5;
    int value2 = 7;
    elem_t back_q = NULL;
    elem_t back_w = NULL;
    Queue q = queue__empty_copy_enabled((copy_operator_t)operator_copy, (delete_operator_t)operator_delete);
    Queue w = queue__empty_copy_disabled();

    queue__enqueue(q, &value1);
    queue__enqueue(w, &value1);
    queue__enqueue(q, &value2);
    queue__enqueue(w, &value2);

    QUEUE_DEBUG_i32(q, w, "\n\tQueues before back:")

    result = (!queue__back(q, &back_q)
           && !queue__back(w, &back_w)
           && !queue__is_empty(q)
           && !queue__is_empty(w)
           && *(int *)back_q == 7
           && *(int *)back_w == 7) ? TEST_SUCCESS : TEST_FAILURE;

    QUEUE_DEBUG_i32(q, w, "\n\tQueues after back:")

    free(back_q);
    QUEUE_FREE(q, w, NULL, NULL)
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
    Queue q_char = queue__empty_copy_enabled((copy_operator_t)operator_copy, (delete_operator_t)operator_delete);
    Queue w_char = NULL;
    Queue q_int = queue__empty_copy_enabled((copy_operator_t)operator_copy, (delete_operator_t)operator_delete);
    Queue w_int = NULL;

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

    QUEUE_DEBUG_char(q_char, w_char, "\n\tQueues after from_array:")
    QUEUE_DEBUG_i32(q_int, w_int, " ")

    free(D);
    QUEUE_FREE(q_char, w_char, q_int, w_int)
    return result;
}

static bool test_queue__to_array_on_empty_queue(char debug)
{
    printf("%s... ", __func__);

    bool result;
    Queue q = queue__empty_copy_enabled((copy_operator_t)operator_copy, (delete_operator_t)operator_delete);
    Queue w = queue__empty_copy_disabled();

    result = (!queue__to_array(q)
           && !queue__to_array(w)
           && queue__is_empty(q)
           && queue__is_empty(w)) ? TEST_SUCCESS : TEST_FAILURE;

    QUEUE_DEBUG_i32(q, w, "\n\tQueues after to_array:")

    QUEUE_FREE(q, w, NULL, NULL)
    return result;
}

static bool test_queue__to_array_on_non_empty_queue(char debug)
{
    printf("%s... ", __func__);

    bool result;
    u32 N = 5;
    int **A = NULL;
    int **B = NULL;
    Queue q = queue__empty_copy_enabled((copy_operator_t)operator_copy, (delete_operator_t)operator_delete);
    Queue w = queue__empty_copy_disabled();

    for (u32 i = 0; i < N; i++) {
        queue__enqueue(q, &i);
        queue__enqueue(w, &i);
    }

    QUEUE_DEBUG_i32(q, w, "\n\tQueues before to_array:")

    A = (int **)queue__to_array(q);
    B = (int **)queue__to_array(w);

    result = (queue__size(q) == N && queue__size(w) == N) ? TEST_SUCCESS : TEST_FAILURE;

    for (u32 i = 0; i < N; i++) {
        result |= *A[i] != (int)i || *B[i] != (int)N;
    }

    QUEUE_DEBUG_i32(q, w, "\n\tQueues after to_array:")

    for (u32 i = 0; i < N; i++) {
        free(A[i]);
    }
    free(A);
    free(B);
    QUEUE_FREE(q, w, NULL, NULL)
    return result;
}

static bool test_queue__foreach_on_empty_queue(char debug)
{
    printf("%s... ", __func__);

    bool result;
    int value = 1;
    Queue q = queue__empty_copy_enabled((copy_operator_t)operator_copy, (delete_operator_t)operator_delete);
    Queue w = queue__empty_copy_disabled();

    queue__foreach(q, (applying_func_t) plus_op, &value);
    queue__foreach(w, (applying_func_t) plus_op, &value);

    result = (queue__is_empty(q) && queue__is_empty(w)) ? TEST_SUCCESS : TEST_FAILURE;

    QUEUE_DEBUG_i32(q, w, "\n\tQueues after foreach:")

    QUEUE_FREE(q, w, NULL, NULL)
    return result;
}

static bool test_queue__foreach_on_non_empty_queue(char debug)
{
    printf("%s... ", __func__);

    bool result;
    u32 N = 8;
    int value = 1;
    int array[8] = {-5, 0, 2, 7, 11, 1, 3, 10};
    elem_t *A = NULL, *B = NULL, *C = NULL, *D = NULL;
    Queue q = queue__empty_copy_enabled((copy_operator_t)operator_copy, (delete_operator_t)operator_delete);
    Queue w = queue__empty_copy_disabled();

    queue__from_array(q, array, N, sizeof(int));
    queue__from_array(w, array, N, sizeof(int));

    A = queue__to_array(q);
    B = queue__to_array(w);

    QUEUE_DEBUG_i32(q, w, "\n\tQueues before foreach:")

    queue__foreach(q, (applying_func_t) plus_op, &value);
    queue__foreach(w, (applying_func_t) plus_op, &value);

    C = queue__to_array(q);
    D = queue__to_array(w);

    result = TEST_SUCCESS;
    for (u32 i = 0; i < N; i++) {
        result |= *(int *)A[i] + value != *(int *)C[i] || *(int *)B[i] != *(int *)D[i];
    }

    QUEUE_DEBUG_i32(q, w, "\n\tQueues after foreach:")

    for (u32 i = 0; i < N; i++) {
        free(A[i]);
        free(C[i]);
    }
    free(A);
    free(B);
    free(C);
    free(D);
    QUEUE_FREE(q, w, NULL, NULL)
    return result;
}

static bool test_queue__sort_on_empty_queue(char debug)
{
    printf("%s... ", __func__);

    bool result;
    Queue q = queue__empty_copy_enabled((copy_operator_t)operator_copy, (delete_operator_t)operator_delete);
    Queue w = queue__empty_copy_disabled();

    queue__sort(q, operator_compare);
    queue__sort(w, operator_compare);

    result = (queue__is_empty(q) && queue__is_empty(w)) ? TEST_SUCCESS : TEST_FAILURE;

    QUEUE_DEBUG_i32(q, w, "\n\tQueues after sort:")

    QUEUE_FREE(q, w, NULL, NULL)
    return result;
}

static bool test_queue__sort_on_non_empty_queue(char debug)
{
    printf("%s... ", __func__);

    bool result;
    u32 N = 8;
    int unordered[8] = {5, -3, 2, 0, 1, 0, 7, 4};
    int ordered[8] = {-3, 0, 0, 1, 2, 4, 5, 7};
    elem_t *A = NULL;
    elem_t *B = NULL;
    Queue q = queue__empty_copy_enabled((copy_operator_t)operator_copy, (delete_operator_t)operator_delete);
    Queue w = queue__empty_copy_disabled();

    queue__from_array(q, unordered, N, sizeof(int));
    queue__from_array(w, unordered, N, sizeof(int));

    QUEUE_DEBUG_i32(q, w, "\n\tQueues before sort:")

    queue__sort(q, operator_compare);
    queue__sort(w, operator_compare);

    A = queue__to_array(q);
    B = queue__to_array(w);

    result = TEST_SUCCESS;
    for (u32 i = 0; i < 8; i++) {
        result |= ordered[i] != *(int *)A[i] || ordered[i] != *(int *)B[i];
    }

    QUEUE_DEBUG_i32(q, w, "\n\tQueues after sort:")

    for (u32 i = 0; i < 8; i++) {
        free(A[i]);
    }
    free(A);
    free(B);
    QUEUE_FREE(q, w, NULL, NULL)
    return result;
}

static bool test_queue__shuffle_on_empty_queue(char debug)
{
    printf("%s... ", __func__);

    bool result;
    Queue q = queue__empty_copy_enabled((copy_operator_t)operator_copy, (delete_operator_t)operator_delete);
    Queue w = queue__empty_copy_disabled();

    queue__shuffle(q);
    queue__shuffle(w);

    result = (queue__is_empty(q) && queue__is_empty(w)) ? TEST_SUCCESS : TEST_FAILURE;

    QUEUE_DEBUG_i32(q, w, "\n\tQueues after shuffle:")

    QUEUE_FREE(q, w, NULL, NULL)
    return result;
}

static bool test_queue__shuffle_on_non_empty_queue(char debug)
{
    printf("%s... ", __func__);

    bool result;
    u32 N = 8;
    char src_char[8] = {'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h'};
    int src_int[8] = {0, 1, 2, 3, 4, 5, 6, 7};
    elem_t *A = NULL, *B = NULL, *C = NULL, *D = NULL;
    Queue q_char = queue__empty_copy_enabled((copy_operator_t)operator_copy, (delete_operator_t)operator_delete);
    Queue w_char = queue__empty_copy_disabled();
    Queue q_int = queue__empty_copy_enabled((copy_operator_t)operator_copy, (delete_operator_t)operator_delete);
    Queue w_int = queue__empty_copy_disabled();

    queue__from_array(q_char, src_char, N, sizeof(char));
    queue__from_array(w_char, src_char, N, sizeof(char));
    queue__from_array(q_int, src_int, N, sizeof(int));
    queue__from_array(w_int, src_int, N, sizeof(int));

    QUEUE_DEBUG_char(q_char, w_char, "\n\tQueues before shuffle:")
    QUEUE_DEBUG_i32(q_int, w_int, " ")

    queue__shuffle(q_char);
    queue__shuffle(w_char);
    queue__shuffle(q_int);
    queue__shuffle(w_int);

    A = queue__to_array(q_char);
    B = queue__to_array(w_char);
    C = queue__to_array(q_int);
    D = queue__to_array(w_int);

    result = TEST_SUCCESS;

    QUEUE_DEBUG_char(q_char, w_char, "\n\tQueues after shuffle:")
    QUEUE_DEBUG_i32(q_int, w_int, " ")

    for (u32 i = 0; i < N; i++) {
        free(A[i]);
        free(C[i]);
    }
    free(A);
    free(B);
    free(C);
    free(D);
    QUEUE_FREE(q_char, w_char, q_int, w_int)
    return result;
}


int main(void)
{
    int nb_success = 0;
    int nb_tests = 0;
    printf("----------- TEST QUEUE -----------\n");

    print_test_result(test_queue__empty_copy_enabled(false), &nb_success, &nb_tests);
    print_test_result(test_queue__empty_copy_disabled(false), &nb_success, &nb_tests);
    print_test_result(test_queue__is_copy_enabled(), &nb_success, &nb_tests);
    print_test_result(test_queue__size(), &nb_success, &nb_tests);
    print_test_result(test_queue__is_empty_on_empty_queue(), &nb_success, &nb_tests);
    print_test_result(test_queue__is_empty_on_non_empty_queue(), &nb_success, &nb_tests);

    print_test_result(test_queue__enqueue_on_empty_queue(false), &nb_success, &nb_tests);
    print_test_result(test_queue__enqueue_on_non_empty_queue(false), &nb_success, &nb_tests);
    print_test_result(test_queue__dequeue_on_empty_queue(false), &nb_success, &nb_tests);
    print_test_result(test_queue__dequeue_on_non_empty_queue(false), &nb_success, &nb_tests);
    print_test_result(test_queue__enqueue_and_dequeue_on_multiple_elements(false), &nb_success, &nb_tests);
    print_test_result(test_queue__pop_on_empty_queue(false), &nb_success, &nb_tests);
    print_test_result(test_queue__pop_on_non_empty_queue(false), &nb_success, &nb_tests);

    print_test_result(test_queue__clean_NULL_on_empty_queue(false), &nb_success, &nb_tests);
    print_test_result(test_queue__clean_NULL_on_non_empty_queue(false), &nb_success, &nb_tests);
    print_test_result(test_queue__clear_on_empty_queue(false), &nb_success, &nb_tests);
    print_test_result(test_queue__clear_on_non_empty_queue(false), &nb_success, &nb_tests);
    print_test_result(test_queue__front_on_empty_queue(false), &nb_success, &nb_tests);
    print_test_result(test_queue__front_on_non_empty_queue(false), &nb_success, &nb_tests);
    print_test_result(test_queue__back_on_empty_queue(false), &nb_success, &nb_tests);
    print_test_result(test_queue__back_on_non_empty_queue(false), &nb_success, &nb_tests);

    print_test_result(test_queue__from_array(false), &nb_success, &nb_tests);
    print_test_result(test_queue__to_array_on_empty_queue(false), &nb_success, &nb_tests);
    print_test_result(test_queue__to_array_on_non_empty_queue(false), &nb_success, &nb_tests);
    print_test_result(test_queue__foreach_on_empty_queue(false), &nb_success, &nb_tests);
    print_test_result(test_queue__foreach_on_non_empty_queue(false), &nb_success, &nb_tests);
    print_test_result(test_queue__sort_on_empty_queue(false), &nb_success, &nb_tests);
    print_test_result(test_queue__sort_on_non_empty_queue(false), &nb_success, &nb_tests);
    print_test_result(test_queue__shuffle_on_empty_queue(false), &nb_success, &nb_tests);
    print_test_result(test_queue__shuffle_on_non_empty_queue(false), &nb_success, &nb_tests);

    print_test_summary(nb_success, nb_tests);

    return TEST_SUCCESS;
}
