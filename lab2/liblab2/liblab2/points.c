/**
 * @file
 * @brief
 * @details
 * @author xsestech 
 * @date 14.11.2024
 */
#include "points.h"
point_t point_create(double x, double y, double z) {
  return (point_t){x, y, z};
}
point_t vector_create(point_t a, point_t b) {
  point_t vec;
  for (int i = 0; i < 3; ++i) {
    vec.coords[i] = b.coords[i] - a.coords[i];
  }
  return vec;
}
double vector_dot_product(point_t a, point_t b) {
  double result = 0;
  for (int i = 0; i < 3; ++i) {
    result += a.coords[i] * b.coords[i];
  }
  return result;
}
point_array_t points_read_from_file(char *file_name) {
  FILE* file = fopen(file_name, "r");
  if (!file) {
    return (point_array_t){NULL, 0};
  }
  uint32_t n;
  point_array_t array;
  fscanf(file, "%ud", &n);
  array.size = n;
  array.points = malloc(sizeof(point_t) * n);
  for (uint32_t i = 0; i < n; ++i) {
    for(int j = 0; j < 3; ++j) {
      fscanf(file, "%lf", &(array.points[i].coords[j]));
    }

  }
  return array;
}
void points_array_destroy(point_array_t points) {
  free(points.points);
}
