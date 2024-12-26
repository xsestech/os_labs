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
} ptimer_t;

void ptimer_start(ptimer_t* timer);

void ptimer_end(ptimer_t* timer);

void ptimer_print(ptimer_t* timer);