/**
 * @file
 * @brief
 * @details
 * @author xsestech 
 * @date 26.12.2024
 */
#include "timer.h"

void ptimer_start(ptimer_t* timer) {
 gettimeofday(&timer->time_start, NULL);
}

void ptimer_end(ptimer_t* timer) {
 gettimeofday(&timer->time_end, NULL);
}

void ptimer_print(ptimer_t* timer) {
 print_fd(STDOUT_FILENO, "Elapsed time: %ld.%06ld\n", timer->time_end.tv_sec - timer->time_start.tv_sec, timer->time_end.tv_usec - timer->time_start.tv_usec);
}