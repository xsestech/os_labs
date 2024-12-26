/**
 * @file
 * @brief
 * @details
 * @author xsestech 
 * @date 25.12.2024
 */
#include <liballoc/alloc.h>

void allocator_lib_assign_fallbacks(allocator_lib_t* lib) {
  lib->is_fallback = true;
  lib->allocator_alloc = allocator_alloc_fallback;
  lib->allocator_free = allocator_free_fallback;
  lib->allocator_destroy = allocator_destroy_fallback;
  lib->allocator_create = allocator_create_fallback;
}

void allocator_lib_load_func(allocator_lib_t *lib, void **pointer_to_func_field, const char *func_name) {
  if(!lib->is_fallback) {
    *pointer_to_func_field = dlsym(lib->handle, func_name);
    if (!lib->allocator_alloc) {
      print_fd(STDERR_FILENO, "Warning: Could not load function %s, using fallbacks: %s", func_name, dlerror());
      dlclose(lib->handle);
      lib->handle = NULL;
      allocator_lib_assign_fallbacks(lib);
    }
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
    allocator_lib_assign_fallbacks(lib);
    return 0;
  }

  dlerror();
  allocator_lib_load_func(lib, (void**)&lib->allocator_alloc, "allocator_alloc");
  allocator_lib_load_func(lib, (void**)&lib->allocator_free, "allocator_free");
  allocator_lib_load_func(lib, (void**)&lib->allocator_destroy, "allocator_destroy");
  allocator_lib_load_func(lib, (void**)&lib->allocator_create, "allocator_create");
  return 0;
}
void allocator_lib_unload(allocator_lib_t *lib) {
  if (lib->handle) {
    dlclose(lib->handle);
    lib->handle = NULL;
  }
}
