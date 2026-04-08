#include <check.h>
#include <math.h>

#include "s21_matrix.h"

// Helper: create a test 3x3 matrix
matrix_t make_3x3(void) {
  matrix_t m;
  s21_create_matrix(3, 3, &m);
  double vals[3][3] = {{1, 2, 3}, {0, 4, 5}, {1, 0, 6}};
  for (int i = 0; i < 3; i++)
    for (int j = 0; j < 3; j++) m.matrix[i][j] = vals[i][j];
  return m;
}

// Helper: create 1x1 matrix
matrix_t make_1x1(double val) {
  matrix_t m;
  s21_create_matrix(1, 1, &m);
  m.matrix[0][0] = val;
  return m;
}

// Helper: create 2x2 identity matrix
matrix_t make_identity_2x2(void) {
  matrix_t m;
  s21_create_matrix(2, 2, &m);
  m.matrix[0][0] = 1;
  m.matrix[0][1] = 0;
  m.matrix[1][0] = 0;
  m.matrix[1][1] = 1;
  return m;
}

// Helper: create zero matrix
matrix_t make_zero_3x3(void) {
  matrix_t m;
  s21_create_matrix(3, 3, &m);
  for (int i = 0; i < 3; i++)
    for (int j = 0; j < 3; j++) m.matrix[i][j] = 0.0;
  return m;
}

/* --- CREATE & REMOVE TESTS --- */
START_TEST(test_create_remove_ok) {
  matrix_t m;
  ck_assert_int_eq(s21_create_matrix(2, 2, &m), S21_RESULT_OK);
  ck_assert_ptr_nonnull(m.matrix);
  s21_remove_matrix(&m);
}
END_TEST

START_TEST(test_create_invalid) {
  matrix_t m;
  ck_assert_int_eq(s21_create_matrix(0, 3, &m), S21_RESULT_ERROR);
  ck_assert_int_eq(s21_create_matrix(3, 0, &m), S21_RESULT_ERROR);
  ck_assert_int_eq(s21_create_matrix(-1, 3, &m), S21_RESULT_ERROR);
  ck_assert_int_eq(s21_create_matrix(3, -1, &m), S21_RESULT_ERROR);
}
END_TEST

START_TEST(test_create_null_result) {
  ck_assert_int_eq(s21_create_matrix(2, 2, NULL), S21_RESULT_ERROR);
}
END_TEST

START_TEST(test_create_1x1) {
  matrix_t m;
  ck_assert_int_eq(s21_create_matrix(1, 1, &m), S21_RESULT_OK);
  ck_assert_int_eq(m.rows, 1);
  ck_assert_int_eq(m.columns, 1);
  s21_remove_matrix(&m);
}
END_TEST

START_TEST(test_create_large_matrix) {
  matrix_t m;
  ck_assert_int_eq(s21_create_matrix(100, 100, &m), S21_RESULT_OK);
  ck_assert_int_eq(m.rows, 100);
  ck_assert_int_eq(m.columns, 100);
  s21_remove_matrix(&m);
}
END_TEST

START_TEST(test_remove_null) {
  s21_remove_matrix(NULL);  // Should not crash
}
END_TEST

/* --- EQUALITY TESTS --- */
START_TEST(test_eq_true_false) {
  matrix_t m1 = make_3x3();
  matrix_t m2 = make_3x3();
  ck_assert_int_eq(s21_eq_matrix(&m1, &m2), 1);
  m2.matrix[0][0] = 99;
  ck_assert_int_eq(s21_eq_matrix(&m1, &m2), 0);
  s21_remove_matrix(&m1);
  s21_remove_matrix(&m2);
}
END_TEST

START_TEST(test_eq_null_matrices) {
  matrix_t m = make_3x3();
  ck_assert_int_eq(s21_eq_matrix(NULL, &m), 0);
  ck_assert_int_eq(s21_eq_matrix(&m, NULL), 0);
  ck_assert_int_eq(s21_eq_matrix(NULL, NULL), 0);
  s21_remove_matrix(&m);
}
END_TEST

START_TEST(test_eq_different_sizes) {
  matrix_t m1, m2;
  s21_create_matrix(2, 2, &m1);
  s21_create_matrix(3, 3, &m2);
  ck_assert_int_eq(s21_eq_matrix(&m1, &m2), 0);
  s21_remove_matrix(&m1);
  s21_remove_matrix(&m2);
}
END_TEST

