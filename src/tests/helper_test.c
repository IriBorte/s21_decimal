#include "test.h"

// Проверка маски минуса
START_TEST(minus_test) {
  unsigned int n = 1 << 31;
  ck_assert_uint_eq(n, MINUS);
}
END_TEST

// Проверка функции получения бита по индексу
START_TEST(s21_get_bit_test_1) {
  unsigned int bit = 1;
  unsigned int n = bit << 31;
  bool res = s21_get_bit(n, 31);
  ck_assert_uint_eq(res, bit);
}
END_TEST

START_TEST(s21_get_bit_test_2) {
  unsigned int bit = 1;
  unsigned int n = bit << 0;
  unsigned int res = s21_get_bit(n, 0);
  ck_assert_uint_eq(res, bit);
}
END_TEST

START_TEST(s21_get_exp_test) {
  unsigned int n = 5;
  unsigned int exp1 = n << 16;
  s21_decimal src = {{0, 0, 0, exp1}};
  unsigned int exp2 = s21_get_exp(src);
  ck_assert_uint_eq(exp2, n);
}
END_TEST

// Проверка функции установки экспаненты в децимал
START_TEST(s21_set_exp_test_1) {
  unsigned int exp1 = 16;
  s21_decimal src = {0};
  s21_set_exp(&src, exp1);
  unsigned int exp2 = s21_get_exp(src);
  ck_assert_uint_eq(exp2, exp1);
}
END_TEST

START_TEST(s21_set_exp_test_2) {
  unsigned int exp1 = 16;
  s21_decimal src = {{0, 0, 0, MINUS}};
  s21_set_exp(&src, exp1);
  int sign = s21_get_sign(src);
  unsigned int exp2 = s21_get_exp(src);
  ck_assert_uint_eq(exp2, exp1);
  ck_assert_uint_eq(sign, 1);
}
END_TEST

// Проверка валидации экспаненты что она меньше 28
START_TEST(s21_check_exp_test_1) {
  s21_decimal src = {{0, 0, 0, 0}};
  s21_set_exp(&src, 16);
  int ret = s21_check_exp(src);
  ck_assert_uint_eq(ret, 0);
}
END_TEST

START_TEST(s21_check_exp_test_2) {
  s21_decimal src = {{0, 0, 0, 0}};
  s21_set_exp(&src, 29);
  int ret = s21_check_exp(src);
  ck_assert_uint_eq(ret, 1);
}
END_TEST

START_TEST(s21_transfer_to_decimal_test_1) {
  s21_big_decimal src = {{12, 13, 45, 0, 0, 0, 0}, 14};
  s21_decimal res = s21_transfer_to_decimal(src);
  ck_assert_uint_eq(res.bits[0], 12);
  ck_assert_uint_eq(res.bits[1], 13);
  ck_assert_uint_eq(res.bits[2], 45);
  ck_assert_uint_eq(res.bits[3], 14 << 16);
}
END_TEST

START_TEST(s21_transfer_to_big_test_1) {
  s21_decimal src = {{12, 13, 45, 12 << 16}};
  s21_big_decimal res = s21_transfer_to_big(src);
  ck_assert_uint_eq(res.bits[0], 12);
  ck_assert_uint_eq(res.bits[1], 13);
  ck_assert_uint_eq(res.bits[2], 45);
  ck_assert_uint_eq(res.exp, 12);
}
END_TEST

START_TEST(s21_check_overflow_test_1) {
  s21_big_decimal src = {0};
  src.bits[0] = (uint64_t)UINT_MAX + 1;
  bool ret = s21_check_overflow(&src);
  ck_assert_uint_eq(src.bits[0], 0);
  ck_assert_uint_eq(src.bits[1], 1);
  ck_assert_uint_eq(ret, 0);
}
END_TEST

START_TEST(s21_check_overflow_test_2) {
  s21_big_decimal src = {0};
  src.bits[0] = UINT64_MAX;
  bool ret = s21_check_overflow(&src);
  ck_assert_uint_eq(src.bits[0], UINT_MAX);
  ck_assert_uint_eq(src.bits[1], UINT_MAX);
  ck_assert_uint_eq(ret, 0);
}
END_TEST

START_TEST(s21_check_overflow_test_3) {
  s21_big_decimal src = {0};
  src.bits[6] = UINT64_MAX;
  bool ret = s21_check_overflow(&src);
  ck_assert_uint_eq(ret, 1);
}
END_TEST

