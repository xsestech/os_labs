/**
 * @file
 * @brief
 * @details
 * @author xsestech 
 * @date 19.11.2024
 */
#include "triangle_args.h"

triangle_args_t *triangle_args_init(const uint32_t thread_id,
                                    pqueue_handle_t q) {
  triangle_args_t *triangle_args = malloc(sizeof(triangle_args_t));
  triangle_args->q = q;
  triangle_args->thread_id = thread_id;
  return triangle_args;
}
void triangle_args_destroy(triangle_args_t *args) {
  pqueue_destroy(args->q);
  free(args);
}