START_TEST(test_eq_floating_point_precision) {
  matrix_t m1, m2;
  s21_create_matrix(2, 2, &m1);
  s21_create_matrix(2, 2, &m2);
  m1.matrix[0][0] = 1.0000001;
  m2.matrix[0][0] = 1.0000002;
  m1.matrix[0][1] = m1.matrix[1][0] = m1.matrix[1][1] = 0.0;
  m2.matrix[0][1] = m2.matrix[1][0] = m2.matrix[1][1] = 0.0;
  ck_assert_int_eq(s21_eq_matrix(&m1, &m2), 1);  // Within epsilon
  s21_remove_matrix(&m1);
  s21_remove_matrix(&m2);
}
END_TEST

/* --- SUM TESTS --- */
START_TEST(test_sum_ok) {
  matrix_t m1 = make_3x3();
  matrix_t m2 = make_3x3();
  matrix_t res;
  ck_assert_int_eq(s21_sum_matrix(&m1, &m2, &res), S21_RESULT_OK);
  ck_assert_double_eq(res.matrix[0][0], 2);
  s21_remove_matrix(&m1);
  s21_remove_matrix(&m2);
  s21_remove_matrix(&res);
}
END_TEST

START_TEST(test_sum_invalid_size) {
  matrix_t m1, m2, res;
  s21_create_matrix(2, 2, &m1);
  s21_create_matrix(3, 3, &m2);
  ck_assert_int_eq(s21_sum_matrix(&m1, &m2, &res), S21_RESULT_ERROR);
  s21_remove_matrix(&m1);
  s21_remove_matrix(&m2);
}
END_TEST

START_TEST(test_sum_null_inputs) {
  matrix_t m = make_3x3();
  matrix_t res;
  ck_assert_int_eq(s21_sum_matrix(NULL, &m, &res), S21_RESULT_ERROR);
  ck_assert_int_eq(s21_sum_matrix(&m, NULL, &res), S21_RESULT_ERROR);
  ck_assert_int_eq(s21_sum_matrix(&m, &m, NULL), S21_RESULT_ERROR);
  s21_remove_matrix(&m);
}
END_TEST

START_TEST(test_sum_1x1) {
  matrix_t m1 = make_1x1(5.0);
  matrix_t m2 = make_1x1(3.0);
  matrix_t res;
  ck_assert_int_eq(s21_sum_matrix(&m1, &m2, &res), S21_RESULT_OK);
  ck_assert_double_eq(res.matrix[0][0], 8.0);
  s21_remove_matrix(&m1);
  s21_remove_matrix(&m2);
  s21_remove_matrix(&res);
}
END_TEST

/* --- SUB TESTS --- */
START_TEST(test_sub_ok) {
  matrix_t m1 = make_3x3();
  matrix_t m2 = make_3x3();
  matrix_t res;
  ck_assert_int_eq(s21_sub_matrix(&m1, &m2, &res), S21_RESULT_OK);
  ck_assert_double_eq(res.matrix[0][0], 0);
  s21_remove_matrix(&m1);
  s21_remove_matrix(&m2);
  s21_remove_matrix(&res);
}
END_TEST

START_TEST(test_sub_null_inputs) {
  matrix_t m = make_3x3();
  matrix_t res;
  ck_assert_int_eq(s21_sub_matrix(NULL, &m, &res), S21_RESULT_ERROR);
  ck_assert_int_eq(s21_sub_matrix(&m, NULL, &res), S21_RESULT_ERROR);
  ck_assert_int_eq(s21_sub_matrix(&m, &m, NULL), S21_RESULT_ERROR);
  s21_remove_matrix(&m);
}
END_TEST

START_TEST(test_sub_negative_result) {
  matrix_t m1 = make_1x1(3.0);
  matrix_t m2 = make_1x1(5.0);
  matrix_t res;
  ck_assert_int_eq(s21_sub_matrix(&m1, &m2, &res), S21_RESULT_OK);
  ck_assert_double_eq(res.matrix[0][0], -2.0);
  s21_remove_matrix(&m1);
  s21_remove_matrix(&m2);
  s21_remove_matrix(&res);
}
END_TEST

/* --- MULT NUMBER TESTS --- */
START_TEST(test_mult_number_ok) {
  matrix_t m = make_3x3();
  matrix_t res;
  ck_assert_int_eq(s21_mult_number(&m, 2.0, &res), S21_RESULT_OK);
  ck_assert_double_eq(res.matrix[0][0], 2);
  s21_remove_matrix(&m);
  s21_remove_matrix(&res);
}
END_TEST

