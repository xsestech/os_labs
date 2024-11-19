/**
 * @file
 * @brief
 * @details
 * @author xsestech 
 * @date 14.11.2024
 */

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>
#include <liblab2/thread.h>

#include <sys/resource.h>
#include <math.h>

rlim_t get_thead_limit() {
  struct rlimit thread_limits;
  if (getrlimit(RLIMIT_NPROC, &thread_limits) == -1) {
    return -1;
  }
  return thread_limits.rlim_cur;
}

int main(int argc, char *argv[]) {
  setlocale(LC_NUMERIC, "C"); // set local that interprets dots as fp separator

  if (argc != 4) {
    perror("Invalid number of args");
    return -1;
  }

  if (strncmp(argv[1], "-j", 2) != 0) {
    perror("Invaild arg");
    return -1;
  }
  const int thread_limit = get_thead_limit();
  char *endptr;
  const uint32_t threads_num = strtoul(argv[2], &endptr, 10);
  if (endptr == argv[2]) {
    perror("Invalid thread number format");
    return -1;
  }
  if (threads_num > thread_limit) {
    perror("Too big number of threads, execeed system limit");
    return -1;
  }

  point_array_t points = points_read_from_file(argv[3]);

  const size_t queue_size = (size_t) pow((double) points.size, 3) / threads_num + 1;

  triangle_thread_t threads[threads_num];
  for (uint32_t i = 0; i < threads_num; ++i) {
    threads[i] = triangle_thread_init(i, queue_size);
  }

  uint32_t task_id = 0;
  for (uint32_t i = 0; i < points.size; ++i) {
    for (uint32_t j = i + 1; j < points.size; ++j) {
      for (uint32_t k = j + 1; k < points.size; ++k) {
        triangle_thread_send_points(
          threads[task_id % threads_num],
          task_id,
          points.points[i],
          points.points[j],
          points.points[k]);
      }
    }
    task_id++;
  }

  double max_value = -INFINITY;
  for (uint32_t i = 0; i < threads_num; ++i) {
    const double value = triangle_thread_join(&threads[i]);
    if (value > max_value) {
      max_value = value;
    }
  }

  printf("%lf", max_value);
  points_array_destroy(points);

  return 0;
}