START_TEST(s21_point_left_test_1) {
  s21_big_decimal src = {0};
  src.bits[0] = 1;
  src.bits[1] = UINT_MAX;
  bool ret = s21_point_left(&src);
  ck_assert_uint_eq(ret, 0);
  ck_assert_uint_eq(src.bits[0], 10);
  ck_assert_uint_eq(src.bits[1], 4294967286);
  ck_assert_uint_eq(src.bits[2], 9);
  ck_assert_uint_eq(src.exp, 1);
}
END_TEST

START_TEST(s21_point_left_test_2) {
  s21_big_decimal src = {0};
  src.bits[6] = UINT64_MAX;
  bool ret = s21_point_left(&src);
  ck_assert_uint_eq(ret, 1);
  ck_assert_uint_eq(src.bits[6], UINT64_MAX);
}
END_TEST

START_TEST(s21_point_left_test_3) {
  s21_big_decimal src = {0};
  src.bits[0] = 11;
  bool ret = s21_point_left(&src);
  ck_assert_uint_eq(ret, 0);
  ck_assert_uint_eq(src.bits[0], 110);
}
END_TEST

START_TEST(s21_point_right_test_1) {
  s21_big_decimal src = {{0}, 1};
  src.bits[0] = 10;
  uint32_t ret = s21_point_right(&src);
  ck_assert_uint_eq(ret, 0);
  ck_assert_uint_eq(src.bits[0], 1);
  ck_assert_uint_eq(src.exp, 0);
}
END_TEST

START_TEST(s21_point_right_test_2) {
  s21_big_decimal src = {{0}, 1};
  src.bits[0] = 1234;
  uint32_t ret = s21_point_right(&src);
  ck_assert_uint_eq(ret, 4);
  ck_assert_uint_eq(src.bits[0], 123);
  ck_assert_uint_eq(src.exp, 0);
}
END_TEST

START_TEST(s21_is_overflow_test_1) {
  s21_big_decimal src = {{0}, 10};
  src.bits[2] = 123456789;
  src.bits[3] = 123456789;
  bool ret = s21_is_overflow(src);
  ck_assert_uint_eq(ret, 1);
}
END_TEST

START_TEST(s21_is_overflow_test_2) {
  s21_big_decimal src = {{0}, 10};
  src.bits[2] = 123456789;
  bool ret = s21_is_overflow(src);
  ck_assert_uint_eq(ret, 0);
}
END_TEST

START_TEST(s21_is_overflow_test_3) {
  s21_big_decimal src = {{0}, 10};
  src.bits[3] = 1;
  bool ret1 = s21_is_overflow(src);
  uint32_t reminder = s21_point_right(&src);
  bool ret2 = s21_is_overflow(src);
  ck_assert_uint_eq(ret1, 1);
  ck_assert_uint_eq(reminder, 6);
  ck_assert_uint_eq(ret2, 0);
}
END_TEST

START_TEST(s21_big_okrug_test_1) {
  s21_big_decimal src = {{0}, 10};
  src.bits[3] = 123456789;
  bool ret = s21_big_okrug(&src);
  ck_assert_uint_eq(ret, 0);
  ck_assert_uint_eq(src.bits[3], 0);
  ck_assert_uint_eq(src.exp, 1);
}
END_TEST

START_TEST(s21_big_okrug_test_2) {
  s21_big_decimal src = {{0}, 10};
  src.bits[3] = 12;
  bool ret = s21_big_okrug(&src);
  ck_assert_uint_eq(ret, 0);
  ck_assert_uint_eq(src.bits[3], 0);
  ck_assert_uint_eq(src.bits[0], 3951369912);
  ck_assert_uint_eq(src.exp, 8);
}
END_TEST

START_TEST(s21_normalize_test_1) {
  s21_big_decimal value_1 = {{0}, 3};
  s21_big_decimal value_2 = {{0}, 1};
  value_1.bits[0] = 12345;
  value_2.bits[0] = 12345;
  bool ret = s21_normalize(&value_1, &value_2);
  ck_assert_uint_eq(ret, 0);
  ck_assert_uint_eq(value_1.bits[0], 12345);
  ck_assert_uint_eq(value_2.bits[0], 1234500);
  ck_assert_uint_eq(value_1.exp, 3);
  ck_assert_uint_eq(value_1.exp, 3);
}
END_TEST