START_TEST(test_mult_number_zero) {
  matrix_t m = make_3x3();
  matrix_t res;
  ck_assert_int_eq(s21_mult_number(&m, 0.0, &res), S21_RESULT_OK);
  ck_assert_double_eq(res.matrix[0][0], 0.0);
  ck_assert_double_eq(res.matrix[1][1], 0.0);
  s21_remove_matrix(&m);
  s21_remove_matrix(&res);
}
END_TEST

START_TEST(test_mult_number_negative) {
  matrix_t m = make_1x1(5.0);
  matrix_t res;
  ck_assert_int_eq(s21_mult_number(&m, -2.0, &res), S21_RESULT_OK);
  ck_assert_double_eq(res.matrix[0][0], -10.0);
  s21_remove_matrix(&m);
  s21_remove_matrix(&res);
}
END_TEST

START_TEST(test_mult_number_null) {
  matrix_t m = make_3x3();
  matrix_t res;
  ck_assert_int_eq(s21_mult_number(NULL, 2.0, &res), S21_RESULT_ERROR);
  ck_assert_int_eq(s21_mult_number(&m, 2.0, NULL), S21_RESULT_ERROR);
  s21_remove_matrix(&m);
}
END_TEST

/* --- MULT MATRIX TESTS --- */
START_TEST(test_mult_matrix_ok) {
  matrix_t m1 = make_3x3();
  matrix_t m2 = make_3x3();
  matrix_t res;
  ck_assert_int_eq(s21_mult_matrix(&m1, &m2, &res), S21_RESULT_OK);
  ck_assert_int_eq(res.rows, 3);
  ck_assert_int_eq(res.columns, 3);
  s21_remove_matrix(&m1);
  s21_remove_matrix(&m2);
  s21_remove_matrix(&res);
}
END_TEST

START_TEST(test_mult_matrix_invalid) {
  matrix_t m1, m2, res;
  s21_create_matrix(2, 3, &m1);
  s21_create_matrix(2, 2, &m2);
  ck_assert_int_eq(s21_mult_matrix(&m1, &m2, &res), S21_RESULT_ERROR);
  s21_remove_matrix(&m1);
  s21_remove_matrix(&m2);
}
END_TEST

START_TEST(test_mult_matrix_identity) {
  matrix_t m = make_identity_2x2();
  matrix_t id = make_identity_2x2();
  matrix_t res;
  ck_assert_int_eq(s21_mult_matrix(&m, &id, &res), S21_RESULT_OK);
  ck_assert_double_eq(res.matrix[0][0], 1.0);
  ck_assert_double_eq(res.matrix[1][1], 1.0);
  s21_remove_matrix(&m);
  s21_remove_matrix(&id);
  s21_remove_matrix(&res);
}
END_TEST

START_TEST(test_mult_matrix_1x1) {
  matrix_t m1 = make_1x1(3.0);
  matrix_t m2 = make_1x1(4.0);
  matrix_t res;
  ck_assert_int_eq(s21_mult_matrix(&m1, &m2, &res), S21_RESULT_OK);
  ck_assert_double_eq(res.matrix[0][0], 12.0);
  s21_remove_matrix(&m1);
  s21_remove_matrix(&m2);
  s21_remove_matrix(&res);
}
END_TEST

START_TEST(test_mult_matrix_null) {
  matrix_t m = make_3x3();
  matrix_t res;
  ck_assert_int_eq(s21_mult_matrix(NULL, &m, &res), S21_RESULT_ERROR);
  ck_assert_int_eq(s21_mult_matrix(&m, NULL, &res), S21_RESULT_ERROR);
  ck_assert_int_eq(s21_mult_matrix(&m, &m, NULL), S21_RESULT_ERROR);
  s21_remove_matrix(&m);
}
END_TEST

/* --- TRANSPOSE TESTS --- */
START_TEST(test_transpose_ok) {
  matrix_t m = make_3x3();
  matrix_t res;
  ck_assert_int_eq(s21_transpose(&m, &res), S21_RESULT_OK);
  ck_assert_double_eq(res.matrix[0][1], m.matrix[1][0]);
  ck_assert_int_eq(res.rows, 3);
  ck_assert_int_eq(res.columns, 3);
  s21_remove_matrix(&m);
  s21_remove_matrix(&res);
}
END_TEST

