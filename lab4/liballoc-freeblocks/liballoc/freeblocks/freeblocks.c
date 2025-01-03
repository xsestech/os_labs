/**
 * @file
 * @brief
 * @details
 * @author xsestech 
 * @date 19.12.2024
 */
#include <liballoc/freeblocks/freeblocks.h>

#define get_block_mem_from_meta(block) \
  &(block[1])

#define get_meta_from_mem_ptr(mem) \
  &(((allocator_block_meta_t*)mem)[-1])

#define get_next_block_ptr(block, new_size) \
(void*)((char*)get_block_mem_from_meta(block) + new_size - 1)

typedef struct allocator_block_meta_t allocator_block_meta_t;

struct allocator_block_meta_t {
  size_t size;
  allocator_block_meta_t *prev_in_mem;
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
  allocator_block_meta_t *next_free = get_next_block_ptr(memory, sizeof(allocator_t));
  next_free->size = size - sizeof(allocator_t) - sizeof(allocator_block_meta_t);
  next_free->next = NULL;
  next_free->prev_in_mem = NULL;
  alloc->next_free = next_free;
  return alloc;
}
void allocator_destroy(allocator_t *const allocator) {
  allocator->next_free = NULL;
  allocator->max_ptr = NULL;
}

void *allocator_alloc(allocator_t *const allocator, const size_t size) {
  allocator_block_meta_t *block = allocator->next_free;
  allocator_block_meta_t *prev_block = NULL;
  if (block == NULL) {
    return NULL;
  }
  while (block->next && block->size < size) {
    prev_block = block;
    block = block->next;
  }
  if (block->size < size) {
    return NULL;
  }
  if (block->size < size + sizeof(allocator_block_meta_t)) {
    if (block->next) {
      allocator->next_free = block->next;
      block->next->prev = NULL; // Update prev pointer
    }
    block->next = NULL;
    return get_block_mem_from_meta(block);
  }
  allocator_block_meta_t *new_free_block = get_next_block_ptr(block, size);
  new_free_block->size = block->size - size - sizeof(allocator_block_meta_t);
  new_free_block->next = block->next;
  new_free_block->prev_in_mem = block;
  new_free_block->prev = prev_block;
  if (prev_block != NULL) {
    prev_block->next = new_free_block;
  } else {
    allocator->next_free = new_free_block;
  }
  if (new_free_block->next) {
    new_free_block->next->prev = new_free_block;
  }
  block->size = size;
  block->next = NULL;
  return get_block_mem_from_meta(block);
}

allocator_block_meta_t *allocator_merge(allocator_t *const allocator, allocator_block_meta_t *block) {
#if defined(ALLOC_FREEBLOCKS_PREV_PTR_MERGE)
  if (block->prev_in_mem && get_next_block_ptr(block->prev_in_mem, block->prev_in_mem->size) == block) {
    block->prev_in_mem->size += block->size + sizeof(allocator_block_meta_t);
    block->prev_in_mem->prev = block->prev_in_mem->next;
    block->prev_in_mem->next = block->next;
    block = block->prev_in_mem;
  }
#endif
  allocator_block_meta_t *next_block = get_next_block_ptr(block, block->size);
  if ((void *) next_block < allocator->max_ptr && (next_block->next || next_block == allocator->next_free)) {
    block->size += next_block->size + sizeof(allocator_block_meta_t);
  }
  return block;
}

void allocator_free(allocator_t *const allocator, void *const memory) {
  allocator_block_meta_t *block = get_meta_from_mem_ptr(memory);
  block->next = allocator->next_free;
  if(allocator->next_free) {
    allocator->next_free->prev = block;
  }
  block = allocator_merge(allocator, block);
  allocator->next_free = block;
}
