#include "common_tests_utils.h"
#include "../queue/queue.h"
#include "../common/defs.h"

////////////////////////////////////////////////////////////////////
///     TEST SUITE
////////////////////////////////////////////////////////////////////

static char test_queue__empty_copy_enabled(char debug)
{
    printf("%s... ", __func__);

    char result;
    Queue q = queue__empty_copy_enabled((copy_operator_t)operator_copy, (delete_operator_t)operator_delete);

    if (debug) queue__debug(q, (void (*)(elem_t))operator_debug_i32);

    result = q ? TEST_SUCCESS : TEST_FAILURE;

    queue__free(q);
    return result;
}

static char test_queue__empty_copy_disabled(char debug)
{
    printf("%s... ", __func__);

    char result;
    Queue q = queue__empty_copy_disabled();

    if (debug) queue__debug(q, (void (*)(elem_t))operator_debug_i32);

    result = q ? TEST_SUCCESS : TEST_FAILURE;

    queue__free(q);
    return result;
}

static char test_queue__is_copy_enabled(void)
{
    printf("%s... ", __func__);

    char result;
    Queue q = queue__empty_copy_enabled((copy_operator_t)operator_copy, (delete_operator_t)operator_delete);
    Queue w = queue__empty_copy_disabled();

    result = (queue__is_copy_enabled(q) && !queue__is_copy_enabled(w)) ? TEST_SUCCESS : TEST_FAILURE;

    queue__free(q);
    queue__free(w);
    return result;
}

static char test_queue__size(void)
{
    printf("%s... ", __func__);

    char result;
    Queue q = queue__empty_copy_enabled((copy_operator_t)operator_copy, (delete_operator_t)operator_delete);
    Queue w = queue__empty_copy_disabled();

    queue__enqueue(q, NULL);
    queue__enqueue(w, NULL);
    queue__enqueue(q, NULL);
    queue__enqueue(w, NULL);

    result = (queue__size(q) == 2 && queue__size(w) == 2) ? TEST_SUCCESS : TEST_FAILURE;

    queue__free(q);
    queue__free(w);
    return result;
}

static char test_queue__is_empty_on_empty_queue(void)
{
    printf("%s... ", __func__);

    char result;
    Queue q = queue__empty_copy_enabled((copy_operator_t)operator_copy, (delete_operator_t)operator_delete);
    Queue w = queue__empty_copy_disabled();

    result = (queue__is_empty(q) && queue__is_empty(w)) ? TEST_SUCCESS : TEST_FAILURE;

    queue__free(q);
    queue__free(w);
    return result;
}

static char test_queue__is_empty_on_non_empty_queue(void)
{
    printf("%s... ", __func__);

    char result;
    Queue q = queue__empty_copy_enabled((copy_operator_t)operator_copy, (delete_operator_t)operator_delete);
    Queue w = queue__empty_copy_disabled();

    queue__enqueue(q, NULL);
    queue__enqueue(w, NULL);

    result = (!queue__is_empty(q) && !queue__is_empty(w)) ? TEST_SUCCESS : TEST_FAILURE;

    queue__free(q);
    queue__free(w);
    return result;
}

static char test_queue__enqueue_on_empty_queue(char debug)
{
    printf("%s... ", __func__);

    char result;
    Queue q = queue__empty_copy_enabled((copy_operator_t)operator_copy, (delete_operator_t)operator_delete);
    Queue w = queue__empty_copy_disabled();

    result = (!queue__enqueue(q, NULL)
           && !queue__enqueue(w, NULL)
           && queue__size(q) == 1
           && queue__size(w) == 1) ? TEST_SUCCESS : TEST_FAILURE;

    if (debug) {
        printf("\n\tQueues after enqueue:");
        queue__debug(q, (void (*)(elem_t))operator_debug_i32);
        queue__debug(w, (void (*)(elem_t))operator_debug_i32);
    }

    queue__free(q);
    queue__free(w);
    return result;
}

