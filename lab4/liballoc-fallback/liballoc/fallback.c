/**
 * @file
 * @brief
 * @details
 * @author xsestech 
 * @date 19.12.2024
 */
#include <liballoc/fallback.h>
#include <assert.h>

#define ALLOC_MINIMAL_BLOCKS 2

#define get_block_mem_from_meta(block) \
  &(block[1])

#define get_meta_from_mem_ptr(mem) \
  &(((allocator_block_meta_t*)mem)[-1])

typedef struct allocator_block_meta_t allocator_block_meta_t;

struct allocator_block_meta_t {
  size_t size;
  allocator_block_meta_t *prev_in_mem;
  allocator_block_meta_t *next;
};

struct allocator_t {

};

allocator_t *allocator_create_fallback(void *const memory, size_t size) {
  if (!memory) {
    return NULL;
  }
  allocator_t *alloc = memory;
  return alloc;
}
void allocator_destroy_fallback(allocator_t *const allocator) {
}

void *allocator_alloc_fallback(allocator_t *const allocator, const size_t size) {
  return malloc(size);
}
void allocator_free_fallback(allocator_t * const allocator, void * const memory) {
  free(memory);
}
