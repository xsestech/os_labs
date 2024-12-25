/**
 * @file
 * @brief
 * @details
 * @author xsestech 
 * @date 23.12.2024
 */

#include <liballoc/alloc.h>
#include <liballoc/fallback.h>
#include <libio/io.h>
#include <dlfcn.h>
#include <sys/mman.h>

#define ALLOC_MEMORY_SIZE 131072 // 32 4KB blocks

int main(int argc, char **argv) {
  (void) argc;

  if (argc != 2) {
    print_fd(STDERR_FILENO, "Error: wrong number of args\n");
    return -1;
  }
  allocator_lib_t lib;
  if (allocator_lib_load(&lib, argv[1]) == -1) {
    print_fd(STDERR_FILENO, "Error: couldn't load allocator");
    return -1;
  }
  void *memory = mmap(NULL, ALLOC_MEMORY_SIZE, PROT_WRITE | PROT_READ, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
  if (memory == MAP_FAILED) {
    print_fd(STDERR_FILENO, "Error: can't allocate memory\n");
    return -1;
  }
  void* allocator = lib.allocator_create(memory, ALLOC_MEMORY_SIZE);
  if (allocator == NULL) {
    print_fd(STDERR_FILENO, "Alloc error\n");
    munmap(memory, ALLOC_MEMORY_SIZE);
    return -1;
  }
  const size_t mems_size = ALLOC_MEMORY_SIZE / 1024 / 2;
  void** mems[mems_size];
  for (size_t i = 0; i < mems_size; ++i) {
    mems[i] = lib.allocator_alloc(allocator, 1024);
    if (!mems[i]) {
      print_fd(STDERR_FILENO, "Error during allocation");
      lib.allocator_destroy(allocator);
      munmap(memory, ALLOC_MEMORY_SIZE);
      return -1;
    }
  }
  for (size_t i = 0; i < mems_size; ++i) {
    if (i % 2 == 0) {
      lib.allocator_free(allocator, mems[i]);
    }
  }
  for (size_t i = 0; i < mems_size; ++i) {
    if (i % 2 == 0) {
      lib.allocator_free(allocator, mems[i]);
    }
  }
  for (size_t i = 0; i < mems_size; ++i) {
    mems[i] = lib.allocator_alloc(allocator, 1024);
    if (!mems[i]) {
      print_fd(STDERR_FILENO, "Error during allocation");
      lib.allocator_destroy(allocator);
      munmap(memory, ALLOC_MEMORY_SIZE);
      return -1;
    }
  }
  size_t allocated_count = 0;
  while(lib.allocator_alloc(allocator, 256)) {
    allocated_count++;
  }

  lib.allocator_destroy(allocator);

  munmap(memory, ALLOC_MEMORY_SIZE);
  return 0;
}
