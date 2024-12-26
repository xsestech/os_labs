/**
 * @file
 * @brief
 * @details
 * @author xsestech 
 * @date 26.12.2024
 */
 
#pragma once

#include <sys/time.h>

#include "libio/io.h"

typedef struct {
 struct timeval time_start;
 struct timeval time_end;
} timer_t;

void timer_start(timer_t* timer);

void timer_end(timer_t* timer);

void timer_print(timer_t* timer);