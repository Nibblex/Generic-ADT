#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "stack.h"

#define DEFAULT_STACK_CAPACITY 2

///////////////////////////////////////////////////////////////////////////////
///     STACK STRUCTURE
///////////////////////////////////////////////////////////////////////////////

struct StackSt
{
    elem_t *elems;
    size_t capacity;
    size_t size;
    char copy_enabled;
    copy_operator_t operator_copy;
    delete_operator_t operator_delete;
};

///////////////////////////////////////////////////////////////////////////////
///     STACK MACRO UTILITARIES
///////////////////////////////////////////////////////////////////////////////

static inline elem_t id(elem_t e) {
    return e;
}

/**
 * Macro to allocate all memory used by the stack
 */
#define STACK_INIT(__copy_op, __delete_op, __n_elems) \
({ \
    Stack __ptr = malloc(sizeof(struct StackSt)); \
    if (__ptr) { \
        __ptr->elems = malloc(sizeof(elem_t) * (__n_elems)); \
        if (__ptr->elems) { \
            __ptr->capacity = (__n_elems); \
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

///////////////////////////////////////////////////////////////////////////////
///     STACK FUNCTIONS TO EXPORT
///////////////////////////////////////////////////////////////////////////////

inline Stack stack__empty_copy_disabled(void)
{
    return STACK_INIT(NULL, NULL, DEFAULT_STACK_CAPACITY);
}

inline Stack stack__empty_copy_enabled(const copy_operator_t copy_op, const delete_operator_t delete_op)
{
    if (!copy_op || !delete_op) return NULL;

    return STACK_INIT(copy_op, delete_op, DEFAULT_STACK_CAPACITY);
}

inline char stack__is_copy_enabled(const Stack s)
{
    return !s ? FAILURE : s->copy_enabled;
}

inline char stack__is_empty(const Stack s)
{
    return !s ? FAILURE : !s->size;
}

inline size_t stack__size(const Stack s)
{
    return !s ? (size_t)FAILURE : s->size;
}

char stack__push(const Stack s, const elem_t element)
{
    if (!s) return FAILURE;

    // Adjust capacity if necessary
    if (s->size == s->capacity) {
        if (ENSURE_CAPACITY(s) < 0) return FAILURE;
    }

    s->elems[s->size] = s->operator_copy(element);
    s->size++;

    return SUCCESS;
}

char stack__pop(const Stack s, elem_t *top)
{
    size_t new_capacity;
    if (!s || !s->size) return FAILURE;

    if (top) {
        *top = s->elems[s->size-1];
    } else {
        if (s->operator_delete) {
            s->operator_delete(s->elems[s->size-1]);
        }
    }

    s->size--;

    new_capacity = s->capacity>>1;
    if (s->size < new_capacity && new_capacity >= DEFAULT_STACK_CAPACITY) {
        ARRAY_RESIZE(s, new_capacity);
    }

    return SUCCESS;
}

char stack__top(const Stack s, elem_t *top)
{
    if (!s || !s->size || !top) return FAILURE;

    *top = s->operator_copy(s->elems[s->size-1]);

    return SUCCESS;
}

Stack stack__from_array(Stack s, void *A, const size_t n_elems, const size_t size)
{
    if (!A) return NULL;

    if (!s) {
        if (!(s = STACK_INIT(NULL, NULL, n_elems))) return NULL;
    } else {
        if (ARRAY_RESIZE(s, s->size + n_elems) < 0) return NULL;
    }

    for (size_t i = 0; i < n_elems; i++) {
        s->elems[s->size + i] = s->operator_copy(A);
        PTR_INCREMENT(A, size);
    }

    s->size += n_elems;

    return s;
}

elem_t *stack__dump(const Stack s)
{
    if (!s || !s->size) return NULL;

    elem_t *res = malloc(sizeof(elem_t) * s->size);
    if (!res) return NULL;

    memcpy(res, s->elems, sizeof(elem_t) * s->size);
    ARRAY_RESIZE(s, DEFAULT_STACK_CAPACITY);

    s->size = 0;

    return res;
}

elem_t *stack__to_array(const Stack s)
{
    if (!s || !s->size) return NULL;

    elem_t *res = malloc(sizeof(elem_t) * s->size);
    if (!res) return NULL;

    if (s->copy_enabled) {
        for (size_t i = 0; i < s->size; i++) {
            res[i] = s->operator_copy(s->elems[i]);
        }
    } else {
        memcpy(res, s->elems, sizeof(elem_t) * s->size);
    }

    return res;
}

void stack__reverse(const Stack s)
{
    if (!s || s->size < 2) return;

    for (size_t i = 0, j = s->size - 1; i < j; i++, j--) {
        PTR_SWAP(s->elems[i], s->elems[j]);
    }
}

void stack__shuffle(const Stack s)
{
    if (!s || s->size < 2) return;

    ARRAY_SHUFFLE(s->elems, 0, s->size);
}

inline void stack__sort(const Stack s, const compare_func_t f)
{
    if (!s || s->size < 2) return;

    qsort(s->elems, s->size, sizeof(elem_t), f);
}

void stack__foreach(const Stack s, const applying_func_t f, void *user_data)
{
    char repeated;
    if (!s || !s->size) return;

    if (s->copy_enabled) {
        for (size_t i = 0; i < s->size; i++) {
            f(s->elems[i], user_data);
        }
    }
    /* If the same element is more than 1 time in the stack,
    this is necessary in order to prevent multiple application of f on this element.
    */
    else {
        repeated = false;
        for (size_t i = 0; i < s->size; i++) {
            for (size_t j = 0; j < i && !repeated; j++) {
                if (s->elems[i] == s->elems[j]) {
                    repeated = true;
                }
            }
            if (!repeated) {
                f(s->elems[i], user_data);
            }
            repeated = false;
        }
    }
}

void stack__clean_NULL(Stack s)
{
    if (!s) return;

    CLEAN_NULL_ELEMS(s, 0, s->size);
}

void stack__clear(const Stack s)
{
    if (!s) return;

    FREE_ELEMS(s, 0, s->size);
    ARRAY_RESIZE(s, DEFAULT_STACK_CAPACITY);

    s->size = 0;
}

void stack__free(const Stack s)
{
    if (!s) return;

    FREE_ELEMS(s, 0, s->size);

    free(s->elems);
    free(s);
}

void stack__debug(const Stack s, const debug_operator_t debug_op)
{
    setvbuf (stdout, NULL, _IONBF, 0);

    printf("\n");
    if (!s) {
        printf("\tInvalid stack (NULL)");
    } else if (!debug_op) {
        printf("\tInvalid degug operator (NULL)");
    } else {
        stack__is_copy_enabled(s) ? printf("\tStack with copy enabled:")
                                  : printf("\tStack with copy disabled:");
        printf("\n\tStack size: %lu, \n\tStack capacity: %lu, \n\tStack content: \n\t", s->size, s->capacity);
        printf("{ ");
        for (size_t i = 0; i < s->capacity; i++) {
            if (i < s->size) {
                debug_op(s->elems[i]);
            } else {
                printf("_ ");
            }
        }
        printf("}");
    }
    printf("\n");
}
