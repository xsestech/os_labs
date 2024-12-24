/**
 * @file
 * @brief
 * @details
 * @author xsestech 
 * @date 23.12.2024
 */
 
#pragma once

#include <stdlib.h>

typedef struct  allocator_t allocator_t;

typedef allocator_t* allocator_create(void* const memory, size_t size);
typedef void allocator_destroy(allocator_t* const allocator);
typedef void* allocator_alloc(allocator_t* const allocator, const size_t size);
typedef void allocator_free(allocator_t* const allocator, void* const memory);