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
 * 2) 'stack__peek_top' and 'stack__pop' return a dynamically allocated pointer to an element in the
 * the stack in order to make it survive independently of the stack life cycle.
 * The user has to manually free the return pointer after usage.
 */
typedef struct StackSt * Stack;


/**
 * @brief create an empty stack with copy disabled
 * @note complexity: O(1)
 * @return a pointer to stack on success, NULL on failure
 */
Stack stack__empty_copy_disabled(void);


/**
 * @brief create an empty stack with copy enabled
 * @note complexity: O(1)
 * @param copy_op copy operator
 * @param delete_op delete operator
 * @return a pointer to stack on success, NULL on failure
 */
Stack stack__empty_copy_enabled(const copy_operator_t copy_op, const delete_operator_t delete_op);


/**
 * @brief checks if the stack has the copy operator enabled
 * @note complexity: O(1)
 * @param s the stack
 * @return 1 if the stack has copy enabled, 0 if not, -1 on failure
 */
char stack__is_copy_enabled(const Stack s);


/**
 * @brief checks if the stack is empty
 * @note complexity: O(1)
 * @param s the stack
 * @return 1 if the stack is empty, 0 if not, -1 on failure
 */
char stack__is_empty(const Stack s);


/**
 * @brief number of elements in the stack
 * @note complexity: O(1)
 * @param s the stack
 * @return the number of elements contained in the stack on success, -1 on failure
 */
size_t stack__size(const Stack s);


/**
 * @brief adds an element in the stack
 * @note complexity: O(1)
 * @param s the stack
 * @param element the element to add
 * @return 0 on success, -1 on failure
 */
char stack__push(const Stack s, const elem_t element);


/**
 * @brief retrieve a copy of the top element (similar to 'stack__peek_top' but the element is removed of the stack)
 * @details the element is stored in 'top' variable and must be manually freed by user afterward
 * @note complexity: O(1)
 * @param s the stack
 * @param top pointer to storage variable
 * @return 0 on success, -1 on failure
 */
char stack__pop(const Stack s, elem_t *top);


/**
 * @brief retrieve the element on the top of the stack without removing it
 * @details the element is stored in 'top' variable and must be manually freed by user afterward
 * @note complexity: O(1)
 * @param s the stack
 * @param top pointer to storage variable
 * @return 0 on success, -1 on failure
 */
char stack__peek_top(const Stack s, elem_t *top);


/**
 * @brief retrieve the element at 'i' position of the stack without removing it
 * @details the element is stored in 'nth' variable and must be manually freed by user afterward
 * @note complexity: O(1)
 * @param s the stack
 * @param nth pointer to storage variable
 * @param i position
 * @return 0 on success, -1 on failure
 */
char stack__peek_nth(const Stack s, elem_t *nth, const size_t i);


/**
 * @brief swaps two elements of the stack
 * @note complexity: O(1)
 * @param s the stack
 * @param i position of the first element
 * @param j position of the second element
 * @return 0 on success, -1 on failure
 */
char stack__swap(const Stack s, size_t i, size_t j);


/**
 * @brief pushes the first 'n_elems' elements of the given array
 * @details if s == NULL creates a new stack with copy disabled by default
 * @details if A == NULL returns the stack unaltered
 * @note complexity: O(n)
 * @param s the stack
 * @param A the array
 * @param n_elems number of elements to push, must be less than or equal to the length of the array
 * @param size byte size of the elements contained in the given array
 * @return a pointer to stack on success, NULL on failure
 */
Stack stack__from_array(Stack s, void *A, const size_t n_elems, const size_t size);


/**
 * @brief dump all elements of the stack into an array
 * @details the array must be manually freed by user afterward, the stack is empty after use of this function
 * @note complexity: O(n)
 * @param s the stack
 * @return a pointer to dynamically allocated array on success, NULL on failure
 */
elem_t *stack__dump(const Stack s);


/**
 * @brief retrieves a copy of all elements of the stack into an array
 * @details the array must be manually freed by user afterward
 * @note complexity: O(n)
 * @param s the stack
 * @return a pointer to dynamically allocated array on success, NULL on failure
 */
elem_t *stack__to_array(const Stack s);


/**
 * @brief retrieves a copy of the entire stack
 * @details if copy is enabled the new one contains a copy of all elements of the original stack
 * @note complexity: O(n)
 * @param q the stack
 * @return a pointer to stack on success, NULL on failure
 */
Stack stack__copy(const Stack s);


/**
 * @brief compare two stacks including all their elements
 * @note complexity: O(n)
 * @param s first Stack
 * @param t second Stack
 * @param cmp the compare function
 * @return 1 if the stacks are equal including all their elements, 0 if not, -1 on failure
 */
char stack__cmp(const Stack s, const Stack t, compare_func_t cmp);


/**
 * @brief maps the given function to the stack
 * @note complexity: O(n) with copy enabled, O(n²) with copy disabled
 * @param s the stack
 * @param func the applying function
 * @param user_data optional data to be used as an additional argument of the application function
 */
void stack__foreach(const Stack s, const applying_func_t func, void *user_data);


/**
 * @brief filter the given stack using a predicate
 * @note complexity: O(n)
 * @param s the stack
 * @param pred the predicate
 * @param user_data optional data to be used as an additional argument of the predicate
 */
void stack__filter(const Stack s, const filter_func_t pred, void *user_data);


/**
 * @brief verifies that all elements of the stack satisfy the predicate
 * @note complexity: O(n)
 * @param s the stack
 * @param pred the predicate
 * @param user_data optional data to be used as an additional argument of the predicate
 * @return 1 if all elements satisfy the predicate, 0 if not, -1 on failure
 */
char stack__all(const Stack s, const filter_func_t pred, void *user_data);


/**
 * @brief verifies that at least one element of the stack satisfies the predicate
 * @note complexity: O(n)
 * @param s the stack
 * @param pred the predicate
 * @param user_data optional data to be used as an additional argument of the predicate
 * @return 1 if any element satisfies the predicate , 0 if not, -1 on failure
 */
char stack__any(const Stack s, const filter_func_t pred, void *user_data);


/**
 * @brief reverse the stack
 * @note complexity: O(n)
 * @param s the stack
 */
void stack__reverse(const Stack s);


/**
 * @brief shuffles the stack
 * @note complexity: O(n)
 * @param s the stack
 */
void stack__shuffle(const Stack s);


/**
 * @brief uses qsort to sort the stack elements using the given compare function
 * @note complexity: O(n*log(n))
 * @param s the stack
 * @param cmp the compare function
 */
void stack__sort(const Stack s, const compare_func_t cmp);


/**
 * @brief removes all NULL pointers in the stack
 * @note complexity: O(n)
 * @param s the stack
 */
void stack__clean_NULL(const Stack s);


/**
 * @brief removes all elements in the stack
 * @details if copy is enabled frees all allocated memory used by these elements, the stack is still usable afterwards
 * @note complexity: O(n) with copy enabled, O(1) with copy disabled
 * @param s the stack
 */
void stack__clear(const Stack s);


/**
 * @brief frees all allocated memory used by the stack
 * @details if copy is enabled frees all memory used by the elements in the stack
 * @note complexity: O(n) with copy enabled, O(1) with copy disabled
 * @param s the stack
 */
void stack__free(const Stack s);


/**
 * @brief prints the stack's content
 * @note complexity: O(n)
 * @param s the stack
 * @param debug the debug function
 */
void stack__debug(const Stack s, const debug_func_t debug);


#endif
