#ifndef __VEC_H__
#define __VEC_H__

#define PTR_INCREMENT(__ptr, __size) \
    (__ptr) = (void *)((size_t)(__ptr) + (__size))

#define SWAP(__ptr, __i, __j) \
    elem_t *__elems = (__ptr)->elems; \
    elem_t __temp = __elems[__i]; \
    __elems[__i] = __elems[__j]; \
    __elems[__j] = __temp

#define RESIZE(__ptr, __new_capacity) \
({ \
    int __result_res = FAILURE; \
    elem_t *__realloc_res = realloc((__ptr)->elems, sizeof(elem_t) * (__new_capacity)); \
    if (__realloc_res) { \
        (__ptr)->elems = __realloc_res; \
        (__ptr)->capacity = (__new_capacity); \
        __result_res = SUCCESS; \
    } \
    (char)__result_res; \
})

#define ENSURE_CAPACITY(__ptr) \
({ \
    int __result_ens = FAILURE; \
    size_t __capacity = (__ptr)->capacity; \
    if ((__ptr)->back == __capacity) { \
        size_t __offset = (__capacity < SIZE_MAX>>1) ? __capacity : SIZE_MAX - __capacity; \
        while (__offset && (__result_ens = RESIZE((__ptr), __capacity + __offset))) { \
            __offset = __offset>>1; \
        } \
    } else { \
        __result_ens = 0; \
    } \
    (char)__result_ens; \
})

#define FROM_ARRAY(__ptr, __array, __n_elems, __size) \
    for (size_t i = 0; i < __n_elems; i++) { \
        (__ptr)->elems[(__ptr)->back + i] = (__ptr)->operator_copy(A); \
        PTR_INCREMENT(__array, __size); \
    } \
    (__ptr)->back += n_elems; \
    (__ptr)->length += n_elems

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
    (__dst)->length = (__src)->length; \
})

#define PTR_CONTAINS(__ptr, __start, __end, __elem) \
({ \
    elem_t *__elems = (__ptr)->elems; \
    int __result_ptr_contains = false; \
    for (size_t i = (__start); i < (__end) && !__result_ptr_contains; i++) { \
        __result_ptr_contains |= (__elems[i] == (__elem)); \
    } \
    (char)__result_ptr_contains; \
})

#define ARRAY_CMP(__ptr_1, __ptr_2, __cmp, __n_elems) \
({ \
    int __result_cmp = true; \
    for (size_t i = 0; i < (__n_elems) && __result_cmp; i++) { \
        __result_cmp &= !(__cmp)((__ptr_1) + i, (__ptr_2) + i); \
    } \
    (char)__result_cmp; \
})

#define FOREACH(__ptr, __func, __user_data, __start, __end) do { \
    elem_t *__elems = (__ptr)->elems; \
    char __repeated; \
    if ((__ptr)->copy_enabled) { \
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

#define FILTER(__ptr, __start, __end, __pred, __user_data) \
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
    (__ptr)->length = k

#define ALL(__ptr, __start, __end, __pred, __user_data) \
({ \
    elem_t *__elems = (__ptr)->elems; \
    int __result_all = true; \
    for (size_t i = (__start); i < (__end); i++) { \
        __result_all &= (__pred)(__elems[i], (__user_data)); \
    } \
    (char)__result_all; \
})

#define ANY(__ptr, __start, __end, __pred, __user_data) \
({ \
    elem_t *__elems = (__ptr)->elems; \
    int __result_any = false; \
    for (size_t i = (__start); i < (__end) && !__result_any; i++) { \
        __result_any |= (__pred)(__elems[i], (__user_data)); \
    } \
    (char)__result_any; \
})

#define SHUFFLE(__ptr, __start, __end, __seed) do { \
    size_t __i, __j; \
    srand(__seed); \
    for (size_t i = (__start); i < (__end); i++) { \
        __i = ((__start) + (size_t)(rand() % (int)((__start) + (__end)))); \
        __j = ((__start) + (size_t)(rand() % (int)((__start) + (__end)))); \
        SWAP(__ptr, __i, __j); \
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
    (__ptr)->length = k

#define FREE_ELEMS(__ptr, __start, __end) do { \
    elem_t *__elems = (__ptr)->elems; \
    if ((__ptr)->copy_enabled) { \
        for (size_t i = (__start); i < (__end); i++) { \
            (__ptr)->operator_delete(__elems[i]); \
        } \
    } \
    (__ptr)->back = 0; \
    (__ptr)->length = 0; \
} while (false)

#endif
