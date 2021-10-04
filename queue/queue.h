#ifndef __QUEUE_H__
#define __QUEUE_H__

#include "../common/defs.h"


/**
 * Implementation of a FIFO Abstract Data Type
 *
 * Notes :
 * 1) You have to correctly implement copy, delete and debug operators
 * by handling NULL value, otherwise you can end up with an undefined behaviour.
 * The prototypes of these functions are:
 * elem_t (*copy_op)(elem_t)
 * void (*delete_op)(elem_t)
 * void (*debug_op)(elem_t)
 *
 * 2) 'queue__peek_front', 'queue__peek_back', 'queue_peek_nth' and 'queue__dequeue' return a dynamically allocated pointer to an element of
 * the queue in order to make it survive independently of the queue life cycle.
 * The user has to manually free the return pointer after usage.
 */
typedef struct QueueSt * Queue;


/**
 * @brief create an empty queue with copy disabled
 * @note complexity: O(1)
 * @return a pointer to queue on success, NULL on failure
 */
Queue queue__empty_copy_disabled(void);


/**
 * @brief create an empty queue with copy enabled
 * @note complexity: O(1)
 * @param copy_op copy operator
 * @param delete_op delete operator
 * @return a pointer to queue on success, NULL on failure
 */
Queue queue__empty_copy_enabled(const copy_operator_t copy_op, const delete_operator_t delete_op);


/**
 * @brief checks if the queue has the copy operator enabled
 * @note complexity: O(1)
 * @param q the queue
 * @return 1 if the queue has copy enabled, 0 if not, -1 on failure
 */
char queue__is_copy_enabled(const Queue q);


/**
 * @brief checks if the queue is empty
 * @note complexity: O(1)
 * @param q the queue
 * @return 1 if the queue is empty, 0 if not, -1 on failure
 */
char queue__is_empty(const Queue q);


/**
 * @brief number of elements in the queue
 * @note complexity: O(1)
 * @param q the queue
 * @return  the number of elements contained in the queue on success, -1 on failure
 */
size_t queue__length(const Queue q);


/**
 * @brief adds an element in the queue
 * @note complexity: O(1)
 * @param q the queue
 * @param element the element to add
 * @return 0 on success, -1 on failure
 */
char queue__enqueue(const Queue q, const elem_t element);


/**
 * @brief retrieve a copy of the front element (similar to 'queue__peek_front' but the element is removed of the queue)
 * @details the element is stored in 'front' variable and must be manually freed by user afterward
 * @note complexity: O(1)
 * @param q the queue
 * @param front pointer to storage variable
 * @return 0 on success, -1 on failure
 */
char queue__dequeue(const Queue q, elem_t *front);


/**
 * @brief retrieve the element on the front of the queue without removing it
 * @details the element is stored in 'front' variable and must be manually freed by user afterward
 * @note complexity: O(1)
 * @param q the queue
 * @param front pointer to storage variable
 * @return 0 on success, -1 on failure
 */
char queue__peek_front(const Queue q, elem_t *front);


/**
 * @brief retrieve the element on the back of the queue without removing it
 * @details the element is stored in 'back' variable and must be manually freed by user afterward
 * @note complexity: O(1)
 * @param q the queue
 * @param back pointer to storage variable
 * @return 0 on success, -1 on failure
 */
char queue__peek_back(const Queue q, elem_t *back);


/**
 * @brief Retrieve the element at 'i' position of the queue without removing it
 * @details The element is stored in 'nth' variable and must be manually freed by user afterward
 * @note complexity: O(1)
 * @param q the queue
 * @param i position
 * @param nth pointer to storage variable
 * @return 0 on success, -1 on failure
 */
char queue__peek_nth(const Queue q, const size_t i, elem_t *nth);


/**
 * @brief swaps two elements of the queue
 * @note complexity: O(1)
 * @param q the queue
 * @param i position of the first element
 * @param j position of the second element
 * @return 0 on success, -1 on failure
 */
char queue__swap(const Queue q, size_t i, size_t j);


/**
 * @brief retrieves a copy of the entire queue
 * @details if copy is enabled the new one contains a copy of all elements of the original queue
 * @note complexity: O(n)
 * @param q the queue
 * @return a pointer to queue on success, NULL on failure
 */
Queue queue__copy(const Queue q);


