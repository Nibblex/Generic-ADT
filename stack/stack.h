#ifndef __STACK_H__
#define __STACK_H__

#include "../common/defs.h"


/**
 * Implementation of a FILO Abstract Data Type
 *
 * Notes :
 * 1) You have to correctly implement copy, delete and debug operators
 * by handling NULL value, otherwise you can end up with an undefined behaviour.
 * The prototypes of these functions are:
 * elem_t (*copy_op)(elem_t)
 * void (*delete_op)(elem_t)
 * void (*debug_op)(elem_t)
 *
 * 2) 'stack__top' and 'stack__pop' return a dynamically allocated pointer to an element in the
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
 * @brief Adds an element in the stack
 * @note Complexity: O(1)
 * @param s the stack
 * @param element the element to add
 * @return 0 on success, 1 otherwise
 */
char stack__push(const Stack s, const elem_t element);


/**
 * @brief Retrieve a copy of the top element (similar to 'stack__top' but the element is removed of the stack)
 * @details The element is stored in 'top' variable and must be manually freed by user afterward
 * @note Complexity: O(1)
 * @param s the stack
 * @param top pointer to storage variable
 * @return 0 on success, 1 otherwise
 */
char stack__pop(const Stack s, elem_t *top);


/**
 * @brief Retrieve the element on the top of the stack without removing it
 * @details The element is stored in 'top' variable and must be manually freed by user afterward
 * @note Complexity: O(1)
 * @param s the stack
 * @param top pointer to storage variable
 * @return 0 on success, 1 otherwise
 */
char stack__top(const Stack s, elem_t *top);


/**
 * @brief Verify if the element's copy is enabled for the given stack
 * @note Complexity: O(1)
 * @param s the stack
 * @return 1 if the stack has copy enabled, 0 otherwise
 */
char stack__is_copy_enabled(const Stack s);


/**
 * @brief Verify if the stack is empty
 * @note Complexity: O(1)
 * @param s the stack
 * @return 1 if the stack is empty, 0 otherwise
 */
char stack__is_empty(const Stack s);


/**
 * @brief Number of elements in the stack
 * @note Complexity: O(1)
 * @param s the stack
 * @return an unsigned integer corresponding to the number of elements in the stack
 */
size_t stack__size(const Stack s);


/**
 * @brief Adds the first 'n_elems' elements of the given array in the stack
 * @details If the stack already has elements they are kept in that stack
 * @note Complexity: O(n)
 * @param s the stack, if q == NULL creates a new one with copy disabled by default
 * @param A the array, if A == NULL returns the stack unaltered
 * @param n_elems number of elements to push, must be less than the array length
 * @param type type of array elements, available types: 'CHAR', 'INT', 'UINT', 'FLOAT', 'STRING', 'GENERIC'
 * @return a stack containing all the elements of the array, NULL on error
 */
Stack stack__from_array(Stack s, const elem_t *A, const size_t n_elems, const DataType type);


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
 * @brief Randomly mixes the stack
 * @note Complexity: O(n)
 * @param s the stack
 */
void stack__shuffle(const Stack s);


/**
 * @brief Maps the given function to the stack
 * @note Complexity: O(n) with copy enabled, O(n²) with copy disabled
 * @param s the stack
 * @param f the applying function
 * @param user_data optional data to be used as an additional argument of the application function
 */
void stack__foreach(const Stack s, const applying_func_t f, void *user_data);


/**
 * @brief Removes all NULL pointers in the stack
 * @note Complexity: O(n)
 * @param s the stack
 */
void stack__clean_NULL(const Stack s);


/**
 * @brief Removes all elements in the stack
 * @details If copy is enabled frees all allocated memory used by these elements, the stack is still usable afterwards
 * @note Complexity: O(n) with copy enabled, O(1) with copy disabled
 * @param s the stack
 */
void stack__clear(const Stack s);


/**
 * @brief Frees all allocated memory used by the stack
 * @details If copy is enabled frees all memory used by the elements in the stack
 * @note Complexity: O(n) with copy enabled, O(1) with copy disabled
 * @param s the stack
 */
void stack__free(const Stack s);


/**
 * @brief Prints the stack's content
 * @note Complexity: O(n)
 * @param s the stack
 * @param debug_op debug operator
 */
void stack__debug(const Stack s, void (*debug_op) (elem_t));


#endif
