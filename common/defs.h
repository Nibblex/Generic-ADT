#ifndef __DEFS_H__
#define __DEFS_H__

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

/**
 * Macro for pointer swap
 */
#define PTR_SWAP(a, b) \
    do { \
        elem_t __temp = (a); \
        (a) = (b); \
        (b) = __temp; \
    } while (false)

/**
 * Macro for pointer increment by __size bytes
 */
#define PTR_INCREMENT(p, __size) \
    do { \
        size_t __temp; \
        __temp = (long unsigned int)(p); \
        __temp += (__size); \
        (p) = (void *)__temp; \
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
