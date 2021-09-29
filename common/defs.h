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
    (__ptr) = (void *)((size_t)(__ptr) + (__size))

#define PTR_SWAP(__ptr_1, __ptr_2) \
    elem_t __temp = (__ptr_1); \
    (__ptr_1) = (__ptr_2); \
    (__ptr_2) = __temp

#define ARRAY_RESIZE(__ptr, __new_capacity) \
({ \
    char __result_res = FAILURE; \
    elem_t *__realloc_res = realloc((__ptr)->elems, sizeof(elem_t) * (__new_capacity)); \
    if (__realloc_res) { \
        (__ptr)->elems = __realloc_res; \
        (__ptr)->capacity = (__new_capacity); \
        __result_res = SUCCESS; \
    } \
    __result_res; \
})

#define ENSURE_CAPACITY(__ptr) \
({ \
    char __result_ens = FAILURE; \
    size_t __capacity = (__ptr)->capacity; \
    size_t __offset = (__capacity < LONG_MAX) ? __capacity \
                                              : ULONG_MAX - __capacity; \
    while (__offset && (__result_ens = ARRAY_RESIZE((__ptr), __capacity + __offset))) { \
        __offset = __offset>>1; \
    } \
    __result_ens; \
})

#define COPY(__dst, __src, __start, __n_elems) \
({ \
    if ((__src)->copy_enabled) { \
        (__dst)->copy_enabled = true; \
        for (size_t i = 0; i < (__n_elems); i++) { \
            (__dst)->elems[i] = (__src)->operator_copy((__src)->elems[(__start) + i]); \
        } \
    } else { \
        (__dst)->copy_enabled = false; \
        memcpy((__dst)->elems, (__src)->elems + (__start), sizeof(elem_t) * (__n_elems)); \
    } \
    (__dst)->size = (__src)->size; \
})

#define ARRAY_CMP(__ptr_1, __ptr_2, __cmp, __n_elems) \
({ \
    char res = true; \
    for (size_t i = 0; i < (__n_elems) && res; i++) { \
        res &= (char)!(__cmp)((__ptr_1) + i, (__ptr_2) + i); \
    } \
    res; \
})

#define ARRAY_FOREACH(__ptr, __func, __user_data, __start, __end, __copy_enabled) do { \
    elem_t *__elems = (__ptr); \
    char __repeated; \
    if (__copy_enabled) { \
        for (size_t i = (__start); i < (__end); i++) { \
            (__func)(__elems[i], (__user_data)); \
        } \
    } else { \
        __repeated = false; \
        for (size_t i = (__start); i < (__end); i++) { \
            for (size_t j = (__start); j < i && !__repeated; j++) { \
                if (__elems[i] == __elems[j]) { \
                    __repeated = true; \
                } \
            } \
            if (!__repeated) { \
                (__func)(__elems[i], (__user_data)); \
            } \
            __repeated = false; \
        } \
    } \
} while(false)

#define ARRAY_FILTER(__ptr, __start, __end, __pred, __user_data) \
    elem_t *__elems = (__ptr)->elems; \
    size_t k = 0; \
    for (size_t i = (__start); i < (__end); i++) { \
        if ((__pred)(__elems[i], (__user_data))) { \
            __elems[k] = __elems[i]; \
            k++; \
        } else if ((__ptr)->operator_delete) { \
            (__ptr)->operator_delete(__elems[i]); \
        } \
    } \
    (__ptr)->size = k

#define ARRAY_ALL(__ptr, __start, __end, __pred, __user_data) \
({ \
    elem_t *__elems = (__ptr); \
    char res = true; \
    for (size_t i = (__start); i < (__end); i++) { \
        res &= (__pred)(__elems[i], (__user_data)); \
    } \
    res; \
})

#define ARRAY_ANY(__ptr, __start, __end, __pred, __user_data) \
({ \
    elem_t *__elems = (__ptr); \
    char res = false; \
    for (size_t i = (__start); i < (__end) && !res; i++) { \
        res |= (__pred)(__elems[i], (__user_data)); \
    } \
    res; \
})

#define ARRAY_SHUFFLE(__ptr, __start, __end, __seed) do { \
    elem_t *__elems = (__ptr); \
    size_t __a, __b; \
    srand(__seed); \
    for (size_t i = (__start); i < (__end); i++) { \
        __a = ((__start) + (size_t)(rand() % (int)((__start) + (__end)))); \
        __b = ((__start) + (size_t)(rand() % (int)((__start) + (__end)))); \
        PTR_SWAP(__elems[__a], __elems[__b]); \
    } \
} while (false)

#define CLEAN_NULL_ELEMS(__ptr, __start, __end) \
    elem_t *__elems = (__ptr)->elems; \
    size_t k = 0; \
    for (size_t i = (__start); i < (__end); i++) { \
        if (__elems[i]) { \
            __elems[k] = __elems[i]; \
            k++; \
        } \
    } \
    (__ptr)->size = k

#define FREE_ELEMS(__ptr, __start, __end) do { \
    elem_t *__elems = (__ptr)->elems; \
    if ((__ptr)->copy_enabled) { \
        for (size_t i = (__start); i < (__end); i++) { \
            (__ptr)->operator_delete(__elems[i]); \
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
 * Function pointer for lambda applying
 */
typedef char (*filter_func_t)(elem_t, void *);

/**
 * Function pointer for element comparison
 */
typedef int (*compare_func_t)(const void *, const void *);

/**
 * Function pointer for element print
 */
typedef void (*debug_func_t)(elem_t);

#endif
