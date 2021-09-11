#ifndef __DEFS_H__
#define __DEFS_H__

#include <limits.h>

#ifndef SUCCESS
#define SUCCESS 0
#endif
#ifndef FAILURE
#define FAILURE -1
#endif
#ifndef true
#define true 1
#endif
#ifndef false
#define false 0
#endif

#define PTR_INCREMENT(__ptr, __size) \
    __ptr = (void *)((size_t)__ptr + (__size))

#define PTR_SWAP(__ptr1, __ptr2) \
    elem_t __temp = __ptr1; \
    __ptr1 = __ptr2; \
    __ptr2 = __temp

#define ARRAY_RESIZE(__ptr, __new_capacity) \
({ \
    char __result_res = FAILURE; \
    elem_t *__realloc_res = realloc(__ptr->elems, sizeof(elem_t) * (__new_capacity)); \
    if (__realloc_res) { \
        __ptr->elems = __realloc_res; \
        __ptr->capacity = (__new_capacity); \
        __result_res = SUCCESS; \
    } \
    __result_res; \
})

#define ENSURE_CAPACITY(__ptr) \
({ \
    char __result_ens = FAILURE; \
    size_t __capacity = __ptr->capacity; \
    size_t __offset = (__capacity < LONG_MAX) ? __capacity \
                                              : ULONG_MAX - __capacity; \
    while (__offset && (__result_ens = ARRAY_RESIZE(__ptr, __capacity + __offset))) { \
        __offset = __offset>>1; \
    } \
    __result_ens; \
})

#define ARRAY_SHUFFLE(__ptr, __start, __end) do { \
    size_t __a, __b; \
    for (size_t i = (__start); i < (__end); i++) { \
        __a = ((__start) + (size_t)(rand() % (int)((__start) + (__end)))); \
        __b = ((__start) + (size_t)(rand() % (int)((__start) + (__end)))); \
        PTR_SWAP(__ptr[__a], __ptr[__b]); \
    } \
} while (false)

#define CLEAN_NULL_ELEMS(__ptr, __start, __end) \
    size_t k = 0; \
    for (size_t i = (__start); i < (__end); i++) { \
        if (__ptr->elems[i]) { \
            __ptr->elems[k] = __ptr->elems[i]; \
            k++; \
        } \
    } \
    __ptr->size = k

#define FREE_ELEMS(__ptr, __start, __end) do { \
    if (__ptr->copy_enabled) { \
        for (size_t i = (__start); i < (__end); i++) { \
            __ptr->operator_delete(__ptr->elems[i]); \
        } \
    } \
} while (false)

/**
 * Generical element type
 */
typedef void * elem_t;

/**
 * Function pointer required to copy an entity within the structure
 */
typedef elem_t (*copy_operator_t)(elem_t);

/**
 * Function pointer required to delete an entity within the structure
 */
typedef void (*delete_operator_t)(elem_t);

/**
 * Function pointer for lambda applying
 */
typedef void (*applying_func_t)(elem_t, void *);

/**
 * Function pointer for element comparison
 */
typedef int (*compare_func_t)(const void *, const void *);

#endif
