/**
 * @file
 * @brief
 * @details
 * @author xsestech 
 * @date 19.12.2024
 */
#include "freeblocks.h"
#include <assert.h>

#define ALLOC_MINIMAL_BLOCKS 2

#define get_block_mem_from_meta(block) \
  &(block[1])

#define get_meta_from_mem_ptr(mem) \
  &(((allocator_block_meta_t*)mem)[-1])

typedef struct allocator_block_meta_t allocator_block_meta_t;

struct allocator_block_meta_t {
  size_t size;
  allocator_block_meta_t *prev;
  allocator_block_meta_t *next;
};

struct allocator_t {
  allocator_block_meta_t *next_free;
  void *max_ptr;
};

allocator_t *allocator_create(void *const memory, size_t size) {
  if (!memory) {
    return NULL;
  }
  if (size < sizeof(allocator_t)) {
    return NULL;
  }
  allocator_t *alloc = memory;
  alloc->max_ptr = (char *) memory + size;
  allocator_block_meta_t *next_free = memory + sizeof(allocator_t);
  next_free->size = size - sizeof(allocator_t);
  next_free->next = alloc->max_ptr;
  next_free->prev = alloc->max_ptr;
  return alloc;
}
void allocator_destroy(allocator_t *const allocator) {
}

void *allocator_alloc(allocator_t *const allocator, const size_t size) {
  allocator_block_meta_t *block = allocator->next_free;
  if (block == allocator->max_ptr) {
    return NULL;
  }
  while (block->next != allocator->max_ptr && block->size >= size) {
    block = block->next;
  }
  if (block->next == allocator->max_ptr && block->size < size) {

    return NULL;
  }
  if (block->size < size + sizeof(allocator_block_meta_t)) {
    if (block == allocator->next_free) {
      allocator->next_free = block->next;
    } else {
      block->prev->next = block->next;
    }
    block->next = NULL;
    return get_block_mem_from_meta(block);
  }
  allocator_block_meta_t *new_free_block = get_block_mem_from_meta(block) + size;
  new_free_block->size = block->size - size - sizeof(allocator_block_meta_t);
  new_free_block->next = block->next;
  new_free_block->prev = block->prev;
  if (block == allocator->next_free) {
    allocator->next_free = new_free_block;
  }
  block->next = NULL;
  return get_block_mem_from_meta(block);
}
void allocator_free(allocator_t * const allocator, void * const memory) {
  allocator_block_meta_t* block = get_meta_from_mem_ptr(memory);
  block->next = allocator->next_free;
  allocator->next_free = block;
}
