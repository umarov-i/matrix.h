#include <stdlib.h>

#include "s21_matrix.h"

int s21_sum_matrix(matrix_t *A, matrix_t *B, matrix_t *result) {
  if (A == NULL || B == NULL || result == NULL) return S21_RESULT_ERROR;
  if (A->matrix == NULL || B->matrix == NULL) return S21_RESULT_ERROR;
  if (A->rows != B->rows || A->columns != B->columns) return S21_RESULT_ERROR;

  int create_result = s21_create_matrix(B->rows, B->columns, result);
  if (create_result != S21_RESULT_OK) return S21_RESULT_ERROR;

  for (int i = 0; i < B->rows; i++) {
    for (int j = 0; j < B->columns; j++) {
      result->matrix[i][j] = A->matrix[i][j] + B->matrix[i][j];
    }
  }
  return S21_RESULT_OK;
}
