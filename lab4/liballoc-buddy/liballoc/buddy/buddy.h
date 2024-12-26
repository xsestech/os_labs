/**
 * @file
 * @brief
 * @details
 * @author xsestech 
 * @date 19.12.2024
 */
 
#pragma once

#include <stdlib.h>
#include <math.h>


typedef struct  allocator_t allocator_t;

/**
 * Creates allocator structure on pre-allocated chunk of memory
 * @param memory pre-allocated chunk of memory
 * @param size size of chunk
 * @return pointer to allocator structure
 */
allocator_t* allocator_create(void* const memory, size_t size);
/**
 * Destroys allocator structure
 * @param allocator pointer to allocator
 */
void allocator_destroy(allocator_t* const allocator);
/**
 * Allocates memory of given size on chunk and returns pointer to it
 * @param allocator pointer to allocator
 * @param size size of allocated block
 */
void* allocator_alloc(allocator_t* const allocator, const size_t size);
/**
 * Free previously allocated memory
 * @param allocator pointer to allocator
 * @param memory previously allocated memory
 */
void allocator_free(allocator_t* const allocator, void* const memory);