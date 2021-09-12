#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "queue.h"

#define DEFAULT_QUEUE_CAPACITY 2

///////////////////////////////////////////////////////////////////////////////
///     QUEUE STRUCTURE
///////////////////////////////////////////////////////////////////////////////

struct QueueSt
{
    elem_t *elems;
    size_t capacity;
    size_t front;
    size_t back;
    size_t size;
    char copy_enabled;
    copy_operator_t operator_copy;
    delete_operator_t operator_delete;
};

///////////////////////////////////////////////////////////////////////////////
///     QUEUE MACRO UTILITARIES
///////////////////////////////////////////////////////////////////////////////

static inline elem_t id(elem_t e) {
    return e;
}

/**
 * Macro to allocate all memory used by the queue
 */
#define QUEUE_INIT(__copy_op, __delete_op, __n_elems) \
({ \
    Queue __ptr = malloc(sizeof(struct QueueSt)); \
    if (__ptr) { \
        __ptr->elems = malloc(sizeof(elem_t) * (__n_elems)); \
        if (__ptr->elems) { \
            __ptr->capacity = (__n_elems); \
            __ptr->front = 0; \
            __ptr->back = 0; \
            __ptr->size = 0; \
            __ptr->copy_enabled = __copy_op ? true : false; \
            __ptr->operator_copy = __copy_op ? __copy_op : id; \
            __ptr->operator_delete = __delete_op; \
        } else { \
            free(__ptr); \
            __ptr = NULL; \
        } \
    } \
    __ptr; \
})

/**
 * Macro to shift entire queue to the left of the elems array
 */
#define QUEUE_SHIFT(__ptr) \
    memmove(__ptr->elems, __ptr->elems + __ptr->front, sizeof(elem_t) * __ptr->size); \
    __ptr->front = 0; \
    __ptr->back = __ptr->size

///////////////////////////////////////////////////////////////////////////////
///     QUEUE FUNCTIONS TO EXPORT
///////////////////////////////////////////////////////////////////////////////

inline Queue queue__empty_copy_disabled(void)
{
    return QUEUE_INIT(NULL, NULL, DEFAULT_QUEUE_CAPACITY);
}

inline Queue queue__empty_copy_enabled(const copy_operator_t copy_op, const delete_operator_t delete_op)
{
    if (!copy_op || !delete_op) return NULL;

    return QUEUE_INIT(copy_op, delete_op, DEFAULT_QUEUE_CAPACITY);
}

char queue__enqueue(const Queue q, const elem_t element)
{
    if (!q) return FAILURE;

    // Adjust capacity if necessary
    if (q->back == q->capacity) {
        if (ENSURE_CAPACITY(q) < 0) return FAILURE;
    }

    q->elems[q->back] = q->operator_copy(element);
    q->size++;
    q->back++;

    return SUCCESS;
}

char queue__dequeue(const Queue q, elem_t *front)
{
    size_t new_capacity;
    if (!q || !q->size) return FAILURE;

    if (front) {
        *front = q->elems[q->front];
    } else {
        if (q->operator_delete) {
            q->operator_delete(q->elems[q->front]);
        }
    }

    q->size--;
    q->front++;

    new_capacity = q->capacity>>1;
    if (q->size < new_capacity && new_capacity >= DEFAULT_QUEUE_CAPACITY) {
        QUEUE_SHIFT(q);
        ARRAY_RESIZE(q, new_capacity);
    }

    return SUCCESS;
}

char queue__front(const Queue q, elem_t *front)
{
    if (!q || !q->size || !front) return FAILURE;

    *front = q->operator_copy(q->elems[q->front]);

    return SUCCESS;
}

char queue__back(const Queue q, elem_t *back)
{
    if (!q || !q->size || !back) return FAILURE;

    *back = q->operator_copy(q->elems[q->back - 1]);

    return SUCCESS;
}

inline char queue__is_copy_enabled(const Queue q)
{
    return !q ? FAILURE : q->copy_enabled;
}

inline char queue__is_empty(const Queue q)
{
    return !q ? FAILURE : !q->size;
}

