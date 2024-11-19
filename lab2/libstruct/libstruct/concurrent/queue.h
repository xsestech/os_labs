/**
 * @file
 * @brief
 * @details
 * @author xsestech 
 * @date 14.11.2024
 */
#ifndef QUEUE_H
#define QUEUE_H

#include <pthread.h>
#include <unistd.h>
#include <assert.h>
#include <stdlib.h>
#include <stdbool.h>

typedef struct pqueue_t *pqueue_handle_t;

pqueue_handle_t pqueue_create(size_t queue_size);
void pqueue_destroy(pqueue_handle_t queue);
void pqueue_push(pqueue_handle_t queue, void *item);
void *pqueue_pop(pqueue_handle_t queue);

#endif //QUEUE_H
