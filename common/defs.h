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

#define SWAP(x, y) \
    do { \
        unsigned char __temp[sizeof(x) == sizeof(y) ? (signed)sizeof(x) : -1]; \
        memcpy(__temp, &y, sizeof(x)); \
        memcpy(&y, &x, sizeof(x)); \
        memcpy(&x, __temp, sizeof(x)); \
    } while (false)

#define INC_POINTER(p, __size) \
    do { \
        unsigned long int __temp; \
        memcpy(&__temp, &p, sizeof(unsigned long int)); \
        __temp += __size; \
        memcpy(&p, &__temp, sizeof(unsigned long int)); \
    } while (false)

/**
 * Generical element type
 */
typedef void * elem_t;

/**
 * Data types definition used in some functions
 */
typedef enum {CHAR = 0, INT, UINT, FLOAT, STRING, GENERIC} DataType;

/**
 * Function pointer for lambda applying
 */
typedef elem_t (*copy_operator_t)(elem_t);

/**
 * Function pointer for lambda applying
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
