/**
 * @file queue.c
 * @brief Implementation of a queue that supports FIFO and LIFO operations.
 *
 * This queue implementation uses a singly-linked list to represent the
 * queue elements. Each queue element stores a string value.
 *
 * Assignment for basic C skills diagnostic.
 * Developed for courses 15-213/18-213/15-513 by R. E. Bryant, 2017
 * Extended to store strings, 2018
 *
 * @author zino <theforgotten23623@gmail.com>
 */

#include "queue.h"
#include "harness.h"

#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>

#define CHECK(p, ...)                                                          \
    if (!p) {                                                                  \
        __VA_ARGS__;                                                           \
    }

/**
 * @brief Allocates a new queue
 * @return The new queue, or NULL if memory allocation failed
 */

void init_queue(queue_t *q) {
    q->head = NULL;
    q->tail = NULL;
    q->size = 0;
}

queue_t *queue_new(void) {
    queue_t *q = malloc(sizeof(queue_t));
    /* What if malloc returned NULL? */
    CHECK(q, return NULL);
    init_queue(q);
    return q;
}

/**
 * @brief Free all memory used by a list element
 * @param[in] ele The list element to free
 */

void list_ele_free(list_ele_t *ele) {
    if (!ele) {
        return;
    }

    free(ele->value);
    ele->value = NULL;
}

/**
 * @brief Frees all memory used by a queue
 * @param[in] q The queue to free
 */
void queue_free(queue_t *q) {
    if (!q) {
        return;
    }

    // the next list element to be deallocated
    list_ele_t *next = q->head;
    for (size_t i = 0; i < q->size; i++) {
        list_ele_t *current = next;
        next = next->next;
        list_ele_free(current);
        free(current);
    }

    /* How about freeing the list elements and the strings? */
    /* Free queue structure */
    free(q);
}

/**
 * @brief Attempts to insert an element at head of a queue
 *
 * This function explicitly allocates space to create a copy of `s`.
 * The inserted element points to a copy of `s`, instead of `s` itself.
 *
 * @param[in] q The queue to insert into
 * @param[in] s String to be copied and inserted into the queue
 *
 * @return true if insertion was successful
 * @return false if q is NULL, or memory allocation failed, and leave q
 * untouched
 */
bool queue_insert_head(queue_t *q, const char *s) {
    /* What should you do if the q is NULL? */
    CHECK(q, return false);

    /* What if either call to malloc returns NULL? */
    list_ele_t *newh = malloc(sizeof(list_ele_t));
    CHECK(newh, return false);

    /* Don't forget to allocate space for the string and copy it */
    const size_t l = strlen(s) + 1;
    char *value = malloc(l);

    // this is important
    CHECK(value, free(newh); return false);

    memcpy(value, s, l);

    newh->next = q->head;
    newh->value = value;
    q->head = newh;

    // initial insert, need to initialize tail
    if (q->size == 0) {
        q->tail = newh;
    }

    q->size++;
    return true;
}

/**
 * @brief Attempts to insert an element at tail of a queue
 *
 * This function explicitly allocates space to create a copy of `s`.
 * The inserted element points to a copy of `s`, instead of `s` itself.
 *
 * @param[in] q The queue to insert into
 * @param[in] s String to be copied and inserted into the queue
 *
 * @return true if insertion was successful
 * @return false if q is NULL, or memory allocation failed
 */
bool queue_insert_tail(queue_t *q, const char *s) {
    /* You need to write the complete code for this function */
    CHECK(q, return false);

    list_ele_t *newt = malloc(sizeof(list_ele_t));
    CHECK(newt, return false);

    const size_t l = strlen(s) + 1;
    char *value = malloc(l);
    CHECK(value, free(newt); return false);

    memcpy(value, s, l);

    newt->value = value;
    newt->next = NULL;

    if (q->size == 0) {
        q->head = newt;
    } else {
        q->tail->next = newt;
    }

    q->tail = newt;
    q->size++;

    /* Remember: It should operate in O(1) time */
    return true;
}

/**
 * @brief Attempts to remove an element from head of a queue
 *
 * If removal succeeds, this function frees all memory used by the
 * removed list element and its string value before returning.
 *
 * If removal succeeds and `buf` is non-NULL, this function copies up to
 * `bufsize - 1` characters from the removed string into `buf`, and writes
 * a null terminator '\0' after the copied string.
 *
 * @param[in]  q       The queue to remove from
 * @param[out] buf     Output buffer to write a string value into
 * @param[in]  bufsize Size of the buffer `buf` points to
 *
 * @return true if removal succeeded
 * @return false if q is NULL or empty
 */
bool queue_remove_head(queue_t *q, char *buf, size_t bufsize) {
    /* You need to fix up this code. */
    if (!q || !q->head) {
        return false;
    }

    CHECK(buf, return false);

    list_ele_t *oldh = q->head;
    // strlen does not count the trailing '\0'
    const size_t sizeh = strlen(oldh->value) + 1;
    const size_t cpy_size = bufsize - 1 < sizeh ? bufsize - 1 : sizeh;

    memcpy(buf, oldh->value, cpy_size);
    buf[bufsize - 1] = '\0';

    q->head = oldh->next;
    q->size--;

    // deallocate memory used by the head
    list_ele_free(oldh);
    free(oldh);

    return true;
}

/**
 * @brief Returns the number of elements in a queue
 *
 * This function runs in O(1) time.
 *
 * @param[in] q The queue to examine
 *
 * @return the number of elements in the queue, or
 *         0 if q is NULL or empty
 */
size_t queue_size(queue_t *q) {
    /* You need to write the code for this function */
    CHECK(q, return 0);

    /* Remember: It should operate in O(1) time */
    return q->size;
}

/**
 * @brief Reverse the elements in a queue
 *
 * This function does not allocate or free any list elements, i.e. it does
 * not call malloc or free, including inside helper functions. Instead, it
 * rearranges the existing elements of the queue.
 *
 * @param[in] q The queue to reverse
 */
void queue_reverse(queue_t *q) {
    /* You need to write the code for this function */
    if (!q || q->size <= 1) {
        return;
    }

    list_ele_t *prev = NULL;
    list_ele_t *current = q->head;
    q->tail = current;
    for (size_t i = 0; i < q->size; i++) {
        list_ele_t *next = current->next;
        current->next = prev;
        prev = current;
        current = next;
    }
    q->head = prev;
}