START_TEST(test_transpose_rectangular) {
  matrix_t m, res;
  s21_create_matrix(2, 3, &m);
  m.matrix[0][0] = 1;
  m.matrix[0][1] = 2;
  m.matrix[0][2] = 3;
  m.matrix[1][0] = 4;
  m.matrix[1][1] = 5;
  m.matrix[1][2] = 6;

  ck_assert_int_eq(s21_transpose(&m, &res), S21_RESULT_OK);
  ck_assert_int_eq(res.rows, 3);
  ck_assert_int_eq(res.columns, 2);
  ck_assert_double_eq(res.matrix[0][0], 1);
  ck_assert_double_eq(res.matrix[1][0], 2);
  ck_assert_double_eq(res.matrix[2][1], 6);

  s21_remove_matrix(&m);
  s21_remove_matrix(&res);
}
END_TEST

START_TEST(test_transpose_1x1) {
  matrix_t m = make_1x1(42.0);
  matrix_t res;
  ck_assert_int_eq(s21_transpose(&m, &res), S21_RESULT_OK);
  ck_assert_double_eq(res.matrix[0][0], 42.0);
  s21_remove_matrix(&m);
  s21_remove_matrix(&res);
}
END_TEST

START_TEST(test_transpose_null) {
  matrix_t m = make_3x3();
  matrix_t res;
  ck_assert_int_eq(s21_transpose(NULL, &res), S21_RESULT_ERROR);
  ck_assert_int_eq(s21_transpose(&m, NULL), S21_RESULT_ERROR);
  s21_remove_matrix(&m);
}
END_TEST

/* --- CALC COMPLEMENTS TESTS --- */
START_TEST(test_calc_complements_ok) {
  matrix_t m = make_3x3();
  matrix_t res;
  ck_assert_int_eq(s21_calc_complements(&m, &res), S21_RESULT_OK);
  s21_remove_matrix(&m);
  s21_remove_matrix(&res);
}
END_TEST

START_TEST(test_calc_complements_invalid) {
  matrix_t m, res;
  s21_create_matrix(2, 3, &m);
  ck_assert_int_eq(s21_calc_complements(&m, &res), S21_CALC_ERROR);
  s21_remove_matrix(&m);
}
END_TEST

START_TEST(test_calc_complements_1x1) {
  matrix_t m, res;
  s21_create_matrix(1, 1, &m);
  ck_assert_int_eq(s21_calc_complements(&m, &res),
                   S21_CALC_ERROR);  // Should fail for 1x1
  s21_remove_matrix(&m);
}
END_TEST

START_TEST(test_calc_complements_2x2) {
  matrix_t m, res;
  s21_create_matrix(2, 2, &m);
  m.matrix[0][0] = 1;
  m.matrix[0][1] = 2;
  m.matrix[1][0] = 3;
  m.matrix[1][1] = 4;

  ck_assert_int_eq(s21_calc_complements(&m, &res), S21_RESULT_OK);
  ck_assert_double_eq(res.matrix[0][0], 4);   // +det([[4]])
  ck_assert_double_eq(res.matrix[0][1], -3);  // -det([[3]])
  ck_assert_double_eq(res.matrix[1][0], -2);  // -det([[2]])
  ck_assert_double_eq(res.matrix[1][1], 1);   // +det([[1]])

  s21_remove_matrix(&m);
  s21_remove_matrix(&res);
}
END_TEST

START_TEST(test_calc_complements_null) {
  matrix_t m = make_3x3();
  matrix_t res;
  ck_assert_int_eq(s21_calc_complements(NULL, &res), S21_RESULT_ERROR);
  ck_assert_int_eq(s21_calc_complements(&m, NULL), S21_RESULT_ERROR);
  s21_remove_matrix(&m);
}
END_TEST

/* --- DETERMINANT TESTS --- */
START_TEST(test_determinant_ok) {
  matrix_t m = make_3x3();
  double det;
  ck_assert_int_eq(s21_determinant(&m, &det), S21_RESULT_OK);
  ck_assert(fabs(det - 22) < 1e-6);
  s21_remove_matrix(&m);
}
END_TEST

START_TEST(test_determinant_invalid) {
  matrix_t m;
  s21_create_matrix(2, 3, &m);
  double det;
  ck_assert_int_eq(s21_determinant(&m, &det), S21_CALC_ERROR);
  s21_remove_matrix(&m);
}
END_TEST

