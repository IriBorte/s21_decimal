#include "test.h"

// convertors tests
START_TEST(s21_from_decimal_to_int_pos) {
  srand(time(NULL));
  int n = rand();
  int dest = 0;
  s21_decimal var = {0};
  int ret1 = s21_from_int_to_decimal(n, &var);
  int ret2 = s21_from_decimal_to_int(var, &dest);
  ck_assert_int_eq(ret1, 0);
  ck_assert_int_eq(ret2, 0);
  ck_assert_int_eq(n, dest);
}
END_TEST

START_TEST(s21_from_decimal_to_int_neg) {
  srand(time(NULL));
  int n = -rand();
  int dest = 0;
  s21_decimal var = {0};
  int ret1 = s21_from_int_to_decimal(n, &var);
  int ret2 = s21_from_decimal_to_int(var, &dest);
  ck_assert_int_eq(ret1, 0);
  ck_assert_int_eq(ret2, 0);
  ck_assert_int_eq(n, dest);
}
END_TEST

START_TEST(s21_from_decimal_to_int_er) {
  int dest = 0;
  s21_decimal var = {0};
  int ret2 = s21_from_decimal_to_int(var, &dest);
  ck_assert_int_eq(ret2, 0);
}
END_TEST

START_TEST(s21_from_float_to_decimal_test_1) {
  float src = 123.5;
  s21_decimal dest = {0};
  uint32_t exp = 1 << 16;
  int ret = s21_from_float_to_decimal(src, &dest);
  ck_assert_int_eq(ret, 0);
  ck_assert_uint_eq(dest.bits[0], 1235);
  ck_assert_uint_eq(dest.bits[1], 0);
  ck_assert_uint_eq(dest.bits[2], 0);
  ck_assert_uint_eq(dest.bits[3], exp);
}
END_TEST

START_TEST(s21_from_float_to_decimal_test_2) {
  float src = 123.45;
  s21_decimal dest = {0};
  uint32_t exp = 2 << 16;
  int ret = s21_from_float_to_decimal(src, &dest);
  ck_assert_int_eq(ret, 0);
  ck_assert_uint_eq(dest.bits[0], 12345);
  ck_assert_uint_eq(dest.bits[1], 0);
  ck_assert_uint_eq(dest.bits[2], 0);
  ck_assert_uint_eq(dest.bits[3], exp);
}
END_TEST

START_TEST(s21_from_float_to_decimal_test_3) {
  double src = -123.45;
  s21_decimal dest = {0};
  uint32_t exp = 2 << 16;
  int ret = s21_from_float_to_decimal(src, &dest);
  ck_assert_int_eq(ret, 0);
  ck_assert_uint_eq(dest.bits[0], 12345);
  ck_assert_uint_eq(dest.bits[1], 0);
  ck_assert_uint_eq(dest.bits[2], 0);
  ck_assert_uint_eq(dest.bits[3], exp | MINUS);
}
END_TEST

START_TEST(s21_from_decimal_to_float_test_1) {
  uint32_t exp = 2 << 16;
  s21_decimal src = {{12345, 0, 0, exp}};
  float dest = 0;
  int ret = s21_from_decimal_to_float(src, &dest);
  ck_assert_int_eq(ret, 0);
  ck_assert_float_eq(dest, 123.45);
}
END_TEST

START_TEST(from_int_to_decimal_1) {
  int a = 7;
  s21_decimal b = {{0, 0, 0, MINUS}};
  int result = s21_from_int_to_decimal(a, &b);
  ck_assert_int_eq(result, 0);
}
END_TEST

Suite *convertors_suite(void) {
  Suite *s;
  TCase *tc_parsers;

  s = suite_create("Convertors");

  tc_parsers = tcase_create("decimal to int");

  // ---------- Convertors and parsers ----------
  tcase_add_test(tc_parsers, s21_from_decimal_to_int_pos);
  tcase_add_test(tc_parsers, s21_from_decimal_to_int_neg);
  tcase_add_test(tc_parsers, s21_from_decimal_to_int_er);
  tcase_add_test(tc_parsers, s21_from_float_to_decimal_test_1);
  tcase_add_test(tc_parsers, s21_from_float_to_decimal_test_2);
  tcase_add_test(tc_parsers, s21_from_float_to_decimal_test_3);
  tcase_add_test(tc_parsers, s21_from_decimal_to_float_test_1);
  tcase_add_test(tc_parsers, s21_from_decimal_to_float_test_1);
  tcase_add_test(tc_parsers, from_int_to_decimal_1);

  suite_add_tcase(s, tc_parsers);

  return s;
}