inline size_t queue__size(const Queue q)
{
    return !q ? (size_t)FAILURE : q->size;
}

Queue queue__from_array(Queue q, void *A, const size_t n_elems, const size_t size)
{
    if (!A) return NULL;

    if (!q) {
        if (!(q = QUEUE_INIT(NULL, NULL, n_elems))) return NULL;
    } else {
        QUEUE_SHIFT(q);
        if (ARRAY_RESIZE(q, q->size + n_elems) < 0) return NULL;
    }

    for (size_t i = 0; i < n_elems; i++) {
        q->elems[q->size + i] = q->operator_copy(A);
        PTR_INCREMENT(A, size);
    }

    q->size += n_elems;
    q->back = q->size;

    return q;
}

elem_t *queue__to_array(const Queue q)
{
    if (!q || !q->size) return NULL;

    elem_t *res = malloc(sizeof(elem_t) * q->size);
    if (!res) return NULL;

    size_t k = 0;
    if (q->copy_enabled) {
        for (size_t i = q->front; i < q->back; i++) {
            res[k] = q->operator_copy(q->elems[i]);
            k++;
        }
    } else {
        memcpy(res, q->elems + q->front, sizeof(elem_t) * q->size);
    }

    return res;
}

inline void queue__sort(const Queue q, const compare_func_t f)
{
    if (!q || q->size < 2) return;

    qsort(q->elems + q->front, q->size, sizeof(elem_t), f);
}

void queue__shuffle(const Queue q)
{
    if (!q || q->size < 2) return;

    ARRAY_SHUFFLE(q->elems, q->front, q->back);
}

void queue__foreach(const Queue q, const applying_func_t f, void *user_data)
{
    char repeated;
    if (!q || !q->size) return;

    if (q->copy_enabled) {
        for (size_t i = q->front; i < q->back; i++) {
            f(q->elems[i], user_data);
        }
    }
    /* If the same element is more than 1 time in the queue,
    this is necessary in order to prevent multiple application of f on this element.
    */
    else{
        repeated = false;
        for (size_t i = q->front; i < q->back; i++) {
            for (size_t j = q->front; j < i && !repeated; j++) {
                if (q->elems[i] == q->elems[j]) {
                    repeated = true;
                }
            }
            if (!repeated) {
                f(q->elems[i], user_data);
            }
            repeated = false;
        }
    }
}

void queue__clean_NULL(const Queue q)
{
    if (!q) return;

    CLEAN_NULL_ELEMS(q, q->front, q->back);

    q->back = q->front + q->size;
}

void queue__clear(const Queue q)
{
    if (!q) return;

    FREE_ELEMS(q, q->front, q->back);
    ARRAY_RESIZE(q, DEFAULT_QUEUE_CAPACITY);

    q->front = 0;
    q->back = 0;
    q->size = 0;
}

void queue__free(const Queue q)
{
    if (!q) return;

    FREE_ELEMS(q, q->front, q->back);

    free(q->elems);
    free(q);
}

void queue__debug(const Queue q, const debug_operator_t debug_op)
{
    setvbuf (stdout, NULL, _IONBF, 0);

    printf("\n");
    if (!q) {
        printf("\tInvalid queue (NULL)");
    } else if (!debug_op) {
        printf("\tInvalid degug operator (NULL)");
    } else {
        queue__is_copy_enabled(q) ? printf("\tQueue with copy enabled:")
                                  : printf("\tQueue with copy disabled:");
        printf("\n\tQueue size: %lu\n\tQueue capacity: %lu\n\tQueue front: %lu\n\tQueue back: %lu\n\tQueue content: \n\t", q->size
                                                                                                                         , q->capacity
                                                                                                                         , q->front
                                                                                                                         , q->back);
        printf("{ ");
        for (size_t i = 0; i < q->capacity; i++) {
            if (q->front < i && i < q->back) {
                debug_op(q->elems[i]);
            } else {
                printf("_ ");
            }
        }
        printf("}");
    }
    printf("\n");
}
