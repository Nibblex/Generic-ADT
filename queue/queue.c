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
    copy_operator_t operator_copy;
    delete_operator_t operator_delete;
};

///////////////////////////////////////////////////////////////////////////////
///     QUEUE MACRO UTILITARIES
///////////////////////////////////////////////////////////////////////////////

/**
 * Macro to allocate all memory used by the queue
 */
#define QUEUE_INIT(__ptr, __copy_op, __delete_op) \
    do { \
        __ptr = malloc(sizeof(struct QueueSt)); \
        if (!__ptr) return NULL; \
        __ptr->elems = malloc(sizeof(elem_t) * DEFAULT_QUEUE_CAPACITY); \
        if (!__ptr->elems) { \
            free(__ptr); \
            return NULL; \
        } \
        __ptr->capacity = DEFAULT_QUEUE_CAPACITY; \
        __ptr->front = 0; \
        __ptr->back = 0; \
        __ptr->size = 0; \
        __ptr->operator_copy = __copy_op; \
        __ptr->operator_delete = __delete_op; \
    } while (false)

/**
 * Macro to grow the queue to __size capacity
 */
#define QUEUE_GROW(__ptr) \
    ARRAY_GROW(__ptr)

/**
 * Macro to shrink the queue to the new capacity
 */
static char queue__shrink(size_t new_capacity, Queue q)
{
    size_t n_elems_to_end;

    if (new_capacity < q->size) return FAILURE;

    elem_t *new_elems = malloc(sizeof(elem_t) * new_capacity);
    if (!new_elems) return FAILURE;

    if (q->front < q->back) {
        memcpy(new_elems, q->elems + q->front, sizeof(elem_t) * q->size);
    } else {
        n_elems_to_end = q->capacity - q->front;
        memcpy(new_elems, q->elems + q->front, sizeof(elem_t) * n_elems_to_end);
        memcpy(new_elems + n_elems_to_end, q->elems, sizeof(elem_t) * q->back);
    }

    free(q->elems);
    q->elems = new_elems;
    q->capacity = new_capacity;
    q->front = 0;
    q->back = q->size;

    return SUCCESS;
}

///////////////////////////////////////////////////////////////////////////////
///     QUEUE FUNCTIONS TO EXPORT
///////////////////////////////////////////////////////////////////////////////

inline Queue queue__empty_copy_disabled(void)
{
    Queue q = NULL;
    QUEUE_INIT(q, NULL, NULL);

    return q;
}

inline Queue queue__empty_copy_enabled(const copy_operator_t copy_op, const delete_operator_t delete_op)
{
    if (!copy_op || !delete_op) return NULL;

    Queue q = NULL;
    QUEUE_INIT(q, copy_op, delete_op);

    return q;
}

char queue__enqueue(const Queue q, const elem_t element)
{
    elem_t *realloc_res = NULL;
    if (!q) return FAILURE;

    // Adjust capacity if necessary
    if (q->size == q->capacity || !q->elems) {
        QUEUE_GROW(q);

        q->back = q->front + q->size;
        memmove(q->elems + q->size, q->elems, sizeof(elem_t) * q->front);
    }

    q->elems[q->back] = q->operator_copy ? q->operator_copy(element) : element;
    q->size++;
    q->back = (q->size != q->capacity && q->back + 1 == q->capacity) ? 0 : q->back + 1;

    return SUCCESS;
}

char queue__dequeue(const Queue q, elem_t *front)
{
    size_t new_capacity;
    if (!q || !q->elems || !q->size) return FAILURE;

    if (front) {
        *front = q->elems[q->front];
    } else {
        if (q->operator_delete) {
            q->operator_delete(q->elems[q->front]);
        }
    }

    q->front = (q->front + 1 == q->capacity) ? 0 : q->front + 1;
    q->size--;

    new_capacity = q->capacity>>1;
    if (q->size < new_capacity && new_capacity >= DEFAULT_QUEUE_CAPACITY) {
        if (queue__shrink(new_capacity, q)) return FAILURE;
    }

    return SUCCESS;
}

char queue__front(const Queue q, elem_t *front)
{
    if (!q || !q->elems || !q->size || !front) return FAILURE;

    *front = q->operator_copy ? q->operator_copy(q->elems[q->front]) : q->elems[q->front];

    return SUCCESS;
}

char queue__back(const Queue q, elem_t *back)
{
    size_t index;
    if (!q || !q->elems || !q->size || !back) return FAILURE;

    index = q->back ? q->back - 1 : q->capacity - 1;

    *back = q->operator_copy ? q->operator_copy(q->elems[index]) : q->elems[index];

    return SUCCESS;
}

inline char queue__is_copy_enabled(const Queue q)
{
    return !q ? FAILURE : q->operator_copy && q->operator_delete;
}

inline char queue__is_empty(const Queue q)
{
    return !q ? FAILURE : q ? !q->size : true;
}

inline size_t queue__size(const Queue q)
{
    return !q ? (size_t)FAILURE : q->size;
}

Queue queue__from_array(Queue q, void *A, const size_t n_elems, const size_t size)
{
    char new_queue = false;
    if (!A) return NULL;

    if (!q) {
        new_queue = true;
        QUEUE_INIT(q, NULL, NULL);
        if (!q) return NULL;
    }

    for (size_t i = 0; i < n_elems; i++) {
        if (queue__enqueue(q, A)) goto error;
        PTR_INCREMENT(A, size);
    }

    return q;

error:
    if (new_queue) {
        queue__free(q);
        return NULL;
    }

    return q;
}

