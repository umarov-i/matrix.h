#include <stdlib.h>

#include "s21_matrix.h"

int s21_transpose(matrix_t *A, matrix_t *result) {
  if (A == NULL || result == NULL) return S21_RESULT_ERROR;
  if (A->matrix == NULL) return S21_RESULT_ERROR;

  int create_result = s21_create_matrix(A->columns, A->rows, result);
  if (create_result != S21_RESULT_OK) return S21_RESULT_ERROR;

  for (int i = 0; i < A->rows; i++) {
    for (int j = 0; j < A->columns; j++) {
      result->matrix[j][i] = A->matrix[i][j];
    }
  }

  return S21_RESULT_OK;
}
