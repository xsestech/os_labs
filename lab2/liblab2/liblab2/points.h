/**
 * @file
 * @brief
 * @details
 * @author xsestech 
 * @date 14.11.2024
 */
#ifndef POINTS_H
#define POINTS_H

#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>


typedef struct {
  double coords[3];
} point_t;

typedef struct  {
  point_t* points;
  size_t size;
} point_array_t;

point_t point_create(double x, double y, double z);
point_t vector_create(point_t a, point_t b);
double vector_dot_product(point_t a, point_t b);
void* calcucate_max_area();
point_array_t points_read_from_file(char* file_name);
void points_array_destroy(point_array_t points);

#endif //POINTS_H
