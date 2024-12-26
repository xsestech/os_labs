/**
 * @file
 * @brief
 * @details
 * @author xsestech 
 * @date 23.12.2024
 */

#pragma once

#include <stdlib.h>
#include <libio/io.h>
#include <dlfcn.h>
#include <liballoc/fallback.h>

typedef struct allocator_t allocator_t;

typedef allocator_t *allocator_create_func(void *const memory, size_t size);
typedef void allocator_destroy_func(allocator_t *const allocator);
typedef void *allocator_alloc_func(allocator_t *const allocator, const size_t size);
typedef void allocator_free_func(allocator_t *const allocator, void *const memory);

typedef struct {
  allocator_create_func *allocator_create;
  allocator_destroy_func *allocator_destroy;
  allocator_alloc_func *allocator_alloc;
  allocator_free_func *allocator_free;
  void *handle;
  bool is_fallback;
} allocator_lib_t;

int allocator_lib_load(allocator_lib_t *lib, const char *path);
void allocator_lib_unload(allocator_lib_t *lib);