START_TEST(test_determinant_1x1) {
  matrix_t m = make_1x1(42.0);
  double det;
  ck_assert_int_eq(s21_determinant(&m, &det), S21_RESULT_OK);
  ck_assert_double_eq(det, 42.0);
  s21_remove_matrix(&m);
}
END_TEST

START_TEST(test_determinant_2x2) {
  matrix_t m;
  s21_create_matrix(2, 2, &m);
  m.matrix[0][0] = 1;
  m.matrix[0][1] = 2;
  m.matrix[1][0] = 3;
  m.matrix[1][1] = 4;

  double det;
  ck_assert_int_eq(s21_determinant(&m, &det), S21_RESULT_OK);
  ck_assert_double_eq(det, -2.0);  // 1*4 - 2*3 = -2
  s21_remove_matrix(&m);
}
END_TEST

START_TEST(test_determinant_zero) {
  matrix_t m = make_zero_3x3();
  double det;
  ck_assert_int_eq(s21_determinant(&m, &det), S21_RESULT_OK);
  ck_assert_double_eq(det, 0.0);
  s21_remove_matrix(&m);
}
END_TEST

START_TEST(test_determinant_null) {
  matrix_t m = make_3x3();
  double det;
  ck_assert_int_eq(s21_determinant(NULL, &det), S21_RESULT_ERROR);
  ck_assert_int_eq(s21_determinant(&m, NULL), S21_RESULT_ERROR);
  s21_remove_matrix(&m);
}
END_TEST

/* --- INVERSE TESTS --- */
START_TEST(test_inverse_ok) {
  matrix_t m = make_3x3();
  matrix_t inv;
  ck_assert_int_eq(s21_inverse_matrix(&m, &inv), S21_RESULT_OK);
  s21_remove_matrix(&m);
  s21_remove_matrix(&inv);
}
END_TEST

START_TEST(test_inverse_singular) {
  matrix_t m;
  s21_create_matrix(2, 2, &m);
  m.matrix[0][0] = 1;
  m.matrix[0][1] = 2;
  m.matrix[1][0] = 2;
  m.matrix[1][1] = 4;  // det = 0
  matrix_t inv;
  ck_assert_int_eq(s21_inverse_matrix(&m, &inv), S21_RESULT_ERROR);
  s21_remove_matrix(&m);
}
END_TEST

START_TEST(test_inverse_1x1) {
  matrix_t m = make_1x1(5.0);
  matrix_t inv;
  ck_assert_int_eq(s21_inverse_matrix(&m, &inv), S21_RESULT_OK);
  ck_assert_double_eq_tol(inv.matrix[0][0], 0.2, 1e-6);  // 1/5 = 0.2
  s21_remove_matrix(&m);
  s21_remove_matrix(&inv);
}
END_TEST

START_TEST(test_inverse_identity) {
  matrix_t m = make_identity_2x2();
  matrix_t inv;
  ck_assert_int_eq(s21_inverse_matrix(&m, &inv), S21_RESULT_OK);
  ck_assert_double_eq_tol(inv.matrix[0][0], 1.0, 1e-6);
  ck_assert_double_eq_tol(inv.matrix[1][1], 1.0, 1e-6);
  ck_assert_double_eq_tol(inv.matrix[0][1], 0.0, 1e-6);
  ck_assert_double_eq_tol(inv.matrix[1][0], 0.0, 1e-6);
  s21_remove_matrix(&m);
  s21_remove_matrix(&inv);
}
END_TEST

START_TEST(test_inverse_null) {
  matrix_t m = make_3x3();
  matrix_t inv;
  ck_assert_int_eq(s21_inverse_matrix(NULL, &inv), S21_RESULT_ERROR);
  ck_assert_int_eq(s21_inverse_matrix(&m, NULL), S21_RESULT_ERROR);
  s21_remove_matrix(&m);
}
END_TEST

START_TEST(test_inverse_non_square) {
  matrix_t m, inv;
  s21_create_matrix(2, 3, &m);
  ck_assert_int_eq(s21_inverse_matrix(&m, &inv), S21_RESULT_ERROR);
  s21_remove_matrix(&m);
}
END_TEST