static char test_queue__enqueue_on_non_empty_queue(char debug)
{
    printf("%s... ", __func__);

    char result;
    u32 N = 5;
    Queue q = queue__empty_copy_enabled((copy_operator_t)operator_copy, (delete_operator_t)operator_delete);
    Queue w = queue__empty_copy_disabled();

    for (u32 i = 0; i < N; i++) {
        queue__enqueue(q, &i);
        queue__enqueue(w, &i);
    }

    if (debug) {
        printf("\n\tQueues after enqueue:");
        queue__debug(q, (void (*)(elem_t))operator_debug_i32);
        queue__debug(w, (void (*)(elem_t))operator_debug_i32);
    }

    result = (queue__size(q) == N && queue__size(w) == N) ? TEST_SUCCESS : TEST_FAILURE;

    queue__free(q);
    queue__free(w);
    return result;
}

static char test_queue__dequeue_on_empty_queue(char debug)
{
    printf("%s... ", __func__);

    char result;
    Queue q = queue__empty_copy_enabled((copy_operator_t)operator_copy, (delete_operator_t)operator_delete);
    Queue w = queue__empty_copy_disabled();

    result = (queue__dequeue(q)
           && queue__dequeue(w)
           && queue__is_empty(q)
           && queue__is_empty(w)) ? TEST_SUCCESS : TEST_FAILURE;

    if (debug) {
        printf("\n\tQueues after dequeue:");
        queue__debug(q, (void (*)(elem_t))operator_debug_i32);
        queue__debug(w, (void (*)(elem_t))operator_debug_i32);
    }

    queue__free(q);
    queue__free(w);
    return result;
}

static char test_queue__dequeue_on_non_empty_queue(char debug)
{
    printf("%s... ", __func__);

    char result;
    u32 N = 5;
    Queue q = queue__empty_copy_enabled((copy_operator_t)operator_copy, (delete_operator_t)operator_delete);
    Queue w = queue__empty_copy_disabled();

    for (u32 i = 0; i < N; i++) {
        queue__enqueue(q, &i);
        queue__enqueue(w, &i);
    }

    if (debug) {
        printf("\n\tQueues before dequeue:");
        queue__debug(q, (void (*)(elem_t))operator_debug_i32);
        queue__debug(w, (void (*)(elem_t))operator_debug_i32);
    }

    result = (!queue__dequeue(q)
           && !queue__dequeue(w)
           && queue__size(q) == N-1
           && queue__size(w) == N-1) ? TEST_SUCCESS : TEST_FAILURE;

    if (debug) {
        printf("\n\tQueues after dequeue:");
        queue__debug(q, (void (*)(elem_t))operator_debug_i32);
        queue__debug(w, (void (*)(elem_t))operator_debug_i32);
    }

    queue__free(q);
    queue__free(w);
    return result;
}

static char test_queue__enqueue_and_dequeue_on_multiple_elements(char debug)
{
    printf("%s... ", __func__);

    char result;
    Queue q = queue__empty_copy_enabled((copy_operator_t)operator_copy, (delete_operator_t)operator_delete);
    Queue w = queue__empty_copy_disabled();

    for (u32 i = 0; i < 60; i++) {
        queue__enqueue(q, &i);
        queue__enqueue(w, &i);
    }

    result = (queue__size(q) == 60 && queue__size(w) == 60) ? TEST_SUCCESS : TEST_FAILURE;

    if (debug) {
        printf("\nAdded 60 elements, expected capacity of 64 and size of 60\n");
        queue__debug(q, (void (*)(elem_t))operator_debug_i32);
        queue__debug(w, (void (*)(elem_t))operator_debug_i32);
    }

    for (u32 i = 0; i < 40; i++) {
        queue__dequeue(q);
        queue__dequeue(w);
    }

    result |= (char)(queue__size(q) != 20 || queue__size(w) != 20);

    if (debug) {
        printf("\nRemoved 40 elements, expected capacity of 32 and size of 20\n");
        queue__debug(q, (void (*)(elem_t))operator_debug_i32);
        queue__debug(w, (void (*)(elem_t))operator_debug_i32);
    }

    for (u32 i = 0; i < 20; i++) {
        queue__enqueue(q, &i);
        queue__enqueue(w, &i);
    }

    result |= (char)(queue__size(q) != 40 || queue__size(w) != 40);

    if (debug) {
        printf("\nAdded 20 elements, expected capacity of 64 and size of 40\n");
        queue__debug(q, (void (*)(elem_t))operator_debug_i32);
        queue__debug(w, (void (*)(elem_t))operator_debug_i32);
    }

    queue__free(q);
    queue__free(w);
    return result;
}

