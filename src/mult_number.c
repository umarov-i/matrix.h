#include <stdlib.h>

#include "s21_matrix.h"

int s21_mult_number(matrix_t *A, double number, matrix_t *result) {
  if (A == NULL || result == NULL) return S21_RESULT_ERROR;
  if (A->matrix == NULL) return S21_RESULT_ERROR;

  int create_result = s21_create_matrix(A->rows, A->columns, result);
  if (create_result != S21_RESULT_OK) return S21_RESULT_ERROR;

  for (int i = 0; i < A->rows; i++) {
    for (int j = 0; j < A->columns; j++) {
      result->matrix[i][j] = A->matrix[i][j] * number;
    }
  }
  return S21_RESULT_OK;
}
