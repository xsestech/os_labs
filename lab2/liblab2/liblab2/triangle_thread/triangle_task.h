/**
 * @file
 * @brief
 * @details
 * @author xsestech 
 * @date 19.11.2024
 */
#ifndef TRAINGLE_TASK_H
#define TRAINGLE_TASK_H

#include <stdlib.h>
#include <liblab2/points.h>

typedef struct {
 uint32_t id;
 point_t points[3];
} triangle_task_t;

triangle_task_t *triangle_task_create(uint32_t id, point_t a, point_t b, point_t c);
void triangle_task_destroy(triangle_task_t *task);

#endif //TRAINGLE_TASK_H
