/**
 * @file
 * @brief
 * @details
 * @author xsestech 
 * @date 19.12.2024
 */
 
#pragma once

#include <stdlib.h>

#define ALLOC_FREEBLOCKS_PREV_PTR_MERGE


typedef struct  allocator_t allocator_t;

allocator_t* allocator_create(void* const memory, size_t size);
void allocator_destroy(allocator_t* const allocator);
void* allocator_alloc(allocator_t* const allocator, const size_t size);
void allocator_free(allocator_t* const allocator, void* const memory);