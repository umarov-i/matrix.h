#include <math.h>
#include <stdlib.h>

#include "s21_matrix.h"

void s21_get_minor(int row, int col, matrix_t *A, matrix_t *result) {
  int m_row = 0;
  int m_col = 0;

  for (int i = 0; i < A->rows; i++) {
    if (i == row) {
      continue;
    }
    m_col = 0;
    for (int j = 0; j < A->columns; j++) {
      if (j == col) {
        continue;
      }
      result->matrix[m_row][m_col] = A->matrix[i][j];
      m_col++;
    }
    m_row++;
  }
}

int s21_calc_complements(matrix_t *A, matrix_t *result) {
  if (A == NULL || result == NULL || A->matrix == NULL) return S21_RESULT_ERROR;
  if (A->rows != A->columns) return S21_CALC_ERROR;
  if (A->rows < 2) return S21_CALC_ERROR;

  int create_code = s21_create_matrix(A->rows, A->columns, result);
  if (create_code != S21_RESULT_OK) return S21_RESULT_ERROR;

  matrix_t minor_matrix;
  double determinant = 0.0;

  for (int i = 0; i < A->rows; i++) {
    for (int j = 0; j < A->columns; j++) {
      if (s21_create_matrix(A->rows - 1, A->columns - 1, &minor_matrix) !=
          S21_RESULT_OK) {
        s21_remove_matrix(result);
        return S21_RESULT_ERROR;
      }

      s21_get_minor(i, j, A, &minor_matrix);
      s21_determinant(&minor_matrix, &determinant);

      double sign = ((i + j) % 2 == 0) ? 1.0 : -1.0;
      result->matrix[i][j] = determinant * sign;

      s21_remove_matrix(&minor_matrix);
    }
  }

  return S21_RESULT_OK;
}