static char test_queue__pop_on_empty_queue(char debug)
{
    printf("%s... ", __func__);

    char result;
    Queue q = queue__empty_copy_enabled((copy_operator_t)operator_copy, (delete_operator_t)operator_delete);
    Queue w = queue__empty_copy_disabled();

    result = (queue__pop(q, NULL)
           && queue__pop(w, NULL)
           && queue__is_empty(q)
           && queue__is_empty(w)) ? TEST_SUCCESS : TEST_FAILURE;

    if (debug) {
        printf("\n\tQueues after pop:");
        queue__debug(q, (void (*)(elem_t))operator_debug_i32);
        queue__debug(w, (void (*)(elem_t))operator_debug_i32);
    }

    queue__free(q);
    queue__free(w);
    return result;
}

static char test_queue__pop_on_non_empty_queue(char debug)
{
    printf("%s... ", __func__);

    char result;
    u32 N = 5;
    elem_t front_q = NULL;
    elem_t front_w = NULL;
    Queue q = queue__empty_copy_enabled((copy_operator_t)operator_copy, (delete_operator_t)operator_delete);
    Queue w = queue__empty_copy_disabled();

    for (u32 i = 0; i < N; i++) {
        queue__enqueue(q, &i);
        queue__enqueue(w, &i);
    }

    if (debug) {
        printf("\n\tQueues before pop:");
        queue__debug(q, (void (*)(elem_t))operator_debug_i32);
        queue__debug(w, (void (*)(elem_t))operator_debug_i32);
    }

    result = TEST_SUCCESS;
    for (u32 i = 0; i < N; i++) {
        result |= (char)(queue__pop(q, &front_q)
                      || queue__pop(w, &front_w)
                      || queue__size(q) != N-i-1
                      || queue__size(w) != N-i-1
                      || *(u32 *)front_q != i
                      || *(u32 *)front_w != N);
        free(front_q);
    }

    if (debug) {
        printf("\n\tQueues after pop:");
        queue__debug(q, (void (*)(elem_t))operator_debug_i32);
        queue__debug(w, (void (*)(elem_t))operator_debug_i32);
    }

    queue__free(q);
    queue__free(w);
    return result;
}

static char test_queue__clear_on_empty_queue(char debug)
{
    printf("%s... ", __func__);

    char result;
    Queue q = queue__empty_copy_enabled((copy_operator_t)operator_copy, (delete_operator_t)operator_delete);
    Queue w = queue__empty_copy_disabled();

    queue__clear(q);
    queue__clear(w);

    if (debug) {
        printf("\n\tQueues after clear:");
        queue__debug(q, (void (*)(elem_t))operator_debug_i32);
        queue__debug(w, (void (*)(elem_t))operator_debug_i32);
    }

    result = (queue__is_empty(q) && queue__is_empty(w)) ? TEST_SUCCESS : TEST_FAILURE;

    queue__free(q);
    queue__free(w);
    return result;
}

