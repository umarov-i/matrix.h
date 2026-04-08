#include <math.h>
#include <stdlib.h>

#include "s21_matrix.h"

int s21_determinant(matrix_t *A, double *result) {
  if (A == NULL || A->matrix == NULL || result == NULL) return S21_RESULT_ERROR;

  if (A->rows != A->columns) return S21_CALC_ERROR;

  if (A->rows == 1) {
    *result = A->matrix[0][0];
    return S21_RESULT_OK;
  }

  if (A->rows == 2) {
    *result = (A->matrix[0][0] * A->matrix[1][1]) -
              (A->matrix[0][1] * A->matrix[1][0]);
    return S21_RESULT_OK;
  }

  int code = S21_RESULT_OK;
  double det = 0.0;
  matrix_t minor_matrix;

  for (int j = 0; j < A->columns; j++) {
    if (s21_create_matrix(A->rows - 1, A->columns - 1, &minor_matrix) !=
        S21_RESULT_OK)
      return S21_RESULT_ERROR;

    for (int row = 1; row < A->rows; row++) {
      int m_col = 0;
      for (int col = 0; col < A->columns; col++) {
        if (col == j) {
          continue;
        }
        minor_matrix.matrix[row - 1][m_col] = A->matrix[row][col];
        m_col++;
      }
    }

    double minor_det = 0.0;
    code = s21_determinant(&minor_matrix, &minor_det);

    if (code != S21_RESULT_OK) {
      s21_remove_matrix(&minor_matrix);
      return code;
    }

    if (j % 2 == 0) {
      det += A->matrix[0][j] * minor_det;
    } else {
      det -= A->matrix[0][j] * minor_det;
    }

    s21_remove_matrix(&minor_matrix);
  }

  *result = det;
  return S21_RESULT_OK;
}