#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "stack.h"

#define DEFAULT_STACK_CAPACITY 2

////////////////////////////////////////////////////////////////////
///     STACK STRUCTURE
////////////////////////////////////////////////////////////////////

struct StackSt
{
    elem_t *elems;
    size_t capacity;
    size_t size;
    copy_operator_t operator_copy;
    delete_operator_t operator_delete;
};

////////////////////////////////////////////////////////////////////
///     STACK FUNCTIONS UTILITARIES
////////////////////////////////////////////////////////////////////

static Stack stack__init(const copy_operator_t copy_op, const delete_operator_t delete_op)
{
    Stack s = malloc(sizeof(struct StackSt));
    if (!s) return NULL;

    s->elems = malloc(sizeof(elem_t) * DEFAULT_STACK_CAPACITY);
    if (!s->elems) {
        free(s);
        return NULL;
    }

    s->capacity = DEFAULT_STACK_CAPACITY;
    s->size = 0;
    s->operator_copy = copy_op;
    s->operator_delete = delete_op;

    return s;
}

////////////////////////////////////////////////////////////////////
///     STACK FUNCTIONS IMPLEMENTATION
////////////////////////////////////////////////////////////////////

inline Stack stack__empty_copy_enabled(const copy_operator_t copy_op, const delete_operator_t delete_op)
{
    if (!copy_op || !delete_op) return NULL;

    return stack__init(copy_op, delete_op);
}

inline Stack stack__empty_copy_disabled(void)
{
    return stack__init(NULL, NULL);
}

char stack__push(const Stack s, const elem_t element)
{
    size_t new_capacity;
    elem_t *realloc_res = NULL;
    if (!s) return FAILURE;

    // Adjust capacity if necessary
    if (s->size == s->capacity) {
        new_capacity = s->capacity<<1;
        do {
            realloc_res = realloc(s->elems, sizeof(elem_t) * new_capacity);
            if (!realloc_res) {
                new_capacity = (new_capacity + s->capacity)>>1;
            }
        } while (!realloc_res);
        if (new_capacity == s->capacity) return FAILURE;

        s->capacity = new_capacity;
        s->elems = realloc_res;
    }

    s->elems[s->size] = s->operator_copy ? s->operator_copy(element) : element;
    s->size++;

    return SUCCESS;
}

char stack__pop(const Stack s, elem_t *top)
{
    size_t new_capacity;
    elem_t *realloc_res = NULL;
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
    if (s->size < new_capacity && s->capacity > DEFAULT_STACK_CAPACITY) {
        realloc_res = realloc(s->elems, sizeof(elem_t) * new_capacity);
        if (!realloc_res) return FAILURE;

        s->elems = realloc_res;
        s->capacity = new_capacity;
    }

    return SUCCESS;
}

char stack__top(const Stack s, elem_t *top)
{
    if (!s || !s->size || !top) return FAILURE;

    *top = s->operator_copy ? s->operator_copy(s->elems[s->size-1]) : s->elems[s->size-1];

    return SUCCESS;
}

inline char stack__is_copy_enabled(const Stack s)
{
    return s->operator_copy && s->operator_delete;
}

inline char stack__is_empty(const Stack s)
{
    return s ? !s->size : true;
}

size_t stack__size(const Stack s)
{
    return s ? s->size : 0;
}

Stack stack__from_array(Stack s, const elem_t *A, const size_t n_elems, const DataType type)
{
    char new_stack = false;
    if (!A) return s;

    if (!s) {
        new_stack = true;
        s = stack__init(NULL, NULL);
        if (!s) return NULL;
    }

    switch (type) {
        case CHAR:
            for (size_t i = 0; i < n_elems; i++) {
                if (stack__push(s, (char *)A+i)) goto error;
            }
            break;
        case INT:
            for (size_t i = 0; i < n_elems; i++) {
                if (stack__push(s, (int *)A+i)) goto error;
            }
            break;
        case UINT:
            for (size_t i = 0; i < n_elems; i++) {
                if (stack__push(s, (unsigned int *)A+i)) goto error;
            }
            break;
        case FLOAT:
            for (size_t i = 0; i < n_elems; i++) {
                if (stack__push(s, (float *)A+i)) goto error;
            }
            break;
        case STRING:
            for (size_t i = 0; i < n_elems; i++) {
                if (stack__push(s, (char **)A+i)) goto error;
            }
            break;
        case GENERIC:
            for (size_t i = 0; i < n_elems; i++) {
                if (stack__push(s, (elem_t *)A+i)) goto error;
            }
            break;
        default:
            goto error;
    }

    return s;

error:
    if (new_stack) {
        stack__free(s);
    }

    return NULL;
}

elem_t *stack__to_array(const Stack s)
{
    if (!s || !s->size) return NULL;

    elem_t *res = malloc(sizeof(elem_t) * s->size);
    if (!res) return NULL;

    for (size_t i = 0; i < s->size; i++) {
        res[i] = s->operator_copy ? s->operator_copy(s->elems[i]) : s->elems[i];
    }

    return res;
}

inline void stack__sort(const Stack s, const compare_func_t f)
{
    if (!s || s->size < 2) return;

    qsort(s->elems, s->size, sizeof(elem_t), f);
}

void stack__shuffle(const Stack s)
{
    size_t a, b;
    if (!s) return;

    for (size_t i = 0; i < s->size; i++){
        a = (size_t) (rand() % (int)s->size);
        b = (size_t) (rand() % (int)s->size);
        SWAP(s->elems[a], s->elems[b]);
    }
}

void stack__foreach(const Stack s, const applying_func_t f, void *user_data)
{
    char repeated;
    if (!s || !s->size) return;

    if (s->operator_copy && s->operator_delete) {
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
    size_t k = 0;
    for (size_t i = 0; i < s->size; i++) {
        if (s->elems[i]) {
            s->elems[k] = s->elems[i];
            k++;
        }
    }

    s->size = k;
}

void stack__clear(const Stack s)
{
    if (!s) return;

    if (s->operator_delete) {
        for (size_t i = 0; i < s->size; i++) {
            s->operator_delete(s->elems[i]);
        }
    }

    free(s->elems);
    s->elems = malloc(sizeof(elem_t) * DEFAULT_STACK_CAPACITY);
    if (!s->elems) return;

    s->size = 0;
}

void stack__free(const Stack s)
{
    if (!s) return;

    if (s->operator_delete) {
        for (size_t i = 0; i < s->size; i++) {
            s->operator_delete(s->elems[i]);
        }
    }

    free(s->elems);
    free(s);
}

void stack__debug(const Stack s, void (*debug_op) (elem_t))
{
    setvbuf (stdout, NULL, _IONBF, 0);
    if (!s || !s->elems)
        printf("\tStack (NULL)\n");
    else {
        printf("\n\n");
        stack__is_copy_enabled(s) ? printf("\tStack with copy enabled:") : printf("\tStack with copy disabled:");
        printf("\n\tsize: %lu, \n\tcapacity: %lu, \n\tcontent: \n\t", s->size, s->capacity);
        printf("{ ");
        for (size_t i = 0; i < s->capacity; i++) {
            if (i < s->size) {
                debug_op(s->elems[i]);
            } else {
                printf("_ ");
            }
        }
        printf("} ");
        printf("\n\n");
    }
}
