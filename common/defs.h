#ifndef __DEFS_H__
#define __DEFS_H__

#ifndef SUCCESS
#define SUCCESS 0
#endif
#ifndef FAILURE
#define FAILURE 1
#endif
#ifndef true
#define true 1
#endif
#ifndef false
#define false 0
#endif

/**
 * generical element type
 */
typedef void * elem_t;

/**
 * data types used in 'queue__from_array' function as a parameter
 */
typedef enum  { CHAR = 0, INT, FLOAT, STRING, GENERAL} DataType;

/**
 * function pointer for lambda applying
 */
typedef elem_t (*copy_operator_t)(elem_t);

/**
 * function pointer for lambda applying
 */
typedef void (*delete_operator_t)(elem_t);

/**
 * function pointer for lambda applying
 */
typedef void (*applying_func_t)(elem_t, void *);

/**
 * function pointer for element comparison
 */
typedef int (*compare_func_t)(const void *, const void *);

#endif
