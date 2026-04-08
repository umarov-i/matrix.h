#include <math.h>
#include <stdlib.h>

#include "s21_matrix.h"

int s21_inverse_matrix(matrix_t *A, matrix_t *result) {
  if (A == NULL || result == NULL) return S21_RESULT_ERROR;
  if (A->rows != A->columns) return S21_RESULT_ERROR;

  double det = 0.0;
  if (s21_determinant(A, &det) != S21_RESULT_OK) return S21_RESULT_ERROR;
  if (fabs(det) < 1e-7) return S21_RESULT_ERROR;

  if (A->rows == 1) {
    s21_create_matrix(1, 1, result);
    result->matrix[0][0] = 1.0 / A->matrix[0][0];
    return S21_RESULT_OK;
  }

  matrix_t cofactors;
  if (s21_calc_complements(A, &cofactors) != S21_RESULT_OK)
    return S21_RESULT_ERROR;

  matrix_t adj;
  if (s21_transpose(&cofactors, &adj) != S21_RESULT_OK) {
    s21_remove_matrix(&cofactors);
    return S21_RESULT_ERROR;
  }

  s21_mult_number(&adj, 1.0 / det, result);
  s21_remove_matrix(&cofactors);
  s21_remove_matrix(&adj);

  return S21_RESULT_OK;
}
