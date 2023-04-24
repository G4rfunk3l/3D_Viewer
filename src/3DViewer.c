#include "3DViewer.h"

#include <ctype.h>

/* Основная функция, принимает путь до файла .obj, парсит строки внутри и
 * создает матрицу координат и матрицу полигонов в соответствии с результатами
 * парсинга */
object_t parsing(char *objfile) {
  size_t len = 0, current_row = 0, size_of_polygon = 0, count_of_polygon = 0;
  double max = EPS;
  char *line = NULL;
  FILE *file = fopen(objfile, "r");
  int *polygon = (int *)calloc(1, sizeof(int));
  double **matrix = (double **)calloc(1, sizeof(double *));
  if (!file) {
    printf("Error: unable to open file %s\n", objfile);
    exit(EXIT_FAILURE);
  }
  while (file && getline(&line, &len, file) != -1) {
    if (*line == 'v' && *(line + 1) == ' ') {
      create_matrix(line + 1, &matrix, current_row++, &max);
    } else if (*line == 'f') {
      create_polygon(line + 1, &polygon, &size_of_polygon);
      count_of_polygon++;
    }
  }
  free(line);
  fclose(file);

  object_t result = {current_row,
                     count_of_polygon,
                     {matrix, max, current_row, 3},
                     {polygon, size_of_polygon}};
  return result;
}
/* Функция принимает строки из .obj файла (с "v " в начале строки) и создает
 * матрицу координат на ее основе */
void create_matrix(char *line, double ***matrix, size_t current_row,
                   double *max) {
  *matrix = (double **)realloc(*matrix, (current_row + 1) * sizeof(double *));
  (*matrix)[current_row] = (double *)calloc(3, sizeof(double));
  for (int column = 0; *(line++) != '\n'; column++) {
    if (*line != '\n') {
      (*matrix)[current_row][column] = strtod(line, &line);
      if ((*matrix)[current_row][column] > *max) {
        *max = (*matrix)[current_row][column];
      }
    }
  }
}

/* Функция принимает строки из .obj файла (с "f" в начале строки) и создает
 * матрицу полигонов на ее основе */
void create_polygon(char *line, int **polygon, size_t *size) {
  size_t not_first = 0, initial_size = *size;
  for (; *(line) && *(line++) != '\n';) {
    if (*(line - 1) == '/') {
      (void)strtod(line, &line);
    } else if (isdigit(*line)) {
      *polygon = (int *)realloc(*polygon, sizeof(int) * (*size + 1));
      (*polygon)[(*size)++] = strtod(line, &line);
      if (not_first) {
        *polygon = (int *)realloc(*polygon, sizeof(int) * (*size + 1));
        (*polygon)[(*size)] = (*polygon)[*size - 1];
        (*size)++;
      }
      not_first = 1;
    }
  }
  *polygon = (int *)realloc(*polygon, sizeof(int) * (*size + 1));
  (*polygon)[(*size)++] = (*polygon)[initial_size];
}

/* Функция позволяет изменять размер 3D-модели путем увеличения всех координат
 * матрицы (и, соответственно, увеличения расстояния между ними) */
void zoom_matrix(matrix_t *matrix, double factor) {
  for (size_t i = 0; i < matrix->rows; i++) {
    for (size_t j = 0; j < matrix->column; j++) {
      matrix->matrix[i][j] *= factor;
    }
  }
}

/* Функция позволяет перемещать 3D-модель по выбранной оси путем прибавления к
 * координатам этой оси величины сдвига */
void move_matrix(matrix_t *matrix, double offset, int axis) {
  if (axis >= 0 && axis < 3) {
    for (size_t i = 0; i < matrix->rows; i++) {
      matrix->matrix[i][axis] += offset;
    }
  }
}

/* Функция позволяет вращать 3D-модель вокруг выбранной оси путем изменения
 * координат двух других осей по формуле вращения */
void rotation_matrix(matrix_t *obj, double angle, int axis) {
  if (fabs(angle) > EPS) {
    double sin_of_angle = sin(angle);
    double cos_of_angle = cos(angle);
    if (axis == X) {
      for (size_t i = 0; i < obj->rows; i++) {
        double y_tmp = obj->matrix[i][Y];
        double z_tmp = obj->matrix[i][Z];
        obj->matrix[i][Y] = cos_of_angle * y_tmp - sin_of_angle * z_tmp;
        obj->matrix[i][Z] = sin_of_angle * y_tmp + cos_of_angle * z_tmp;
      }
    } else if (axis == Y) {
      for (size_t i = 0; i < obj->rows; i++) {
        double x_tmp = obj->matrix[i][X];
        double z_tmp = obj->matrix[i][Z];
        obj->matrix[i][X] = cos_of_angle * x_tmp + sin_of_angle * z_tmp;
        obj->matrix[i][Z] = -sin_of_angle * x_tmp + cos_of_angle * z_tmp;
      }
    } else if (axis == Z) {
      for (size_t i = 0; i < obj->rows; i++) {
        double x_tmp = obj->matrix[i][X];
        double y_tmp = obj->matrix[i][Y];
        obj->matrix[i][X] = cos_of_angle * x_tmp - sin_of_angle * y_tmp;
        obj->matrix[i][Y] = sin_of_angle * x_tmp + cos_of_angle * y_tmp;
      }
    }
  }
}

/* Служебная функция для удаления матрицы координат из памяти */
void remove_matrix(matrix_t *matrix) {
  for (size_t row = 0; row < matrix->rows; row++) {
    free(matrix->matrix[row]);
  }
  free(matrix->matrix);
  matrix->rows = 0;
  matrix->column = 0;
}

/* Служебная функция для удаления матрицы полигонов из памяти */
void remove_polygon(polygon_t *polygon) {
  free(polygon->polygon);
  polygon->size = 0;
}

/* Служебная функция для удаления обьекта 3D-модели из памяти */
void remove_object(object_t *object) {
  remove_matrix(&object->matrix3d);
  remove_polygon(&object->polygon);
  object->count_of_facets = 0;
  object->count_of_vertexes = 0;
}
