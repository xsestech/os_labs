/**
 * @file
 * @brief
 * @details
 * @author xsestech 
 * @date 19.11.2024
 */
#ifndef TRIANGLE_ARGS_H
#define TRIANGLE_ARGS_H

#include <libstruct/concurrent/queue.h>

typedef struct {
 uint32_t thread_id;
 pqueue_handle_t q;
} triangle_args_t;

triangle_args_t* triangle_args_init(const uint32_t thread_id,
                                   pqueue_handle_t q);
void triangle_args_destroy(triangle_args_t* args);
#endif //TRIANGLE_ARGS_H
