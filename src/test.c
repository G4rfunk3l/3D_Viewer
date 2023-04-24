#include <check.h>
#include <stdio.h>
#include <stdlib.h>

#include "3DViewer.h"

matrix_t fill_matrix_from_array(const double src[][3], size_t rows) {
  matrix_t matrix;
  matrix.rows = rows;
  matrix.column = 3;
  matrix.max = 0;
  matrix.matrix = (double **)malloc(rows * sizeof(double *));
  for (size_t row = 0; row < rows; row++) {
    matrix.matrix[row] = (double *)malloc(3 * sizeof(double));
  }

  for (size_t row = 0; row < rows; row++) {
    for (size_t column = 0; column < 3; column++) {
      matrix.matrix[row][column] = src[row][column];
    }
  }

  return matrix;
}

polygon_t fill_polygon_from_array(const int src[], size_t size) {
  polygon_t polygon;
  polygon.size = size * 2;

  polygon.polygon = (int *)malloc(size * sizeof(int));
  for (size_t i = 0; i < size; i++) {
    polygon.polygon[i] = src[i];
  }

  return polygon;
}

void object_compare(object_t obj_1, object_t obj_2) {
  for (size_t row = 0; row < obj_1.matrix3d.rows; row++) {
    for (size_t column = 0; column < obj_1.matrix3d.column; column++) {
      ck_assert_double_eq_tol(obj_1.matrix3d.matrix[row][column],
                              obj_2.matrix3d.matrix[row][column], 1e-2);
    }
  }
}

object_t init_object_for_compare() {
  double matrix_array[][3] = {
      {1.000000, -1.000000, -1.000000}, {1.000000, -1.000000, 1.000000},
      {-1.000000, -1.000000, 1.000000}, {-1.000000, -1.000000, -1.000000},
      {1.000000, 1.000000, -1.000000},  {1.000000, 1.000000, 1.000000},
      {-1.000000, 1.000000, 1.000000},  {-1.000000, 1.000000, -1.000000},
  };

  matrix_t matrix = fill_matrix_from_array(
      matrix_array, sizeof(matrix_array) / sizeof(matrix_array[0]));

  int polygon_array[] = {2, 3, 4, 8, 7, 6, 5, 6, 2, 6, 7, 3, 3, 7, 8, 1, 4, 8,
                         1, 2, 4, 5, 8, 6, 1, 5, 2, 2, 6, 3, 4, 3, 8, 5, 1, 8};
  int polygon_size = sizeof(polygon_array) / sizeof(polygon_array[0]);

  polygon_t polygon = fill_polygon_from_array(polygon_array, polygon_size);
  object_t result = {matrix.rows, 0, matrix, polygon};
  return result;
}

START_TEST(parsing_test) {
  object_t real_object = parsing("objects/cube.obj");
  object_t object_for_compare = init_object_for_compare();

  object_compare(real_object, object_for_compare);
  remove_object(&real_object);
  remove_object(&object_for_compare);
}
END_TEST

START_TEST(zoom_matrix_test) {
  object_t real_object = parsing("objects/cube.obj");
  object_t object_for_compare = init_object_for_compare();
  int factor = 3;

  for (size_t row = 0; row < object_for_compare.matrix3d.rows; row++) {
    for (size_t column = 0; column < object_for_compare.matrix3d.column;
         column++) {
      object_for_compare.matrix3d.matrix[row][column] =
          object_for_compare.matrix3d.matrix[row][column] * factor;
    }
  }

  zoom_matrix(&real_object.matrix3d, factor);
  object_compare(real_object, object_for_compare);

  remove_object(&real_object);
  remove_object(&object_for_compare);
}
END_TEST

START_TEST(move_matrix_test) {
  object_t real_object = parsing("objects/cube.obj");
  object_t object_for_compare = init_object_for_compare();
  int offset = 3;
  int axis = Z;

  for (size_t row = 0; row < object_for_compare.matrix3d.rows; row++) {
    object_for_compare.matrix3d.matrix[row][axis] += offset;
  }

  move_matrix(&real_object.matrix3d, offset, axis);
  object_compare(real_object, object_for_compare);

  remove_object(&real_object);
  remove_object(&object_for_compare);
}
END_TEST

START_TEST(rotation_test) {
  object_t real_object = parsing("objects/cube.obj");
  object_t object_for_compare = init_object_for_compare();

  rotation_matrix(&real_object.matrix3d, 360 / 57.2958, X);
  rotation_matrix(&real_object.matrix3d, 360 / 57.2958, Y);
  rotation_matrix(&real_object.matrix3d, 360 / 57.2958, Z);
  object_compare(real_object, object_for_compare);

  remove_object(&real_object);
  remove_object(&object_for_compare);
}
END_TEST

Suite *s21_suite(void) {
  Suite *suite = suite_create("3DViewer");
  TCase *tcase_core = tcase_create("Tests");
  tcase_add_test(tcase_core, parsing_test);
  tcase_add_test(tcase_core, zoom_matrix_test);
  tcase_add_test(tcase_core, move_matrix_test);
  tcase_add_test(tcase_core, rotation_test);

  suite_add_tcase(suite, tcase_core);
  return suite;
}

int main(void) {
  Suite *suite = s21_suite();
  SRunner *suite_runner = srunner_create(suite);
  srunner_run_all(suite_runner, CK_NORMAL);
  int failed_count = srunner_ntests_failed(suite_runner);
  srunner_free(suite_runner);
  return (failed_count != 0) ? EXIT_FAILURE : EXIT_SUCCESS;
}
