/**
 * @file
 * @brief
 * @details
 * @author xsestech 
 * @date 18.11.2024
 */
#include <liblab2/triangle_thread/triangle_thead.h>

void *triangle_thread_func(void *thead_args) {
  triangle_args_t *args = thead_args;
  double *max_area = malloc(sizeof(double));
  *max_area = -INFINITY;
  while (true) {
    triangle_task_t *task = pqueue_pop(args->q);
    if (task == NULL) {
      printf("Thread %d finished its work\n", args->thread_id);
      return max_area;
    }
    point_t v1 = vector_create(task->points[1], task->points[0]);
    point_t v2 = vector_create(task->points[1], task->points[2]);
    double area = vector_dot_product(v1, v2);
    triangle_task_destroy(task);
    if (area > *max_area) {
      *max_area = area;
    }
  }
}

triangle_thread_t triangle_thread_init(uint32_t id, size_t queue_size) {
  triangle_thread_t triangle_thread;
  pqueue_handle_t q = pqueue_create(queue_size);
  triangle_thread.args = triangle_args_init(id, q);
  pthread_create(&triangle_thread.thread, NULL, triangle_thread_func, triangle_thread.args);
  return triangle_thread;
}

void triangle_thread_send(triangle_thread_t thread, void *item) {
  pqueue_push(thread.args->q, item);
}

void triangle_thread_send_points(triangle_thread_t thread, uint32_t id, point_t a, point_t b, point_t c) {
  triangle_task_t *task = triangle_task_create(id, a, b, c);
  triangle_thread_send(thread, task);
}

double triangle_thread_join(triangle_thread_t *thread) {
  void *max_value_p;
  triangle_thread_send(*thread, NULL);
  pthread_join(thread->thread, &max_value_p);

  if (!max_value_p) {
    return NAN;
  }
  double max_value = *((double *) max_value_p);
  return max_value;
}
