#include <stdlib.h>

#include "s21_matrix.h"

int s21_create_matrix(int rows, int columns, matrix_t *result) {
  if (rows <= 0 || columns <= 0 || result == NULL) {
    return S21_RESULT_ERROR;
  }

  result->matrix = malloc(sizeof(double *) * rows);
  if (result->matrix == NULL) {
    return S21_RESULT_ERROR;
  }

  for (int i = 0; i < rows; i++) {
    result->matrix[i] = malloc(sizeof(double) * columns);
    if (result->matrix[i] == NULL) {
      for (int j = 0; j < i; j++) {
        free(result->matrix[j]);
      }
      free(result->matrix);
      result->matrix = NULL;
      return S21_RESULT_ERROR;
    }
  }

  result->rows = rows;
  result->columns = columns;
  return S21_RESULT_OK;
}
