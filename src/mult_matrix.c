#include <stdlib.h>

#include "s21_matrix.h"

int s21_mult_matrix(matrix_t *A, matrix_t *B, matrix_t *result) {
  if (A == NULL || B == NULL || result == NULL) return S21_RESULT_ERROR;
  if (A->matrix == NULL || B->matrix == NULL) return S21_RESULT_ERROR;
  if (A->columns != B->rows) return S21_RESULT_ERROR;

  int create_result = s21_create_matrix(A->rows, B->columns, result);
  if (create_result != S21_RESULT_OK) return S21_RESULT_ERROR;

  for (int i = 0; i < A->rows; i++) {
    for (int j = 0; j < B->columns; j++) {
      double sum = 0;
      for (int k = 0; k < A->columns; k++) {
        sum += A->matrix[i][k] * B->matrix[k][j];
      }
      result->matrix[i][j] = sum;
    }
  }

  return S21_RESULT_OK;
}
