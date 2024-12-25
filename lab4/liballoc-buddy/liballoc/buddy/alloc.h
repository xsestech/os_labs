/**
 * @file
 * @brief
 * @details
 * @author xsestech 
 * @date 19.12.2024
 */
 
#pragma once

#include <stdlib.h>


typedef struct  allocator_t allocator_t;

allocator_t* allocator_create_fallback(void* const memory, size_t size);
void allocator_destroy_fallback(allocator_t* const allocator);
void* allocator_alloc_fallback(allocator_t* const allocator, const size_t size);
void allocator_free_fallback(allocator_t* const allocator, void* const memory);