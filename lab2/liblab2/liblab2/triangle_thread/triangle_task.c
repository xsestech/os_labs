/**
 * @file
 * @brief
 * @details
 * @author xsestech 
 * @date 19.11.2024
 */
#include "triangle_task.h"



triangle_task_t *triangle_task_create(uint32_t id, point_t a, point_t b, point_t c) {
 triangle_task_t *task = malloc(sizeof(triangle_task_t));
 if (task == NULL) {
  fprintf(stderr, "Failed to allocate memory for triangle_task_t\n");
  exit(EXIT_FAILURE);
 }
 task->id = id;
 task->points[0] = a;
 task->points[1] = b;
 task->points[2] = c;
 return task;
}

void triangle_task_destroy(triangle_task_t *task) {
 free(task);
}