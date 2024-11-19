/**
 * @file
 * @brief
 * @details
 * @author xsestech 
 * @date 14.11.2024
 */
#include <stdio.h>
#include <libstruct/concurrent/queue.h>

struct pqueue_t {
  void **data;
  size_t top;
  size_t bottom;
  size_t size;
  size_t capacity;
  pthread_mutex_t lock;
  pthread_cond_t not_empty;
  pthread_cond_t not_full;
};

pqueue_handle_t pqueue_create(size_t queue_size) {
  pqueue_handle_t pqueue = malloc(sizeof(struct pqueue_t));
  if (!pqueue) {
    return NULL;
  }
  pqueue->data = malloc(sizeof(void *) * queue_size);
  if (!pqueue->data) {
    free(pqueue);
    return NULL;
  }
  pqueue->capacity = queue_size;
  pqueue->size = 0;
  pqueue->top = 0;
  pqueue->bottom = 0;
  pthread_mutex_init(&pqueue->lock, NULL);
  pthread_cond_init(&pqueue->not_empty, NULL);
  pthread_cond_init(&pqueue->not_full, NULL);
  return pqueue;
}
void pqueue_destroy(pqueue_handle_t queue) {
  free(queue->data);
  pthread_cond_destroy(&queue->not_empty);
  pthread_cond_destroy(&queue->not_full);
  pthread_mutex_destroy(&queue->lock);
  free(queue);
}

void pqueue_push(pqueue_handle_t queue, void *item) {
  pthread_mutex_lock(&queue->lock);

  while (queue->size == queue->capacity) {
    pthread_cond_wait(&queue->not_full, &queue->lock);
  }
  queue->data[queue->top] = item;
  queue->top = (queue->top + 1) % queue->capacity;
  queue->size++;

  pthread_cond_signal(&queue->not_empty);
  pthread_mutex_unlock(&queue->lock);
}
void *pqueue_pop(pqueue_handle_t queue) {
  pthread_mutex_lock(&queue->lock);

  while (queue->size == 0) {
    pthread_cond_wait(&queue->not_empty, &queue->lock);
  }
  void *item = queue->data[queue->bottom];
  queue->bottom = (queue->bottom + 1) % queue->capacity;
  queue->size--;

  pthread_cond_signal(&queue->not_full);
  pthread_mutex_unlock(&queue->lock);
  return item;
}


