/**
 * @file
 * @brief
 * @details
 * @author xsestech 
 * @date 19.12.2024
 */
#include <stdbool.h>
#include <liballoc/buddy/buddy.h>

#define MAX_SIZE_T_POW2 32
#define MIN_BLOCK_SIZE_POW2 5

#define get_block_mem_from_meta(block) \
  &(block[1])

#define get_meta_from_mem_ptr(mem) \
  &(((allocator_block_meta_t*)mem)[-1])

#define get_next_block_ptr(block, new_size) \
(void*)((char*)block + (1 << new_size))

typedef struct allocator_block_meta_t allocator_block_meta_t;

struct allocator_block_meta_t {
  size_t size;
  allocator_block_meta_t *prev_in_mem;
  allocator_block_meta_t *prev;
  allocator_block_meta_t *next;
};

struct allocator_t {
  allocator_block_meta_t *pow2blocks[MAX_SIZE_T_POW2];
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
  next_free->size = log2(size - sizeof(allocator_t));
  next_free->next = NULL;
  next_free->prev_in_mem = NULL;
  for (size_t i = 0; i < MAX_SIZE_T_POW2; ++i) {
    alloc->pow2blocks[i] = NULL;
  }
  alloc->pow2blocks[next_free->size] = next_free;
  return alloc;
}
void allocator_destroy(allocator_t *const allocator) {
  for (size_t i = 0; i < MAX_SIZE_T_POW2; ++i) {
    allocator->pow2blocks[i] = NULL;
  }
  allocator->max_ptr = NULL;
}

static void remove_from_pow2blocks(allocator_t *const allocator, allocator_block_meta_t *block, size_t level) {
  if (block->prev) {
    block->prev->next = block->next;
  } else {
    allocator->pow2blocks[level] = block->next;
  }
  if (block->next) {
    block->next->prev = block->prev;
  }
}

static void add_to_pow2blocks(allocator_t *const allocator, allocator_block_meta_t *block, size_t level) {
  block->next = allocator->pow2blocks[level];
  block->prev = NULL;
  if (allocator->pow2blocks[level]) {
    allocator->pow2blocks[level]->prev = block;
  }
  allocator->pow2blocks[level] = block;
}

void *allocator_alloc(allocator_t *const allocator, const size_t size) {
  size_t pow2size = ceil(log2(size + sizeof(allocator_block_meta_t)));
  if (pow2size < MIN_BLOCK_SIZE_POW2) {
    pow2size = MIN_BLOCK_SIZE_POW2;
  }
  allocator_block_meta_t *block = NULL;
  size_t closest_idx;
  for (closest_idx = pow2size; closest_idx < MAX_SIZE_T_POW2; ++closest_idx) {
    if (allocator->pow2blocks[closest_idx]) {
      block = allocator->pow2blocks[closest_idx];
      break;
    }
  }
  if (block == NULL) {
    return NULL;
  }
  if (closest_idx == pow2size) {
    remove_from_pow2blocks(allocator, block, pow2size);
    block->next = NULL;
    block->prev = NULL;
    return get_block_mem_from_meta(block);
  }
  while (closest_idx > pow2size) {
    remove_from_pow2blocks(allocator, block, closest_idx);
    size_t new_size = block->size - 1;
    allocator_block_meta_t *new_block = get_next_block_ptr(block, new_size);
    new_block->size = new_size;
    new_block->next = block;
    new_block->prev_in_mem = block;
    new_block->prev = NULL;

    block->size = new_size;
    block->prev = new_block;
    block->prev_in_mem = NULL;
    add_to_pow2blocks(allocator, new_block, new_size);
    closest_idx--;
  }
  remove_from_pow2blocks(allocator, block, pow2size);
  block->next = NULL;
  block->prev = NULL;
  return get_block_mem_from_meta(block);
}

allocator_block_meta_t *allocator_merge(allocator_t *const allocator, allocator_block_meta_t *block, bool *was_merged) {
  *was_merged = false;

  // Merge with the previous block if possible
  if (block->prev_in_mem && block->prev_in_mem->size == block->size &&
    get_next_block_ptr(block->prev_in_mem, block->prev_in_mem->size) == block) {
    remove_from_pow2blocks(allocator, block->prev_in_mem, block->size);
    block->prev_in_mem->size++;
    block = block->prev_in_mem;
    *was_merged = true;
  }

  // Merge with the next block if possible
  allocator_block_meta_t *next_block = get_next_block_ptr(block, block->size);
  if ((void *) next_block < allocator->max_ptr && next_block->size == block->size &&
    (next_block->next || next_block == allocator->pow2blocks[next_block->size])) {
    remove_from_pow2blocks(allocator, next_block, block->size);
    block->size++;
    *was_merged = true;
  }

  return block;
}

void allocator_free(allocator_t *const allocator, void *const memory) {
  if (!allocator || !memory) {
    return;
  }
  allocator_block_meta_t *block = get_meta_from_mem_ptr(memory);
  bool was_merged = true;
  while (was_merged) {
    block = allocator_merge(allocator, block, &was_merged);
  }
  add_to_pow2blocks(allocator, block, block->size);
}