Suite *matrix_suite(void) {
  Suite *s = suite_create("Matrix");
  TCase *tc_create = tcase_create("Create/Remove");
  TCase *tc_eq = tcase_create("Equality");
  TCase *tc_sum = tcase_create("Sum");
  TCase *tc_sub = tcase_create("Sub");
  TCase *tc_mult_num = tcase_create("MultNumber");
  TCase *tc_mult_mat = tcase_create("MultMatrix");
  TCase *tc_transpose = tcase_create("Transpose");
  TCase *tc_complements = tcase_create("Complements");
  TCase *tc_determinant = tcase_create("Determinant");
  TCase *tc_inverse = tcase_create("Inverse");

  // Create/Remove tests
  tcase_add_test(tc_create, test_create_remove_ok);
  tcase_add_test(tc_create, test_create_invalid);
  tcase_add_test(tc_create, test_create_null_result);
  tcase_add_test(tc_create, test_create_1x1);
  tcase_add_test(tc_create, test_create_large_matrix);
  tcase_add_test(tc_create, test_remove_null);

  // Equality tests
  tcase_add_test(tc_eq, test_eq_true_false);
  tcase_add_test(tc_eq, test_eq_null_matrices);
  tcase_add_test(tc_eq, test_eq_different_sizes);
  tcase_add_test(tc_eq, test_eq_floating_point_precision);

  // Sum tests
  tcase_add_test(tc_sum, test_sum_ok);
  tcase_add_test(tc_sum, test_sum_invalid_size);
  tcase_add_test(tc_sum, test_sum_null_inputs);
  tcase_add_test(tc_sum, test_sum_1x1);

  // Sub tests
  tcase_add_test(tc_sub, test_sub_ok);
  tcase_add_test(tc_sub, test_sub_null_inputs);
  tcase_add_test(tc_sub, test_sub_negative_result);

  // Mult number tests
  tcase_add_test(tc_mult_num, test_mult_number_ok);
  tcase_add_test(tc_mult_num, test_mult_number_zero);
  tcase_add_test(tc_mult_num, test_mult_number_negative);
  tcase_add_test(tc_mult_num, test_mult_number_null);

  // Mult matrix tests
  tcase_add_test(tc_mult_mat, test_mult_matrix_ok);
  tcase_add_test(tc_mult_mat, test_mult_matrix_invalid);
  tcase_add_test(tc_mult_mat, test_mult_matrix_identity);
  tcase_add_test(tc_mult_mat, test_mult_matrix_1x1);
  tcase_add_test(tc_mult_mat, test_mult_matrix_null);

  // Transpose tests
  tcase_add_test(tc_transpose, test_transpose_ok);
  tcase_add_test(tc_transpose, test_transpose_rectangular);
  tcase_add_test(tc_transpose, test_transpose_1x1);
  tcase_add_test(tc_transpose, test_transpose_null);

  // Complements tests
  tcase_add_test(tc_complements, test_calc_complements_ok);
  tcase_add_test(tc_complements, test_calc_complements_invalid);
  tcase_add_test(tc_complements, test_calc_complements_1x1);
  tcase_add_test(tc_complements, test_calc_complements_2x2);
  tcase_add_test(tc_complements, test_calc_complements_null);

  // Determinant tests
  tcase_add_test(tc_determinant, test_determinant_ok);
  tcase_add_test(tc_determinant, test_determinant_invalid);
  tcase_add_test(tc_determinant, test_determinant_1x1);
  tcase_add_test(tc_determinant, test_determinant_2x2);
  tcase_add_test(tc_determinant, test_determinant_zero);
  tcase_add_test(tc_determinant, test_determinant_null);

  // Inverse tests
  tcase_add_test(tc_inverse, test_inverse_ok);
  tcase_add_test(tc_inverse, test_inverse_singular);
  tcase_add_test(tc_inverse, test_inverse_1x1);
  tcase_add_test(tc_inverse, test_inverse_identity);
  tcase_add_test(tc_inverse, test_inverse_null);
  tcase_add_test(tc_inverse, test_inverse_non_square);

  suite_add_tcase(s, tc_create);
  suite_add_tcase(s, tc_eq);
  suite_add_tcase(s, tc_sum);
  suite_add_tcase(s, tc_sub);
  suite_add_tcase(s, tc_mult_num);
  suite_add_tcase(s, tc_mult_mat);
  suite_add_tcase(s, tc_transpose);
  suite_add_tcase(s, tc_complements);
  suite_add_tcase(s, tc_determinant);
  suite_add_tcase(s, tc_inverse);

  return s;
}

int main(void) {
  int no_failed = 0;
  Suite *s = matrix_suite();
  SRunner *runner = srunner_create(s);
  srunner_run_all(runner, CK_NORMAL);
  no_failed = srunner_ntests_failed(runner);
  srunner_free(runner);
  return (no_failed == 0) ? 0 : 1;
}