static char test_queue__clear_on_non_empty_queue(char debug)
{
    printf("%s... ", __func__);

    char result;
    u32 N = 5;
    Queue q = queue__empty_copy_enabled((copy_operator_t)operator_copy, (delete_operator_t)operator_delete);
    Queue w = queue__empty_copy_disabled();

    for (u32 i = 0; i < N; i++) {
        queue__enqueue(q, &i);
        queue__enqueue(w, &i);
    }

    if (debug) {
        printf("\n\tQueues before clear:");
        queue__debug(q, (void (*)(elem_t))operator_debug_i32);
        queue__debug(w, (void (*)(elem_t))operator_debug_i32);
    }

    queue__clear(q);
    queue__clear(w);

    if (debug) {
        printf("\n\tQueues after clear:");
        queue__debug(q, (void (*)(elem_t))operator_debug_i32);
        queue__debug(w, (void (*)(elem_t))operator_debug_i32);
    }

    result = (queue__is_empty(q) && queue__is_empty(w)) ? TEST_SUCCESS : TEST_FAILURE;

    queue__free(q);
    queue__free(w);
    return result;
}

static char test_queue__front_on_empty_queue(char debug)
{
    printf("%s... ", __func__);

    char result;
    Queue q = queue__empty_copy_enabled((copy_operator_t)operator_copy, (delete_operator_t)operator_delete);
    Queue w = queue__empty_copy_disabled();

    result = (queue__front(q, NULL)
           && queue__front(w, NULL)
           && queue__is_empty(q)
           && queue__is_empty(w)) ? TEST_SUCCESS : TEST_FAILURE;

    if (debug) {
        printf("\n\tQueues after front:");
        queue__debug(q, (void (*)(elem_t))operator_debug_i32);
        queue__debug(w, (void (*)(elem_t))operator_debug_i32);
    }

    queue__free(q);
    queue__free(w);
    return result;
}

static char test_queue__front_on_non_empty_queue(char debug)
{
    printf("%s... ", __func__);

    char result;
    int value = 5;
    elem_t front_q = NULL;
    elem_t front_w = NULL;
    Queue q = queue__empty_copy_enabled((copy_operator_t)operator_copy, (delete_operator_t)operator_delete);
    Queue w = queue__empty_copy_disabled();

    queue__enqueue(q, &value);
    queue__enqueue(w, &value);

    if (debug) {
        printf("\n\tQueues before front:");
        queue__debug(q, (void (*)(elem_t))operator_debug_i32);
        queue__debug(w, (void (*)(elem_t))operator_debug_i32);
    }

    result = (!queue__front(q, &front_q)
           && !queue__front(w, &front_w)
           && !queue__is_empty(q)
           && !queue__is_empty(w)
           && *(int *)front_q == 5
           && *(int *)front_w == 5) ? TEST_SUCCESS : TEST_FAILURE;

    if (debug) {
        printf("\n\tQueues after front:");
        queue__debug(q, (void (*)(elem_t))operator_debug_i32);
        queue__debug(w, (void (*)(elem_t))operator_debug_i32);
    }

    free(front_q);
    queue__free(q);
    queue__free(w);
    return result;
}

static char test_queue__back_on_empty_queue(char debug)
{
    printf("%s... ", __func__);

    char result;
    Queue q = queue__empty_copy_enabled((copy_operator_t)operator_copy, (delete_operator_t)operator_delete);
    Queue w = queue__empty_copy_disabled();

    result = (queue__back(q, NULL)
           && queue__back(w, NULL)
           && queue__is_empty(q)
           && queue__is_empty(w)) ? TEST_SUCCESS : TEST_FAILURE;

    if (debug) {
        printf("\n\tQueues after back:");
        queue__debug(q, (void (*)(elem_t))operator_debug_i32);
        queue__debug(w, (void (*)(elem_t))operator_debug_i32);
    }

    queue__free(q);
    queue__free(w);
    return result;
}

