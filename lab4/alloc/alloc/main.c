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
#include <sys/time.h>

#include "timer.h"

#define ALLOC_MEMORY_SIZE 33554732 // 32MB
// #define ALLOC_MEMORY_SIZE 5 * 1024 + 600

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
  void *allocator = lib.allocator_create(memory, ALLOC_MEMORY_SIZE);
  if (allocator == NULL) {
    print_fd(STDERR_FILENO, "Alloc error\n");
    munmap(memory, ALLOC_MEMORY_SIZE);
    return -1;
  }
  srand(0);
  ptimer_t timer_alloc, timer_free;

  const size_t mems_size = ALLOC_MEMORY_SIZE / 1024 * 0.8;
  void **mems[mems_size];
  ptimer_start(&timer_alloc);
  for (size_t i = 0; i < mems_size; ++i) {
    mems[i] = lib.allocator_alloc(allocator, 600);
    if (!mems[i]) {
      print_fd(STDERR_FILENO, "Error during allocation");
      lib.allocator_destroy(allocator);
      munmap(memory, ALLOC_MEMORY_SIZE);
      return -1;
    }
  }
  ptimer_end(&timer_alloc);
  print_fd(STDOUT_FILENO, "Allocation time:\n");
  ptimer_print(&timer_alloc);
  size_t deallocated = 0;
  for (size_t i = 0; i < mems_size; ++i) {
    if (rand() % 2 == 0) {
      lib.allocator_free(allocator, mems[i]);
      deallocated++;
    }
  }
  ptimer_start(&timer_free);
  deallocated *= 0.8;
  for (size_t i = 0; i < deallocated / 2; ++i) {
    if (!lib.allocator_alloc(allocator, 512)) {
      print_fd(STDERR_FILENO, "Error during allocation");
      lib.allocator_destroy(allocator);
      munmap(memory, ALLOC_MEMORY_SIZE);
      return -1;
    }
  }
  ptimer_end(&timer_free);
  print_fd(STDOUT_FILENO, "Free time:\n");
  ptimer_print(&timer_free);

  size_t allocated_count = 0;
  while (lib.allocator_alloc(allocator, 256)) {
    allocated_count++;
  }

  print_fd(STDOUT_FILENO, "Max 256 allocation count: %lu", allocated_count);
  lib.allocator_destroy(allocator);

  munmap(memory, ALLOC_MEMORY_SIZE);
  return 0;
}
