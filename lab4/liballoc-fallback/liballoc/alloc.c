/**
 * @file
 * @brief
 * @details
 * @author xsestech 
 * @date 25.12.2024
 */
#include <liballoc/alloc.h>

void allocator_lib_load_func(allocator_lib_t *lib, void **pointer_to_func_field, void* fallback, const char *func_name) {
  *pointer_to_func_field = dlsym(lib->handle, func_name);
  if (!lib->allocator_alloc) {
    print_fd(STDERR_FILENO, "Warning: Could not load function %s, using fallback function: %s", func_name, dlerror());
    *pointer_to_func_field = fallback;
  }
}

int allocator_lib_load(allocator_lib_t *lib, const char *path) {
  if (!lib || !path) {
    print_fd(STDERR_FILENO, "Error: Path or lib pointer can't be null");
    return -1;
  }
  lib->handle = dlopen(path, RTLD_LOCAL | RTLD_NOW);
  if (!lib->handle) {
    print_fd(STDERR_FILENO, "Warning: Could not load library, using fallback functions: %s\n", dlerror());
    lib->allocator_alloc = allocator_alloc_fallback;
    lib->allocator_free = allocator_free_fallback;
    lib->allocator_destroy = allocator_destroy_fallback;
    lib->allocator_create = allocator_create_fallback;
    return 0;
  }

  dlerror();

  allocator_lib_load_func(lib, (void**)&lib->allocator_alloc, allocator_alloc_fallback, "allocator_alloc");
  allocator_lib_load_func(lib, (void**)&lib->allocator_free, allocator_free_fallback, "allocator_free");
  allocator_lib_load_func(lib, (void**)&lib->allocator_destroy, allocator_destroy_fallback, "allocator_destroy");
  allocator_lib_load_func(lib, (void**)&lib->allocator_create, allocator_create_fallback, "allocator_create");
  return 0;
}
void allocator_lib_unload(allocator_lib_t *lib) {
  if (lib->handle) {
    dlclose(lib->handle);
    lib->handle = NULL;
  }
}
