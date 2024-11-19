/**
 * @file
 * @brief
 * @details
 * @author xsestech 
 * @date 18.11.2024
 */
#ifndef TRIANGLE_THEAD_H
#define TRIANGLE_THEAD_H

#include <pthread.h>
#include <liblab2/triangle_thread/triangle_args.h>
#include <liblab2/triangle_thread/triangle_task.h>
#include <stdbool.h>
#include <float.h>






typedef struct {
  pthread_t thread;
  triangle_args_t* args;
} triangle_thread_t;



triangle_thread_t triangle_thread_init(uint32_t id, size_t queue_size);
void triangle_thread_send_points(triangle_thread_t thread, uint32_t id, point_t a, point_t b, point_t c);
double triangle_thread_join(triangle_thread_t* thread);


void *triangle_thread(void *thead_args);

#endif //TRIANGLE_THEAD_H
