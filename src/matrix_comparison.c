#include <math.h>
#include <stdlib.h>

#include "s21_matrix.h"
#define SUCCESS 1
#define FAILURE 0
#define EPSILON 1e-7

int s21_eq_matrix(matrix_t *A, matrix_t *B) {
  if (A == NULL || B == NULL) return FAILURE;
  if (A->matrix == NULL || B->matrix == NULL) return FAILURE;
  if (A->rows != B->rows || A->columns != B->columns) return FAILURE;

  for (int i = 0; i < A->rows; i++) {
    for (int j = 0; j < A->columns; j++) {
      if (fabs(A->matrix[i][j] - B->matrix[i][j]) > EPSILON) return FAILURE;
    }
  }
  return SUCCESS;
}