/**
 * @brief enqueues the first 'n_elems' elements of the given array
 * @details if q == NULL creates a new queue with copy disabled by default
 * @details if A == NULL returns the queue unaltered
 * @note complexity: O(n)
 * @param q the queue
 * @param A the array
 * @param n_elems number of elements to push, must be less than or equal to the length of the array
 * @param size byte size of the elements contained in the given array
 * @return a pointer to queue on success, NULL on failure
 */
Queue queue__from_array(Queue q, void *A, const size_t n_elems, const size_t size);


/**
 * @brief dump all elements of the queue into an array
 * @details the array must be manually freed by user afterward, the queue is empty after use of this function
 * @note complexity: O(n)
 * @param q the queue
 * @return a pointer to dynamically allocated array on success, NULL on failure
 */
elem_t *queue__dump(const Queue q);


/**
 * @brief retrieves a copy of all items in a queue stored in array
 * @details the array must be manually freed by user afterward
 * @note complexity: O(n)
 * @param q the queue
 * @return a pointer to dynamically allocated array on success, NULL on failure
 */
elem_t *queue__to_array(const Queue q);


/**
 * @brief check if a pointer is on the queue
 * @note complexity: O(n)
 * @param q the queue
 * @param elem the pointer
 * @return 1 if the pointer is on the queue, 0 if not, -1 on failure
 */
char queue__ptr_contains(const Queue q, const elem_t elem);


/**
 * @brief compare two queues including all their elements
 * @note complexity: O(n)
 * @param q first queue
 * @param w second queue
 * @param cmp the compare function
 * @return 1 if the queues are equal including all their elements, 0 if not, -1 on failure
 */
char queue__cmp(const Queue q, const Queue w, const compare_func_t cmp);


/**
 * @brief verifies that all elements of the queue satisfy the predicate
 * @note complexity: O(n)
 * @param q the queue
 * @param pred the predicate
 * @param user_data optional data to be used as an additional argument of the predicate
 * @return 1 if all elements satisfy the predicate, 0 if not, -1 on failure
 */
char queue__all(const Queue q, const filter_func_t pred, void *user_data);


/**
 * @brief verifies that at least one element of the queue satisfies the predicate
 * @note complexity: O(n)
 * @param q the queue
 * @param pred the predicate
 * @param user_data optional data to be used as an additional argument of the predicate
 * @return 1 if any element satisfies the predicate , 0 if not, -1 on failure
 */
char queue__any(const Queue q, const filter_func_t pred, void *user_data);


/**
 * @brief maps the given function to the queue
 * @note complexity: O(n) with copy enabled, O(n²) with copy disabled
 * @param q the queue
 * @param func the applying function
 * @param user_data optional data to be used as an additional argument of the application function
 */
void queue__foreach(const Queue q, const applying_func_t func, void *user_data);


/**
 * @brief filter the given queue using a predicate
 * @note complexity: O(n)
 * @param q the queue
 * @param pred the predicate
 * @param user_data optional data to be used as an additional argument of the predicate
 */
void queue__filter(const Queue q, const filter_func_t pred, void *user_data);


/**
 * @brief reverse the queue
 * @note complexity: O(n)
 * @param q the queue
 */
void queue__reverse(const Queue q);


/**
 * @brief shuffles the queue
 * @note complexity: O(n)
 * @param q the queue
 * */
void queue__shuffle(const Queue q, const unsigned int seed);


/**
 * @brief uses qsort to sort the queue elements using the given compare function
 * @note complexity: O(n*log(n))
 * @param q the queue
 * @param cmp the compare function
 */
void queue__sort(const Queue q, const compare_func_t cmp);


/**
 * @brief removes all NULL pointers in the queue
 * @note complexity: O(n)
 * @param q the queue
 */
void queue__clean_NULL(const Queue q);


/**
 * @brief removes all elements in the queue
 * @details if copy is enabled frees all allocated memory used by these elements, the queue is still usable afterwards
 * @note complexity: O(n) with copy enabled, O(1) with copy disabled
 * @param q the queue
 */
void queue__clear(const Queue q);


/**
 * @brief frees all allocated memory used by the queue
 * @details if copy is enabled frees all memory used by the elements in the queue
 * @note complexity: O(n) with copy enabled, O(1) with copy disabled
 * @param q the queue
 */
void queue__free(const Queue q);


/**
 * @brief prints the queue's content
 * @note complexity: O(n)
 * @param q the queue
 * @param debug the debug function
 */
void queue__debug(const Queue q, const debug_func_t debug);


#endif