static char test_queue__back_on_non_empty_queue(char debug)
{
    printf("%s... ", __func__);

    char result;
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

    if (debug) {
        printf("\n\tQueues before back:");
        queue__debug(q, (void (*)(elem_t))operator_debug_i32);
        queue__debug(w, (void (*)(elem_t))operator_debug_i32);
    }

    result = (!queue__back(q, &back_q)
           && !queue__back(w, &back_w)
           && !queue__is_empty(q)
           && !queue__is_empty(w)
           && *(int *)back_q == 7
           && *(int *)back_w == 7) ? TEST_SUCCESS : TEST_FAILURE;

    if (debug) {
        printf("\n\tQueues after back:");
        queue__debug(q, (void (*)(elem_t))operator_debug_i32);
        queue__debug(w, (void (*)(elem_t))operator_debug_i32);
    }

    free(back_q);
    queue__free(q);
    queue__free(w);
    return result;
}

static char test_queue__from_array(char debug)
{
    printf("%s ", __func__);

    char result;
    int A[5] = {1, 2, 3, 4, 5};
    int B[5] = {6, 7, 8, 9, 10};
    char C[5] = {'a', 'b', 'c', 'd', 'e'};
    Queue q_int = queue__empty_copy_enabled((copy_operator_t)operator_copy, (delete_operator_t)operator_delete);
    Queue w_int = queue__empty_copy_disabled();
    Queue q_char = queue__empty_copy_enabled((copy_operator_t)operator_copy, (delete_operator_t)operator_delete);
    Queue w_char = queue__empty_copy_disabled();

    result = (queue__from_array(q_int, A, 5, INT)
           && queue__from_array(w_int, A, 5, INT)
           && queue__from_array(q_int, B, 5, INT)
           && queue__from_array(w_int, B, 5, INT)
           && queue__from_array(q_char, C, 5, CHAR)
           && queue__from_array(w_char, C, 5, CHAR)
           && queue__size(q_int) == 10
           && queue__size(w_int) == 10
           && queue__size(q_char) == 5
           && queue__size(w_char) == 5) ? TEST_SUCCESS : TEST_FAILURE;

    if (debug) {
        printf("\n\tQueues after to_array:");
        queue__debug(q_int, (void (*)(elem_t))operator_debug_i32);
        queue__debug(w_int, (void (*)(elem_t))operator_debug_i32);
        queue__debug(q_char, (void (*)(elem_t))operator_debug_char);
        queue__debug(w_char, (void (*)(elem_t))operator_debug_char);
    }

    queue__free(q_int);
    queue__free(w_int);
    queue__free(q_char);
    queue__free(w_char);
    return result;
}

static char test_queue__to_array_on_empty_queue(char debug)
{
    printf("%s ", __func__);

    char result;
    Queue q = queue__empty_copy_enabled((copy_operator_t)operator_copy, (delete_operator_t)operator_delete);
    Queue w = queue__empty_copy_disabled();

    result = (!queue__to_array(q)
           && !queue__to_array(w)
           && queue__is_empty(q)
           && queue__is_empty(w)) ? TEST_SUCCESS : TEST_FAILURE;

    if (debug) {
        printf("\n\tQueues after to_array:");
        queue__debug(q, (void (*)(elem_t))operator_debug_i32);
        queue__debug(w, (void (*)(elem_t))operator_debug_i32);
    }

    queue__free(q);
    queue__free(w);
    return result;
}

static char test_queue__to_array_on_non_empty_queue(char debug)
{
    printf("%s ", __func__);

    char result;
    u32 N = 5;
    int **A = NULL;
    int **B = NULL;
    Queue q = queue__empty_copy_enabled((copy_operator_t)operator_copy, (delete_operator_t)operator_delete);
    Queue w = queue__empty_copy_disabled();

    for (u32 i = 0; i < N; i++) {
        queue__enqueue(q, &i);
        queue__enqueue(w, &i);
    }

    if (debug) {
        printf("\n\tQueues before to_array:");
        queue__debug(q, (void (*)(elem_t))operator_debug_i32);
        queue__debug(w, (void (*)(elem_t))operator_debug_i32);
    }

    A = (int **)queue__to_array(q);
    B = (int **)queue__to_array(w);

    if (debug) {
        printf("\n\tQueues after to_array:");
        queue__debug(q, (void (*)(elem_t))operator_debug_i32);
        queue__debug(w, (void (*)(elem_t))operator_debug_i32);
    }

    result = (queue__size(q) == N && queue__size(w) == N) ? TEST_SUCCESS : TEST_FAILURE;

    for (u32 i = 0; i < N; i++) {
        result |= (char)(*A[i] != (int)i || *B[i] != (int)N);
    }

    for (u32 i = 0; i < N; i++) {
        free(A[i]);
    }
    free(A);
    free(B);
    queue__free(q);
    queue__free(w);
    return result;
}

