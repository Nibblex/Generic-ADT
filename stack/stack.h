#ifndef __STACK_H__
#define __STACK_H__

#include "../common/defs.h"

/**
 * Implementation of a FILO Abstract Data Type
 *
 * Notes :
 * 1) You have to correctly implement copy, delete and debug operators
 * by handling NULL value, otherwise you can end up with an undefined behaviour.
 * The prototypes of these functions are :
 * elem_t (*copy_op)(elem_t)
 * void (*delete_op)(elem_t)
 * void (*debug_op)(void *)
 *
 * 2) 'stack_peek' and 'stack_pop' return a dynamically allocated pointer to the head element of
 * the stack in order to make it survive independently of the stack life cycle.
 * The user has to manually free the return pointer after usage.
 */
typedef struct StackSt * Stack;


/**
 * @brief Create an empty stack with copy enabled
 * @note Complexity: O(1)
 * @param copy_op copy operator
 * @param delete_op delete operator
 * @return a newly created stack, NULL on error
 */
Stack stack__empty_copy_enabled(const copy_operator_t copy_op, const delete_operator_t delete_op);


/**
 * @brief Create an empty stack with copy disabled
 * @note Complexity: O(1)
 * @return a newly created stack, NULL on error
 */
Stack stack__empty_copy_disabled(void);

  
/**
 * @brief Push an element in the stack
 * @note Complexity: O(1) (O(n) is stack's length is a power of 2)
 * @param s the stack
 * @param element the element to add
 * @return 0 on success, 1 otherwise
 */
char stack__push(const Stack s, const elem_t element);


/**
 * @brief Pop an element out of the stack
 * @note Complexity: O(1) (O(n) is stack's length is a power of 2)
 * @param s the stack
 * @return an enumeration representing the popped element
 */
char stack__pop(const Stack s, elem_t *top);


/**
 * @brief Retrieve the element on the top of the stack without removing it

 * @note Complexity: O(1)
 * @param s the stack
 * @return an enumeration representing the element on top
 */
char stack__peek(const Stack s, elem_t *top);


/**
 * @brief Verify if the element's copy is enabled for the given stack
 * @note Complexity: O(1)
 * @param s is the stack you want to check
 * @return 1 if copy is enabled, 0 if not
 */
char stack__is_copy_enabled(const Stack s);


/**
 * @brief Verify if the stack is empty
 * @note Complexity: O(1)
 * @param s the stack
 * @return 1 if empty
 * @return 0 otherwise
 */
char stack__is_empty(const Stack s);


/**
 * @brief Count the number of elements in the stack
 * @note Complexity: O(1)
 * @param s the stack
 * @return an integer corresponding to the number of elements in the stack
 */
size_t stack__size(const Stack s);


/**
 * @brief Pushes the first 'n_elems' elements of the given array in the given stack
 * @details If the stack already has elements they are kept in that stack
 * @note Complexity: O(n)
 * @param s the stack, if q == NULL creates a new one with copy disabled by default
 * @param A the array, if A == NULL returns the stack received unaltered
 * @param n_elems number of elements to push, must be less than the array length
 * @param type type of array elements, available types: 'CHAR', 'INT', 'FLOAT', 'STRING', 'GENERAL'
 * @return a stack containing all the elements of the array, NULL on error
 */
Stack stack__from_array(Stack s, void *A, const size_t n_elems, const DataType type);


/**
 * @brief Retrieves a copy of all items in a stack stored in array
 * @details The array must be manually freed by user afterward
 * @note Complexity: O(n)
 * @param s the stack
 * @return a dynamically allocated array which contains all the elements from the given stack, NULL on error
 */
elem_t *stack__to_array(const Stack s);


/**
 * @brief Uses qsort to sort the stack using the comparison criteria given by the received compare function
 * @note Complexity: O(n*log(n))
 * @param s the stack
 * @param f the compare function
 */
void stack__sort(const Stack s, const compare_func_t f);


/**
 * @brief Randomly mixes the stack's content
 * @note Complexity: O(n)
 * @param s the stack
 * */
void stack__mix(const Stack s);


/**
 * @brief Applies a function with only a stack object as parameter
 * @note Complexity: O(n) if copy enabled, O(n¹) if not 
 * @param s the stack
 * @param f the applying function
 */
void stack__foreach(const Stack s, const applying_func_t f, void *user_data);


/**
 * @brief Remove all NULL from the stack
 * @note Complexity: O(n²)
 * @param s is the stack you want to clean
 * @return 1 if the stack has been cleaned up, 0 else
 */
void stack__clean_NULL(const Stack s);


/**
 * @brief Empty the stack, if copy is enabled frees all allocated memory used by the elements in the stack, the stack is still usable afterwards
 * @note Complexity: O(n) with copy enabled, O(1) with copy disabled
 * @param s the stack to clear
 */
void stack__clear(const Stack s);


/**
 * @brief Free all allocated memory used by the stack
 * @note Complexity: O(n)
 * @param s the stack to free
 */
void stack__free(const Stack s);


/**
 * @brief Prints the stack's content
 * @note Complexity: O(n)
 * @param s the stack to debug
 * @param is_compact to display a compact debug (only values)
 */
void stack__debug(const Stack s, void (*debug_op) (elem_t));

#endif
