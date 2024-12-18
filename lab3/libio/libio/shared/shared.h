/**
 * @file
 * @brief
 * @details
 * @author xsestech 
 * @date 12.12.2024
 */
 
#pragma once

#include <libio/shared/types.h>
#include <assert.h>
#include <string.h>
#include <sys/mman.h>
#include <unistd.h>

shared_handle_t shared_producer_open(char* name, size_t size);
shared_handle_t shared_consumer_open(char* name, size_t size);
char* shared_get_str(shared_handle_t shared);
int shared_put_str(shared_handle_t shared, char* str);
void shared_close(shared_handle_t shared);