static char test_queue__foreach_on_empty_queue(char debug)
{
    printf("%s ", __func__);

    char result;
    int value = 1;
    Queue q = queue__empty_copy_enabled((copy_operator_t)operator_copy, (delete_operator_t)operator_delete);
    Queue w = queue__empty_copy_disabled();

    queue__foreach(q, (applying_func_t) plus_op, &value);
    queue__foreach(w, (applying_func_t) plus_op, &value);

    if (debug) {
        printf("\n\tQueues after foreach:");
        queue__debug(q, (void (*)(elem_t))operator_debug_i32);
        queue__debug(w, (void (*)(elem_t))operator_debug_i32);
    }

    result = (queue__is_empty(q) && queue__is_empty(w)) ? TEST_SUCCESS : TEST_FAILURE;

    queue__free(q);
    queue__free(w);
    return result;
}

static char test_queue__foreach_on_non_empty_queue(char debug)
{
    printf("%s ", __func__);

    char result;
    u32 N = 8;
    int value = 1;
    int array[8] = {-5, 0, 2, 7, 11, 1, 3, 10};
    elem_t *A = NULL, *B = NULL, *C = NULL, *D = NULL;
    Queue q = queue__empty_copy_enabled((copy_operator_t)operator_copy, (delete_operator_t)operator_delete);
    Queue w = queue__empty_copy_disabled();

    queue__from_array(q, array, N, INT);
    queue__from_array(w, array, N, INT);

    A = queue__to_array(q);
    B = queue__to_array(w);

    if (debug) {
        printf("\n\tQueues before foreach:");
        queue__debug(q, (void (*)(elem_t))operator_debug_i32);
        queue__debug(w, (void (*)(elem_t))operator_debug_i32);
    }

    queue__foreach(q, (applying_func_t) plus_op, &value);
    queue__foreach(w, (applying_func_t) plus_op, &value);

    if (debug) {
        printf("\n\tQueues after foreach:");
        queue__debug(q, (void (*)(elem_t))operator_debug_i32);
        queue__debug(w, (void (*)(elem_t))operator_debug_i32);
    }

    C = queue__to_array(q);
    D = queue__to_array(w);

    result = TEST_SUCCESS;
    for (u32 i = 0; i < N; i++) {
        result |= (char)(*(int *)A[i] + value != *(int *)C[i] || *(int *)B[i] != *(int *)D[i]);
    }

    for (u32 i = 0; i < N; i++) {
        free(A[i]);
        free(C[i]);
    }
    free(A);
    free(B);
    free(C);
    free(D);
    queue__free(q);
    queue__free(w);
    return result;
}

static char test_queue__sort_on_empty_queue(char debug)
{
    printf("%s ", __func__);

    char result;
    Queue q = queue__empty_copy_enabled((copy_operator_t)operator_copy, (delete_operator_t)operator_delete);
    Queue w = queue__empty_copy_disabled();

    queue__sort(q, operator_compare);
    queue__sort(w, operator_compare);

    if (debug) {
        printf("\n\tQueues after sort:");
        queue__debug(q, (void (*)(elem_t))operator_debug_i32);
        queue__debug(w, (void (*)(elem_t))operator_debug_i32);
    }

    result = (queue__is_empty(q) && queue__is_empty(w)) ? TEST_SUCCESS : TEST_FAILURE;

    queue__free(q);
    queue__free(w);
    return result;
}

