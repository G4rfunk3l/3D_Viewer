#ifndef SRC_3DVIEWER_H_
#define SRC_3DVIEWER_H_

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define EPS 1e-10

enum { X, Y, Z, AXIS };

/* Структура матрицы координат */
typedef struct matrix_t {
  double **matrix;
  double max;
  size_t rows;
  size_t column;
} matrix_t;

/* Структура матрицы полигонов */
typedef struct polygon_t {
  int *polygon;
  size_t size;
} polygon_t;

/* Структура объекта 3D-модели */
typedef struct object_t {
  size_t count_of_vertexes;
  size_t count_of_facets;
  matrix_t matrix3d;
  polygon_t polygon;
} object_t;

typedef void (*rotation_t[AXIS])(matrix_t *, double);

object_t parsing(char *objfile);

void create_matrix(char *line, double ***matrix, size_t current_row,
                   double *max);
void create_polygon(char *line, int **polygon, size_t *size);

void zoom_matrix(matrix_t *matrix, double factor);
void move_matrix(matrix_t *matrix, double offset, int axis);
void rotation_matrix(matrix_t *obj, double angle, int axis);

void remove_matrix(matrix_t *matrix);
void remove_polygon(polygon_t *polygon);
void remove_object(object_t *object);

#endif  //  SRC_3DVIEWER_H_