elem_t *queue__to_array(const Queue q)
{
    if (!q || !q->elems || !q->size) return NULL;

    elem_t *res = malloc(sizeof(elem_t) * q->size);
    if (!res) return NULL;

    size_t k = 0;
    if (q->front < q->back) {
        for (size_t i = q->front; i < q->back; i++) {
            res[k] = q->operator_copy ? q->operator_copy(q->elems[i]) : q->elems[i];
            k++;
        }
    } else {
        for (size_t i = q->front; i < q->capacity; i++) {
            res[k] = q->operator_copy ? q->operator_copy(q->elems[i]) : q->elems[i];
            k++;
        }
        for (size_t i = 0; i < q->back; i++) {
            res[k] = q->operator_copy ? q->operator_copy(q->elems[i]) : q->elems[i];
            k++;
        }
    }

    return res;
}

inline void queue__sort(const Queue q, const compare_func_t f)
{
    if (!q || !q->elems || q->size < 2) return;

    if (q->back < q->front) {
        queue__shrink(q->capacity, q);
    }

    qsort((q->back == q->front) ? q->elems : q->elems + q->front, q->size, sizeof(elem_t), f);
}

void queue__shuffle(const Queue q)
{
    size_t a, b;
    if (!q || !q->elems || q->size < 2) return;

    if (q->front < q->back) {
        ARRAY_SHUFFLE(q->elems, q->front, q->back);
    } else {
        ARRAY_SHUFFLE(q->elems, q->front, q->capacity);
        ARRAY_SHUFFLE(q->elems, 0, q->back);
    }
}

void queue__foreach(const Queue q, const applying_func_t f, void *user_data)
{
    char repeated;
    if (!q || !q->elems || !q->size) return;

    if (q->operator_copy && q->operator_delete) {
        if (q->front < q->back) {
            for (size_t i = q->front; i < q->back; i++) {
                f(q->elems[i], user_data);
            }
        } else {
            for (size_t i = q->front; i < q->capacity; i++) {
                f(q->elems[i], user_data);
            }
            for (size_t i = 0; i < q->back; i++) {
                f(q->elems[i], user_data);
            }
        }
    }
    /* If the same element is more than 1 time in the queue,
    this is necessary in order to prevent multiple application of f on this element.
    */
    else{
        repeated = false;
        if (q->front < q->back) {
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
        } else {
            for (size_t i = q->front; i < q->capacity; i++) {
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
            for (size_t i = 0; i < q->back; i++) {
                for (size_t j = q->front; j < q->capacity && !repeated; j++) {
                    if (q->elems[i] == q->elems[j]) {
                        repeated = true;
                    }
                }
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
}

void queue__clean_NULL(const Queue q)
{
    size_t k;
    if (!q || !q->elems) return;

    if (q->back < q->front) {
        queue__shrink(q->capacity, q);
    }

    ARRAY_CLEAN_NULL(q->elems, q->front, q->back);

    q->size = k;
    q->back = q->front + q->size;
}

void queue__clear(const Queue q)
{
    if (!q) return;

    if (q->operator_delete) {
        queue__foreach(q, (applying_func_t)(void *)q->operator_delete, NULL);
    }

    free(q->elems);
    q->elems = malloc(sizeof(elem_t) * DEFAULT_QUEUE_CAPACITY);
    q->capacity = DEFAULT_QUEUE_CAPACITY;
    q->front = 0;
    q->back = 0;
    q->size = 0;
}

void queue__free(const Queue q)
{
    if (!q) return;

    if (q->operator_delete) {
        queue__foreach(q, (applying_func_t)(void *)q->operator_delete, NULL);
    }

    free(q->elems);
    free(q);
}

void queue__debug(const Queue q, void (*debug_op) (elem_t))
{
    setvbuf (stdout, NULL, _IONBF, 0);
    printf("\n");
    if (!q || !q->elems) {
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
        printf("In queue order: { ");
        if (q->size) {
            if (q->front < q->back) {
                for (size_t i = q->front; i < q->back; i++) {
                    debug_op(q->elems[i]);
                }
            } else {
                for (size_t i = q->front; i < q->capacity; i++) {
                    debug_op(q->elems[i]);
                }
                for (size_t i = 0; i < q->back; i++) {
                    debug_op(q->elems[i]);
                }
            }
        }
        printf("}\n\tIn elems order: { ");
        if (!q->size) {
            for (size_t i = 0; i < q->capacity; i++) {
                printf("_ ");
            }
        } else {
            if (q->front < q->back) {
                for (size_t i = 0; i < q->front; i++) {
                    printf("_ ");
                }
                for (size_t i = q->front; i < q->back; i++) {
                    debug_op(q->elems[i]);
                }
                for (size_t i = q->back; i < q->capacity; i++) {
                    printf("_ ");
                }
            } else {
                for (size_t i = 0; i < q->back; i++) {
                    debug_op(q->elems[i]);
                }
                for (size_t i = q->back; i < q->front; i++) {
                    printf("_ ");
                }
                for (size_t i = q->front; i < q->capacity; i++) {
                    debug_op(q->elems[i]);
                }
            }
        }
        printf("}");
    }
    printf("\n");
}