START_TEST(s21_big_add_test_1) {
  s21_big_decimal value_1 = {{0}, 1};
  s21_big_decimal value_2 = {{0}, 1};
  s21_big_decimal result = {0};
  value_1.bits[0] = 1;
  value_2.bits[0] = 1;
  bool ret = s21_big_add(value_1, value_2, &result);
  ck_assert_uint_eq(ret, 0);
  ck_assert_uint_eq(value_1.bits[0], 1);
  ck_assert_uint_eq(value_2.bits[0], 1);
}
END_TEST

START_TEST(s21_big_sub_test_1) {
  s21_big_decimal value_1 = {{0}, 0};
  s21_big_decimal value_2 = {{0}, 0};
  s21_big_decimal result = {0};
  value_1.bits[0] = 4;
  value_2.bits[0] = 2;
  s21_big_sub(value_1, value_2, &result);
  ck_assert_uint_eq(result.bits[0], 2);
}
END_TEST

START_TEST(s21_big_sub_test_2) {
  s21_big_decimal value_1 = {{0}, 0};
  s21_big_decimal value_2 = {{0}, 0};
  s21_big_decimal result = {0};
  value_1.bits[0] = 78327;
  value_2.bits[0] = 695;
  s21_big_sub(value_1, value_2, &result);
  ck_assert_uint_eq(result.bits[0], 77632);
}
END_TEST

START_TEST(s21_big_sub_test_3) {
  s21_big_decimal value_1 = {{0}, 0};
  s21_big_decimal value_2 = {{0}, 0};
  s21_big_decimal result_big = {{0}, 0};
  s21_decimal result = {0};
  value_1.bits[1] = 1;
  value_1.bits[0] = 695;
  value_2.bits[0] = 783745;
  s21_big_sub(value_1, value_2, &result_big);
  result = s21_transfer_to_decimal(result_big);
  ck_assert_uint_eq(result.bits[0], 4294184246);
  ck_assert_uint_eq(result.bits[1], 0);
}
END_TEST

Suite *helper_suite(void) {
  Suite *s;
  TCase *tc_helper;

  s = suite_create("Helper");

  tc_helper = tcase_create("Helper functions");

  // ---------- Helper functions ----------
  tcase_add_test(tc_helper, minus_test);
  tcase_add_test(tc_helper, s21_get_bit_test_1);
  tcase_add_test(tc_helper, s21_get_bit_test_2);
  tcase_add_test(tc_helper, s21_get_exp_test);
  tcase_add_test(tc_helper, s21_set_exp_test_1);
  tcase_add_test(tc_helper, s21_set_exp_test_2);
  tcase_add_test(tc_helper, s21_check_exp_test_1);
  tcase_add_test(tc_helper, s21_check_exp_test_2);
  tcase_add_test(tc_helper, s21_transfer_to_decimal_test_1);
  tcase_add_test(tc_helper, s21_transfer_to_big_test_1);
  tcase_add_test(tc_helper, s21_check_overflow_test_1);
  tcase_add_test(tc_helper, s21_check_overflow_test_2);
  tcase_add_test(tc_helper, s21_check_overflow_test_3);
  tcase_add_test(tc_helper, s21_point_left_test_1);
  tcase_add_test(tc_helper, s21_point_left_test_2);
  tcase_add_test(tc_helper, s21_point_left_test_3);
  tcase_add_test(tc_helper, s21_point_right_test_1);
  tcase_add_test(tc_helper, s21_point_right_test_2);
  tcase_add_test(tc_helper, s21_is_overflow_test_1);
  tcase_add_test(tc_helper, s21_is_overflow_test_2);
  tcase_add_test(tc_helper, s21_is_overflow_test_3);
  tcase_add_test(tc_helper, s21_big_okrug_test_1);
  tcase_add_test(tc_helper, s21_big_okrug_test_2);
  tcase_add_test(tc_helper, s21_normalize_test_1);
  tcase_add_test(tc_helper, s21_big_add_test_1);
  tcase_add_test(tc_helper, s21_big_sub_test_1);
  tcase_add_test(tc_helper, s21_big_sub_test_2);
  tcase_add_test(tc_helper, s21_big_sub_test_3);

  suite_add_tcase(s, tc_helper);

  return s;
}
