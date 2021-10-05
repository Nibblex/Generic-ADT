#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "queue.h"
#include "../common/vec.h"

#define DEFAULT_QUEUE_CAPACITY 2

///////////////////////////////////////////////////////////////////////////////
///     QUEUE STRUCTURE
///////////////////////////////////////////////////////////////////////////////

struct QueueSt
{
    elem_t *elems;
    size_t front;
    size_t back;
    size_t length;
    size_t capacity;
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

static inline void skip(elem_t e) {
    return;
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
            __ptr->front = 0; \
            __ptr->back = 0; \
            __ptr->length = 0; \
            __ptr->capacity = (__n_elems); \
            __ptr->copy_enabled = __copy_op ? true : false; \
            __ptr->operator_copy = __copy_op ? __copy_op : id; \
            __ptr->operator_delete = __delete_op ? __delete_op : skip; \
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
    memmove(__ptr->elems, __ptr->elems + __ptr->front, sizeof(elem_t) * __ptr->length); \
    __ptr->front = 0; \
    __ptr->back = __ptr->length

///////////////////////////////////////////////////////////////////////////////
///     QUEUE FUNCTIONS TO EXPORT
///////////////////////////////////////////////////////////////////////////////

Queue queue__empty_copy_disabled(void) {
    return QUEUE_INIT(NULL, NULL, DEFAULT_QUEUE_CAPACITY);
}

Queue queue__empty_copy_enabled(const copy_operator_t copy_op, const delete_operator_t delete_op) {
    if (!copy_op || !delete_op) return NULL;

    return QUEUE_INIT(copy_op, delete_op, DEFAULT_QUEUE_CAPACITY);
}

inline char queue__is_copy_enabled(const Queue q) {
    return !q ? FAILURE : q->copy_enabled;
}

inline char queue__is_empty(const Queue q) {
    return !q ? FAILURE : !q->length;
}

inline size_t queue__length(const Queue q) {
    return !q ? SIZE_MAX : q->length;
}

char queue__enqueue(const Queue q, const elem_t element) {
    if (!q) return FAILURE;

    if (ENSURE_CAPACITY(q) < 0) return FAILURE;

    q->elems[q->back] = q->operator_copy(element);
    q->back++;
    q->length++;

    return SUCCESS;
}

char queue__dequeue(const Queue q, elem_t *front) {
    size_t new_capacity;
    if (!q || !q->length) return FAILURE;

    if (front) {
        *front = q->elems[q->front];
    } else {
        q->operator_delete(q->elems[q->front]);
    }

    q->front++;
    q->length--;

    new_capacity = q->capacity>>1;
    if (q->length < new_capacity && new_capacity >= DEFAULT_QUEUE_CAPACITY) {
        QUEUE_SHIFT(q);
        RESIZE(q, new_capacity);
    }

    return SUCCESS;
}

char queue__remove_nth(const Queue q, const size_t i) {
    if (!q || i >= q->length) return FAILURE;

    q->operator_delete(q->elems[i]);
    q->elems[i] = NULL;

    return SUCCESS;
}

char queue__peek_front(const Queue q, elem_t *front) {
    if (!q || !q->length || !front) return FAILURE;

    *front = q->operator_copy(q->elems[q->front]);

    return SUCCESS;
}

char queue__peek_back(const Queue q, elem_t *back) {
    if (!q || !q->length || !back) return FAILURE;

    *back = q->operator_copy(q->elems[q->back - 1]);

    return SUCCESS;
}

char queue__peek_nth(const Queue q, const size_t i, elem_t *nth) {
    if (!q || !q->length || !nth || i < q->front || i >= q->back) return FAILURE;

    *nth = q->operator_copy(q->elems[i]);

    return SUCCESS;
}

char queue__swap(const Queue q, const size_t i, const size_t j) {
    if (!q || i < q->front || i >= q->back || j < q->front || j >= q->back) return FAILURE;

    SWAP(q, i, j);

    return SUCCESS;
}

Queue queue__copy(const Queue q) {
    if (!q) return NULL;

    Queue copy = QUEUE_INIT(q->operator_copy, q->operator_delete, q->length);
    if (!copy) return NULL;

    COPY(copy, q, q->front, q->length);

    copy->front = 0;
    copy->back = q->length;

    return copy;
}

Queue queue__from_array(Queue q, void *A, const size_t n_elems, const size_t size) {
    if (!A) return NULL;

    if (!q) {
        if (!(q = QUEUE_INIT(NULL, NULL, n_elems))) return NULL;
    } else {
        if (RESIZE(q, q->back + n_elems) < 0) return NULL;
    }

    FROM_ARRAY(q, A, n_elems, size);

    return q;
}

elem_t *queue__dump(const Queue q) {
    if (!q || !q->length) return NULL;

    elem_t *res = malloc(sizeof(elem_t) * q->length);
    if (!res) return NULL;

    memcpy(res, q->elems + q->front, sizeof(elem_t) * q->length);
    RESIZE(q, DEFAULT_QUEUE_CAPACITY);

    q->front = 0;
    q->back = 0;
    q->length = 0;

    return res;
}

elem_t *queue__to_array(const Queue q) {
    if (!q || !q->length) return NULL;

    elem_t *res = malloc(sizeof(elem_t) * q->length);
    if (!res) return NULL;

    size_t k = 0;
    if (q->copy_enabled) {
        for (size_t i = q->front; i < q->back; i++) {
            res[k] = q->operator_copy(q->elems[i]);
            k++;
        }
    } else {
        memcpy(res, q->elems + q->front, sizeof(elem_t) * q->length);
    }

    return res;
}

size_t queue__ptr_search(const Queue q, const elem_t elem) {
    if (!q) return SIZE_MAX;

    return PTR_SEARCH(q, q->front, q->back, elem);
}

size_t queue__search(const Queue q, const elem_t elem, const compare_func_t match) {
    if (!q || !match) return SIZE_MAX;

    return SEARCH(q, q->front, q->back, elem, match);
}

char queue__ptr_contains(const Queue q, const elem_t elem) {
    if (!q) return FAILURE;

    return PTR_SEARCH(q, q->front, q->back, elem) != SIZE_MAX;
}

char queue__contains(const Queue q, const elem_t elem, const compare_func_t match) {
    if (!q || !match) return FAILURE;

    return SEARCH(q, q->front, q->back, elem, match) != SIZE_MAX;
}

char queue__cmp(const Queue q, const Queue w, const compare_func_t match) {
    if (!q || !w || !match) return FAILURE;

    if (q == w) return true;
    if (q->length != w->length) return false;

    return ARRAY_CMP(q->elems + q->front, w->elems + w->front, match, q->length);
}

void queue__foreach(const Queue q, const applying_func_t func, void *user_data) {
    if (!q || !func) return;

    FOREACH(q, func, user_data, q->front, q->back);
}

void queue__filter(const Queue q, const filter_func_t pred, void *user_data) {
    if (!q || !pred) return;

    FILTER(q, q->front, q->back, pred, user_data);

    q->back = q->front + q->length;
}

char queue__all(const Queue q, const filter_func_t pred, void *user_data) {
    if (!q || !pred) return FAILURE;

    return ALL(q, q->front, q->back, pred, user_data);
}

char queue__any(const Queue q, const filter_func_t pred, void *user_data) {
    if (!q || !pred) return FAILURE;

    return ANY(q, q->front, q->back, pred, user_data);
}

void queue__reverse(const Queue q) {
    if (!q || q->length < 2) return;

    for (size_t i = q->front, j = q->back - 1; i < j; i++, j--) {
        SWAP(q, i, j);
    }
}

void queue__shuffle(const Queue q, const unsigned int seed) {
    if (!q) return;

    SHUFFLE(q, q->front, q->back, seed);
}

void queue__sort(const Queue q, const compare_func_t cmp) {
    if (!q || !cmp) return;

    qsort(q->elems + q->front, q->length, sizeof(elem_t), cmp);
}

void queue__clean_NULL(const Queue q) {
    if (!q) return;

    CLEAN_NULL_ELEMS(q, q->front, q->back);

    q->back = q->front + q->length;
}

void queue__clear(const Queue q) {
    if (!q) return;

    FREE_ELEMS(q, q->front, q->back);
    RESIZE(q, DEFAULT_QUEUE_CAPACITY);

    q->front = 0;
}

void queue__free(const Queue q) {
    if (!q) return;

    FREE_ELEMS(q, q->front, q->back);

    free(q->elems);
    free(q);
}

void queue__debug(const Queue q, const debug_func_t debug) {
    setvbuf (stdout, NULL, _IONBF, 0);

    printf("\n");
    if (!q) {
        printf("\tInvalid queue (NULL)");
    } else if (!debug) {
        printf("\tInvalid degug function (NULL)");
    } else {
        queue__is_copy_enabled(q) ? printf("\tQueue with copy enabled:")
                                  : printf("\tQueue with copy disabled:");
        printf("\n\tQueue size: %lu\n\tQueue capacity: %lu\n\tQueue front: %lu\n\tQueue back: %lu\n\tQueue content: \n\t", q->length
                                                                                                                         , q->capacity
                                                                                                                         , q->front
                                                                                                                         , q->back);
        printf("{ ");
        for (size_t i = 0; i < q->capacity; i++) {
            if (q->front <= i && i < q->back) {
                debug(q->elems[i]);
            } else {
                printf("_ ");
            }
        }
        printf("}");
    }
    printf("\n");
}
