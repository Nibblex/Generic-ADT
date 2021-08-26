#ifndef __QUEUE_H__
#define __QUEUE_H__

#include "../common/defs.h"


/**
 * Implementation of a FIFO Abstract Data Type
 *
 * Notes :
 * 1) You have to correctly implement copy, delete and debug operators
 * by handling NULL value, otherwise you can end up with an undefined behaviour.
 * The prototypes of these functions are :
 * elem_t (*copy_op)(elem_t)
 * void (*delete_op)(elem_t)
 * void (*debug_op)(elem_t)
 *
 * 2) 'queue__front', 'queue__back' and 'queue__pop' return a dynamically allocated pointer to an element of
 * the queue in order to make it survive independently of the queue life cycle.
 * The user has to manually free the return pointer after usage.
 */
typedef struct QueueSt * Queue;


/**
 * @brief Create an empty queue with copy enabled
 * @note Complexity: O(1)
 * @param copy_op copy operator
 * @param delete_op delete operator
 * @return a newly created queue, NULL on error
 */
Queue queue__empty_copy_enabled(const copy_operator_t copy_op, const delete_operator_t delete_op);


/**
 * @brief Create an empty queue with copy disabled
 * @note Complexity: O(1)
 * @return a newly created queue, NULL on error
 */
Queue queue__empty_copy_disabled(void);


/**
 * @brief Adds an element in the queue
 * @note Complexity: O(1)
 * @param q the queue
 * @param element the element to add
 * @return 0 on success, 1 otherwise
 */
char queue__enqueue(const Queue q, const elem_t element);


/**
 * @brief Removes the front element of the queue
 * @note Complexity: O(1)
 * @param q the queue
 * @return 0 on success, 1 otherwise
 */
char queue__dequeue(const Queue q);


/**
 * @brief Retrieve a copy of the front element (similar to 'queue__front' but the element is removed of the queue)
 * @details The element is stored in 'front' variable and must be manually freed by user afterward
 * @note Complexity: O(1)
 * @param q the queue
 * @param front pointer to storage variable
 * @return 0 on success, 1 otherwise
 */
char queue__pop(const Queue q, elem_t *front);


/**
 * @brief Retrieve the element on the front of the queue without removing it
 * @details The element is stored in 'front' variable and must be manually freed by user afterward
 * @note Complexity: O(1)
 * @param q the queue
 * @param front pointer to storage variable
 * @return 0 on success, 1 otherwise
 */
char queue__front(const Queue q, elem_t *front);


/**
 * @brief Retrieve the element on the back of the queue without removing it
 * @details The element is stored in 'back' variable and must be manually freed by user afterward
 * @note Complexity: O(1)
 * @param q the queue
 * @param back pointer to storage variable
 * @return 0 on success, 1 otherwise
 */
char queue__back(const Queue q, elem_t *back);


/**
 * @brief Verify if the element's copy is enabled for the given queue
 * @note Complexity: O(1)
 * @param q the queue
 * @return 1 if the queue has copy enabled, 0 otherwise
 */
char queue__is_copy_enabled(const Queue q);


/**
 * @brief Verify if the queue is empty
 * @note Complexity: O(1)
 * @param q the queue
 * @return 1 if the queue is empty, 0 otherwise
 */
char queue__is_empty(const Queue q);


/**
 * @brief Number of elements in the queue
 * @note Complexity: O(1)
 * @param q the queue
 * @return an unsigned integer corresponding to the number of elements in the queue
 */
size_t queue__size(const Queue q);


/**
 * @brief Adds the first 'n_elems' elements of the given array in the queue
 * @details If the queue already has elements they are kept in that queue
 * @note Complexity: O(n)
 * @param q the queue, if q == NULL creates a new one with copy disabled by default
 * @param A the array, if A == NULL returns the queue unaltered
 * @param n_elems number of elements to enqueue, must be less than the array length
 * @param type type of array elements, available types: 'CHAR', 'INT', 'UINT', 'FLOAT', 'STRING', 'GENERIC'
 * @return a queue containing all the elements of the array, NULL on error
 */
Queue queue__from_array(Queue q, void *A, const size_t n_elems, const size_t size);


/**
 * @brief Retrieves a copy of all items in a queue stored in array
 * @details The array must be manually freed by user afterward
 * @note Complexity: O(n)
 * @param q the queue
 * @return a dynamically allocated array which contains all the elements from the given queue, NULL on error
 */
elem_t *queue__to_array(const Queue q);


/**
 * @brief Uses qsort to sort the queue using the comparison criteria given by the received compare function
 * @note Complexity: O(n*log(n))
 * @param q the queue
 * @param f the compare function
 */
void queue__sort(const Queue q, const compare_func_t f);


/**
 * @brief Randomly mixes the queue
 * @note Complexity: O(n)
 * @param q the queue
 * */
void queue__shuffle(const Queue q);


/**
 * @brief Maps the given function to the queue
 * @note Complexity: O(n) with copy enabled, O(n²) with copy disabled
 * @param q the queue
 * @param f the applying function
 * @param user_data optional data to be used as an additional argument of the application function
 */
void queue__foreach(const Queue q, const applying_func_t f, void *user_data);


/**
 * @brief Removes all NULL pointers in the queue
 * @note Complexity: O(n)
 * @param q the queue
 */
void queue__clean_NULL(const Queue q);


/**
 * @brief Removes all elements in the queue
 * @details If copy is enabled frees all allocated memory used by these elements, the queue is still usable afterwards
 * @note Complexity: O(n) with copy enabled, O(1) with copy disabled
 * @param q the queue
 */
void queue__clear(const Queue q);


/**
 * @brief Frees all allocated memory used by the queue
 * @details If copy is enabled frees all memory used by the elements in the queue
 * @note Complexity: O(n) with copy enabled, O(1) with copy disabled
 * @param q the queue
 */
void queue__free(const Queue q);


/**
 * @brief Prints the queue's content
 * @note Complexity: O(n)
 * @param q the queue
 * @param debug_op debug operator
 */
void queue__debug(const Queue q, void (*debug_op) (elem_t));


#endif
