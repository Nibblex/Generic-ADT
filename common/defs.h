#ifndef __DEFS_H__
#define __DEFS_H__

#include <stdint.h>

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
typedef void (*applying_func_t)(const void *, void *);

/**
 * Function pointer for binary lambda applying
 */
typedef void *(*bin_applying_func_t)(const void *, const void *, void *);

/**
 * Function pointer for lambda applying
 */
typedef char (*filter_func_t)(const void *, void *);

/**
 * Function pointer for element comparison
 */
typedef int (*compare_func_t)(const void *, const void *);

/**
 * Function pointer for element print
 */
typedef void (*debug_func_t)(elem_t);

#endif