static char test_queue__sort_on_non_empty_queue(char debug)
{
    printf("%s ", __func__);

    char result;
    u32 N = 8;
    int unordered[8] = {5, -3, 2, 0, 1, 0, 7, 4};
    int ordered[8] = {-3, 0, 0, 1, 2, 4, 5, 7};
    elem_t *A = NULL;
    elem_t *B = NULL;
    Queue q = queue__empty_copy_enabled((copy_operator_t)operator_copy, (delete_operator_t)operator_delete);
    Queue w = queue__empty_copy_disabled();

    queue__from_array(q, unordered, N, INT);
    queue__from_array(w, unordered, N, INT);

    if (debug) {
        printf("\n\tQueues before sort:");
        queue__debug(q, (void (*)(elem_t))operator_debug_i32);
        queue__debug(w, (void (*)(elem_t))operator_debug_i32);
    }

    queue__sort(q, operator_compare);
    queue__sort(w, operator_compare);

    if (debug) {
        printf("\n\tQueues after sort:");
        queue__debug(q, (void (*)(elem_t))operator_debug_i32);
        queue__debug(w, (void (*)(elem_t))operator_debug_i32);
    }

    A = queue__to_array(q);
    B = queue__to_array(w);

    result = TEST_SUCCESS;
    for (u32 i = 0; i < 8; i++) {
        result |= (char)(ordered[i] != *(int *)A[i] || ordered[i] != *(int *)B[i]);
    }

    for (u32 i = 0; i < 8; i++) {
        free(A[i]);
    }
    free(A);
    free(B);
    queue__free(q);
    queue__free(w);
    return result;
}

static char test_queue__mix_on_empty_queue(char debug)
{
    printf("%s ", __func__);

    char result;
    Queue q = queue__empty_copy_enabled((copy_operator_t)operator_copy, (delete_operator_t)operator_delete);
    Queue w = queue__empty_copy_disabled();

    queue__mix(q);
    queue__mix(w);

    if (debug) {
        printf("\n\tQueues after mix:");
        queue__debug(q, (void (*)(elem_t))operator_debug_i32);
        queue__debug(w, (void (*)(elem_t))operator_debug_i32);
    }

    result = (queue__is_empty(q) && queue__is_empty(w)) ? TEST_SUCCESS : TEST_FAILURE;

    queue__free(q);
    queue__free(w);
    return result;
}

static char test_queue__mix_on_non_empty_queue(char debug)
{
    printf("%s ", __func__);

    char result;
    u32 N = 8;
    int src[8] = {5, -3, 2, 0, 1, 0, 7, 4};
    elem_t *A = NULL;
    elem_t *B = NULL;
    Queue q = queue__empty_copy_enabled((copy_operator_t)operator_copy, (delete_operator_t)operator_delete);
    Queue w = queue__empty_copy_disabled();

    queue__from_array(q, src, N, INT);
    queue__from_array(w, src, N, INT);

    if (debug) {
        printf("\n\tQueues before mix:");
        queue__debug(q, (void (*)(elem_t))operator_debug_i32);
        queue__debug(w, (void (*)(elem_t))operator_debug_i32);
    }

    queue__mix(q);
    queue__mix(w);

    if (debug) {
        printf("\n\tQueues after mix:");
        queue__debug(q, (void (*)(elem_t))operator_debug_i32);
        queue__debug(w, (void (*)(elem_t))operator_debug_i32);
    }

    A = queue__to_array(q);
    B = queue__to_array(w);

    result = TEST_SUCCESS;

    for (u32 i = 0; i < N; i++) {
        free(A[i]);
    }
    free(A);
    free(B);
    queue__free(q);
    queue__free(w);
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
    print_test_result(test_queue__mix_on_empty_queue(false), &nb_success, &nb_tests);
    print_test_result(test_queue__mix_on_non_empty_queue(false), &nb_success, &nb_tests);

    print_test_summary(nb_success, nb_tests);

    return TEST_SUCCESS;
}
