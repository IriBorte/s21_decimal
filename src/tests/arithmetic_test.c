#include "test.h"

// arithmetic tests
START_TEST(s21_add_test_1) {
  s21_decimal value_1 = {0};
  s21_decimal value_2 = {0};
  s21_decimal dest = {0};
  int ret = s21_add(value_1, value_2, &dest);
  ck_assert_int_eq(ret, 0);
  ck_assert_uint_eq(dest.bits[0], 0);
  ck_assert_uint_eq(dest.bits[1], 0);
  ck_assert_uint_eq(dest.bits[2], 0);
  ck_assert_uint_eq(dest.bits[3], 0);
}
END_TEST

START_TEST(s21_add_test_2) {
  s21_decimal value_1 = {{1234, 124, 242, 24}};
  s21_decimal value_2 = {0};
  s21_decimal dest = {0};
  int ret = s21_add(value_1, value_2, &dest);
  ck_assert_int_eq(ret, 0);
  ck_assert_uint_eq(dest.bits[0], 1234);
  ck_assert_uint_eq(dest.bits[1], 124);
  ck_assert_uint_eq(dest.bits[2], 242);
  ck_assert_uint_eq(dest.bits[3], 24);
}
END_TEST

START_TEST(s21_add_test_3) {
  s21_decimal value_1 = {{12345, 0, 0, 0}};
  s21_decimal value_2 = {{12345, 0, 0, 0}};
  s21_set_exp(&value_1, 1);
  s21_set_exp(&value_2, 3);
  s21_decimal dest = {0};
  int ret = s21_add(value_1, value_2, &dest);
  ck_assert_int_eq(ret, 0);
  ck_assert_uint_eq(dest.bits[0], 1246845);
  ck_assert_uint_eq(dest.bits[1], 0);
  ck_assert_uint_eq(dest.bits[2], 0);
  ck_assert_uint_eq(dest.bits[3], value_2.bits[3]);
}
END_TEST

START_TEST(s21_add_test_4) {
  s21_decimal value_1 = {{12345, 0, 0, 0}};
  s21_decimal value_2 = {{12345, 0, 0, 0}};
  s21_set_exp(&value_1, 1);
  s21_set_exp(&value_2, 3);
  s21_set_sign(&value_2, 1);
  s21_decimal dest = {0};
  unsigned int res_exp = 3 << 16;
  int ret = s21_add(value_1, value_2, &dest);
  ck_assert_int_eq(ret, 0);
  ck_assert_uint_eq(dest.bits[0], 1222155);
  ck_assert_uint_eq(dest.bits[1], 0);
  ck_assert_uint_eq(dest.bits[2], 0);
  ck_assert_uint_eq(dest.bits[3], res_exp);
}
END_TEST

START_TEST(s21_add_pos_pos) {  // сложение положительных интов
  s21_decimal var1 = {{50, 0, 0, 0}};
  s21_decimal var2 = {{6, 0, 0, 0}};
  s21_decimal result = {{0, 0, 0, 0}};
  s21_decimal ch = {{56, 0, 0, 0}};
  int ret = s21_add(var1, var2, &result);
  ck_assert_int_eq(ret, 0);
  ck_assert_uint_eq(result.bits[0], ch.bits[0]);
  ck_assert_uint_eq(result.bits[1], ch.bits[1]);
  ck_assert_uint_eq(result.bits[2], ch.bits[2]);
  ck_assert_uint_eq(result.bits[3], ch.bits[3]);
}
END_TEST

START_TEST(s21_add_pos_posE) {  // сложение положительных со степенью
  s21_decimal var1 = {{50, 0, 0, 0}};
  s21_decimal var2 = {{777, 3, 0, 0}};
  var2.bits[3] = 3 << 16;
  s21_decimal result = {{0, 0, 0, 0}};
  s21_decimal ch = {{50777, 3, 0, 0}};
  ch.bits[3] = 3 << 16;
  int ret = s21_add(var1, var2, &result);
  ck_assert_int_eq(ret, 0);
  ck_assert_uint_eq(result.bits[0], ch.bits[0]);
  ck_assert_uint_eq(result.bits[1], ch.bits[1]);
  ck_assert_uint_eq(result.bits[2], ch.bits[2]);
  ck_assert_uint_eq(result.bits[3], ch.bits[3]);
}
END_TEST

START_TEST(s21_add_posE_posE) {  // сложение положительных со степенью
  s21_decimal var1 = {{8666, 5, 0, 0}};
  var1.bits[3] = 5 << 16;
  s21_decimal var2 = {{777, 3, 0, 0}};
  var2.bits[3] = 10 << 16;
  s21_decimal result = {{0, 0, 0, 0}};
  s21_decimal ch = {{866600777, 500003, 0, 0}};
  ch.bits[3] = 10 << 16;
  int ret = s21_add(var1, var2, &result);
  ck_assert_int_eq(ret, 0);
  ck_assert_uint_eq(result.bits[0], ch.bits[0]);
  ck_assert_uint_eq(result.bits[1], ch.bits[1]);
  ck_assert_uint_eq(result.bits[2], ch.bits[2]);
  ck_assert_uint_eq(result.bits[3], ch.bits[3]);
}
END_TEST

START_TEST(s21_add_neg_neg) {  // сложение отрицательных интов
  s21_decimal var1 = {{50, 0, 0, 0}};
  var1.bits[3] = 1 << 31;
  s21_decimal var2 = {{50, 0, 0, 0}};
  var2.bits[3] = 1 << 31;
  s21_decimal result = {{0, 0, 0, 0}};
  s21_decimal ch = {{100, 0, 0, 0}};
  ch.bits[3] = 1 << 31;
  int ret = s21_add(var1, var2, &result);
  ck_assert_int_eq(ret, 0);
  ck_assert_uint_eq(result.bits[0], ch.bits[0]);
  ck_assert_uint_eq(result.bits[1], ch.bits[1]);
  ck_assert_uint_eq(result.bits[2], ch.bits[2]);
  ck_assert_uint_eq(result.bits[3], ch.bits[3]);
}
END_TEST

START_TEST(s21_add_neg_negE) {  // сложение отрицательных со степенью
  s21_decimal var1 = {{50, 0, 0, 0}};
  var1.bits[3] = 1 << 31;
  s21_decimal var2 = {{777, 3, 0, 0}};
  var2.bits[3] = 3 << 16;
  var2.bits[3] |= (1 << 31);
  s21_decimal result = {{0, 0, 0, 0}};
  s21_decimal ch = {{50777, 3, 0, 0}};
  ch.bits[3] = 3 << 16;
  ch.bits[3] |= (1 << 31);
  int ret = s21_add(var1, var2, &result);
  ck_assert_int_eq(ret, 0);
  ck_assert_uint_eq(result.bits[0], ch.bits[0]);
  ck_assert_uint_eq(result.bits[1], ch.bits[1]);
  ck_assert_uint_eq(result.bits[2], ch.bits[2]);
  ck_assert_uint_eq(result.bits[3], ch.bits[3]);
}
END_TEST

START_TEST(s21_add_negE_negE) {  // сложение отрицательных со степенью
  s21_decimal var1 = {{50, 60, 0, 0}};
  var1.bits[3] = 1 << 31;
  var1.bits[3] |= 3 << 16;
  s21_decimal var2 = {{4, 5555, 333, 0}};
  var2.bits[3] = 7 << 16;
  var2.bits[3] |= (1 << 31);
  s21_decimal result = {{0, 0, 0, 0}};
  s21_decimal ch = {{500004, 605555, 333, 0}};
  ch.bits[3] = 7 << 16;
  ch.bits[3] |= (1 << 31);
  int ret = s21_add(var1, var2, &result);
  ck_assert_int_eq(ret, 0);
  ck_assert_uint_eq(result.bits[0], ch.bits[0]);
  ck_assert_uint_eq(result.bits[1], ch.bits[1]);
  ck_assert_uint_eq(result.bits[2], ch.bits[2]);
  ck_assert_uint_eq(result.bits[3], ch.bits[3]);
}
END_TEST

START_TEST(s21_add_pos_neg) {  // сложение положительного и отрицательного
                               //(переход в вычитание)
  s21_decimal var1 = {{50, 50, 0, 0}};
  s21_decimal var2 = {{50, 0, 0, 0}};
  var2.bits[3] = 1 << 31;
  s21_decimal result = {{0, 0, 0, 0}};
  s21_decimal ch = {{0, 50, 0, 0}};
  int ret = s21_add(var1, var2, &result);
  ck_assert_int_eq(ret, 0);
  ck_assert_uint_eq(result.bits[0], ch.bits[0]);
  ck_assert_uint_eq(result.bits[1], ch.bits[1]);
  ck_assert_uint_eq(result.bits[2], ch.bits[2]);
  ck_assert_uint_eq(result.bits[3], ch.bits[3]);
}

START_TEST(s21_add_neg_pos) {  // одна инверсия (тоже переход в вычитание)
  s21_decimal var1 = {{50, 50, 0, 0}};
  var1.bits[3] = 1 << 31;
  s21_decimal var2 = {{0, 50, 0, 0}};
  s21_decimal ch = {{50, 0, 0, 0}};
  ch.bits[3] = 1 << 31;
  s21_decimal result = {{0, 0, 0, 0}};
  int ret = s21_add(var1, var2, &result);
  ck_assert_int_eq(ret, 0);
  ck_assert_uint_eq(result.bits[0], ch.bits[0]);
  ck_assert_uint_eq(result.bits[1], ch.bits[1]);
  ck_assert_uint_eq(result.bits[2], ch.bits[2]);
  ck_assert_uint_eq(result.bits[3], ch.bits[3]);
}
END_TEST

START_TEST(s21_add_0_0) {  // проверка на сложение 0
  s21_decimal var1 = {{0, 0, 0, 0}};
  s21_decimal var2 = {{0, 0, 0, 0}};
  s21_decimal ch = {{0, 0, 0, 0}};
  s21_decimal result = {{0, 0, 0, 0}};
  int ret = s21_add(var1, var2, &result);
  ck_assert_int_eq(ret, 0);
  ck_assert_uint_eq(result.bits[0], ch.bits[0]);
  ck_assert_uint_eq(result.bits[1], ch.bits[1]);
  ck_assert_uint_eq(result.bits[2], ch.bits[2]);
  ck_assert_uint_eq(result.bits[3], ch.bits[3]);
}
END_TEST

START_TEST(s21_add_max) {  // проверка на превышение мантиссы
  s21_decimal var1 = {{4294967295, 4294967295, 4294967295, 0}};
  s21_decimal var2 = {{1, 0, 0, 0}};
  s21_decimal result = {{0, 0, 0, 0}};
  int ret = s21_add(var1, var2, &result);
  ck_assert_int_eq(ret, 1);
}
END_TEST

START_TEST(s21_sub_pos_pos) {  // вычитание положительных чисел с положительным
                               // в ответе
  s21_decimal var1 = {{50, 50, 50, 0}};
  s21_decimal var2 = {{50, 50, 0, 0}};
  s21_decimal result = {{0, 0, 0, 0}};
  s21_decimal ch = {{0, 0, 50, 0}};
  int ret = s21_sub(var1, var2, &result);
  ck_assert_int_eq(ret, 0);
  ck_assert_uint_eq(result.bits[0], ch.bits[0]);
  ck_assert_uint_eq(result.bits[1], ch.bits[1]);
  ck_assert_uint_eq(result.bits[2], ch.bits[2]);
  ck_assert_uint_eq(result.bits[3], ch.bits[3]);
}
END_TEST

START_TEST(s21_sub_pos_pos2) {  // вычитание положительных чисел с отрицательным
                                // в ответе
  s21_decimal var1 = {{0, 50, 50, 0}};
  s21_decimal var2 = {{50, 50, 50, 0}};
  s21_decimal result = {{0, 0, 0, 0}};
  s21_decimal ch = {{50, 0, 0, 0}};
  ch.bits[3] = 1 << 31;
  int ret = s21_sub(var1, var2, &result);
  ck_assert_int_eq(ret, 0);
  ck_assert_uint_eq(result.bits[0], ch.bits[0]);
  ck_assert_uint_eq(result.bits[1], ch.bits[1]);
  ck_assert_uint_eq(result.bits[2], ch.bits[2]);
  ck_assert_uint_eq(result.bits[3], ch.bits[3]);
}
END_TEST

START_TEST(s21_sub_pos_posE) {  // вычитание положительных чисел
  s21_decimal var1 = {{30, 00, 00, 0}};
  s21_decimal var2 = {{55555550, 50, 0, 0}};
  var2.bits[3] = 3 << 16;
  s21_decimal result = {{0, 0, 0, 0}};
  s21_decimal ch = {{55525550, 50, 0, 0}};
  ch.bits[3] = 3 << 16;
  ch.bits[3] |= (1 << 31);
  int ret = s21_sub(var1, var2, &result);
  ck_assert_int_eq(ret, 0);
  ck_assert_uint_eq(result.bits[0], ch.bits[0]);
  ck_assert_uint_eq(result.bits[1], ch.bits[1]);
  ck_assert_uint_eq(result.bits[2], ch.bits[2]);
  ck_assert_uint_eq(result.bits[3], ch.bits[3]);
}
END_TEST

START_TEST(s21_sub_posE_posE) {  // вычитание положительных чисел со степенью
  s21_decimal var1 = {{7, 55, 0, 0}};
  var1.bits[3] = 6 << 16;
  s21_decimal var2 = {{30, 50, 0, 0}};
  var2.bits[3] = 7 << 16;
  s21_decimal ch = {{40, 500, 0, 0}};
  ch.bits[3] = 7 << 16;
  s21_decimal result = {{0, 0, 0, 0}};
  int ret = s21_sub(var1, var2, &result);
  ck_assert_int_eq(ret, 0);
  ck_assert_uint_eq(result.bits[0], ch.bits[0]);
  ck_assert_uint_eq(result.bits[1], ch.bits[1]);
  ck_assert_uint_eq(result.bits[2], ch.bits[2]);
  ck_assert_uint_eq(result.bits[3], ch.bits[3]);
}
END_TEST

START_TEST(s21_sub_neg_neg) {  // вычитание отрицательных чисел
  s21_decimal var1 = {{7, 33, 0, 0}};
  var1.bits[3] = 1 << 31;
  s21_decimal var2 = {{10, 55, 0, 0}};
  var2.bits[3] = 1 << 31;
  s21_decimal ch = {{3, 22, 0, 0}};
  s21_decimal result = {{0, 0, 0, 0}};
  int ret = s21_sub(var1, var2, &result);
  ck_assert_int_eq(ret, 0);
  ck_assert_uint_eq(result.bits[0], ch.bits[0]);
  ck_assert_uint_eq(result.bits[1], ch.bits[1]);
  ck_assert_uint_eq(result.bits[2], ch.bits[2]);
  ck_assert_uint_eq(result.bits[3], ch.bits[3]);
}
END_TEST

START_TEST(s21_sub_neg_negE) {  // вычитание отрицтельных чисел со степенью
  s21_decimal var1 = {{7, 55, 0, 0}};
  var1.bits[3] = 1 << 31;
  s21_decimal var2 = {{30, 50, 0, 0}};
  var2.bits[3] = 1 << 31;
  var2.bits[3] |= (2 << 16);
  s21_decimal ch = {{670, 5450, 0, 0}};
  ch.bits[3] = 1 << 31;
  ch.bits[3] |= (2 << 16);
  s21_decimal result = {{0, 0, 0, 0}};
  int ret = s21_sub(var1, var2, &result);
  ck_assert_int_eq(ret, 0);
  ck_assert_uint_eq(result.bits[0], ch.bits[0]);
  ck_assert_uint_eq(result.bits[1], ch.bits[1]);
  ck_assert_uint_eq(result.bits[2], ch.bits[2]);
  ck_assert_uint_eq(result.bits[3], ch.bits[3]);
}
END_TEST

START_TEST(s21_sub_negE_negE) {  // вычитание отрицательных чисел со степенью
  s21_decimal var1 = {{70, 55, 0, 0}};
  var1.bits[3] = 1 << 31;
  var1.bits[3] |= (4 << 16);
  s21_decimal var2 = {{30, 50, 0, 0}};
  var2.bits[3] = 1 << 31;
  var2.bits[3] |= (3 << 16);
  s21_decimal ch = {{230, 445, 0, 0}};
  ch.bits[3] |= (4 << 16);
  s21_decimal result = {{0, 0, 0, 0}};
  int ret = s21_sub(var1, var2, &result);
  ck_assert_int_eq(ret, 0);
  ck_assert_uint_eq(result.bits[0], ch.bits[0]);
  ck_assert_uint_eq(result.bits[1], ch.bits[1]);
  ck_assert_uint_eq(result.bits[2], ch.bits[2]);
  ck_assert_uint_eq(result.bits[3], ch.bits[3]);
}
END_TEST

START_TEST(s21_sub_pos_neg) {  // переход в сложение
  s21_decimal var1 = {{50, 50, 50, 0}};
  s21_decimal var2 = {{50, 50, 50, 0}};
  var2.bits[3] = 1 << 31;
  s21_decimal ch = {{100, 100, 100, 0}};
  s21_decimal result = {{0, 0, 0, 0}};
  int ret = s21_sub(var1, var2, &result);
  ck_assert_int_eq(ret, 0);
  ck_assert_uint_eq(result.bits[0], ch.bits[0]);
  ck_assert_uint_eq(result.bits[1], ch.bits[1]);
  ck_assert_uint_eq(result.bits[2], ch.bits[2]);
  ck_assert_uint_eq(result.bits[3], ch.bits[3]);
}
END_TEST

START_TEST(s21_sub_neg_pos) {  // переход в сложение
  s21_decimal var1 = {{50, 50, 50, 0}};
  var1.bits[3] = 1 << 31;
  var1.bits[3] |= 3 << 16;
  s21_decimal var2 = {{50, 50, 50, 0}};
  s21_decimal ch = {{50050, 50050, 50050, 0}};
  ch.bits[3] = 1 << 31;
  ch.bits[3] |= 3 << 16;
  s21_decimal result = {{0, 0, 0, 0}};
  int ret = s21_sub(var1, var2, &result);
  ck_assert_int_eq(ret, 0);
  ck_assert_uint_eq(result.bits[0], ch.bits[0]);
  ck_assert_uint_eq(result.bits[1], ch.bits[1]);
  ck_assert_uint_eq(result.bits[2], ch.bits[2]);
  ck_assert_uint_eq(result.bits[3], ch.bits[3]);
}
END_TEST

START_TEST(s21_sub_0_0) {  // проверка на вычитание 0
  s21_decimal var1 = {{0, 0, 0, 0}};
  s21_decimal var2 = {{0, 0, 0, 0}};
  s21_decimal ch = {{0, 0, 0, 0}};
  s21_decimal result = {{0, 0, 0, 0}};
  int ret = s21_sub(var1, var2, &result);
  ck_assert_int_eq(ret, 0);
  ck_assert_uint_eq(result.bits[0], ch.bits[0]);
  ck_assert_uint_eq(result.bits[1], ch.bits[1]);
  ck_assert_uint_eq(result.bits[2], ch.bits[2]);
  ck_assert_uint_eq(result.bits[3], ch.bits[3]);
}
END_TEST

START_TEST(s21_sub_max) {  // проверка на превышение мантиссы
  s21_decimal var1 = {{4294967295, 4294967295, 4294967295, 0}};
  var1.bits[3] = 1 << 31;
  s21_decimal var2 = {{10, 0, 0, 0}};
  // var2.bits[3] = 1 << 31;
  s21_decimal result = {{0, 0, 0, 0}};
  int ret = s21_sub(var1, var2, &result);
  ck_assert_int_eq(ret, 1);
}
END_TEST

START_TEST(s21_sub_pos29_pos) {  // проверка на понижение степени до 28
                                 //(отбрасывание знака)
  s21_decimal var1 = {{50, 0, 0, 0}};
  var1.bits[3] |= (29 << 16);
  s21_decimal var2 = {{2, 3, 0, 0}};
  var2.bits[3] = 27 << 16;
  s21_decimal ch = {{0, 0, 0, 0}};
  s21_decimal result = {{0, 0, 0, 0}};
  int ret = s21_sub(var1, var2, &result);
  ck_assert_int_eq(ret, 2);
  ck_assert_uint_eq(result.bits[0], ch.bits[0]);
  ck_assert_uint_eq(result.bits[1], ch.bits[1]);
  ck_assert_uint_eq(result.bits[2], ch.bits[2]);
  ck_assert_uint_eq(result.bits[3], ch.bits[3]);
}
END_TEST

// ----------------------------------------------

START_TEST(s21_mul_test_1) {
  s21_decimal var1 = {{50, 0, 0, MINUS}};
  unsigned int n = 1 << 16;
  s21_decimal var2 = {{32, 0, 0, n}};
  s21_decimal result = {{0, 0, 0, 0}};
  int ret = s21_mul(var1, var2, &result);
  ck_assert_int_eq(ret, 0);
  ck_assert_uint_eq(result.bits[0], 1600);
  ck_assert_uint_eq(result.bits[1], 0);
  ck_assert_uint_eq(result.bits[2], 0);
  ck_assert_uint_eq(result.bits[3], 2147549184);
}
END_TEST

START_TEST(mul_int_1) {
  s21_decimal value_1 = {{2, 0, 0, 0}};
  s21_decimal value_2 = {{23, 0, 0, 0}};
  s21_decimal result = {0};
  int code;

  code = s21_mul(value_1, value_2, &result);

  ck_assert_int_eq(code, 0);
  ck_assert_int_eq(result.bits[0], 46);
  ck_assert_int_eq(result.bits[1], 0);
  ck_assert_int_eq(result.bits[2], 0);
  ck_assert_int_eq(result.bits[3], 0);
}
END_TEST

START_TEST(mul_int_2) {
  s21_decimal value_1 = {{2, 0, 0, 0}};
  s21_decimal value_2 = {{23, 0, 0, 0}};
  s21_set_sign(&value_1, 1);
  s21_decimal result = {0};
  int code;

  code = s21_mul(value_1, value_2, &result);

  ck_assert_int_eq(code, 0);
  ck_assert_int_eq(result.bits[0], 46);
  ck_assert_int_eq(result.bits[1], 0);
  ck_assert_int_eq(result.bits[2], 0);
  ck_assert_int_eq(result.bits[3], 2147483648);
}
END_TEST

START_TEST(mul_int_3) {
  s21_decimal value_1 = {{2, 0, 0, 0}};
  s21_decimal value_2 = {{23, 0, 0, 0}};
  s21_set_sign(&value_2, 1);
  s21_decimal result = {0};
  int code;

  code = s21_mul(value_1, value_2, &result);

  ck_assert_int_eq(code, 0);
  ck_assert_int_eq(result.bits[0], 46);
  ck_assert_int_eq(result.bits[1], 0);
  ck_assert_int_eq(result.bits[2], 0);
  ck_assert_int_eq(result.bits[3], 2147483648);
}
END_TEST

START_TEST(mul_int_4) {
  s21_decimal value_1 = {{2, 0, 0, 0}};
  s21_decimal value_2 = {{23, 0, 0, 0}};
  s21_set_sign(&value_2, 1);
  s21_set_sign(&value_1, 1);
  s21_decimal result = {0};
  int code;

  code = s21_mul(value_1, value_2, &result);

  ck_assert_int_eq(code, 0);
  ck_assert_int_eq(result.bits[0], 46);
  ck_assert_int_eq(result.bits[1], 0);
  ck_assert_int_eq(result.bits[2], 0);
  ck_assert_int_eq(result.bits[3], 0);
}
END_TEST

START_TEST(mul_float_1) {
  s21_decimal value_1 = {{2, 0, 0, 0}};
  s21_decimal value_2 = {{23, 0, 0, 0}};
  s21_decimal result = {0};
  int code;
  s21_set_exp(&value_1, 2);
  s21_set_exp(&value_2, 3);
  code = s21_mul(value_1, value_2, &result);

  ck_assert_int_eq(code, 0);
  ck_assert_int_eq(result.bits[0], 46);
  ck_assert_int_eq(result.bits[1], 0);
  ck_assert_int_eq(result.bits[2], 0);
  ck_assert_int_eq(result.bits[3], 327680);
}
END_TEST

START_TEST(mul_float_2) {
  s21_decimal value_1 = {{2, 0, 0, 0}};
  s21_decimal value_2 = {{23, 0, 0, 0}};
  s21_set_sign(&value_1, 1);
  s21_set_exp(&value_1, 2);
  s21_set_exp(&value_2, 3);
  s21_decimal result = {0};
  int code;

  code = s21_mul(value_1, value_2, &result);

  ck_assert_int_eq(code, 0);
  ck_assert_int_eq(result.bits[0], 46);
  ck_assert_int_eq(result.bits[1], 0);
  ck_assert_int_eq(result.bits[2], 0);
  ck_assert_int_eq(result.bits[3], 2147811328);
}
END_TEST

START_TEST(mul_float_3) {
  s21_decimal value_1 = {{2, 0, 0, 0}};
  s21_decimal value_2 = {{23, 0, 0, 0}};
  s21_set_sign(&value_2, 1);
  s21_decimal result = {0};
  int code;
  s21_set_exp(&value_1, 2);
  s21_set_exp(&value_2, 3);
  code = s21_mul(value_1, value_2, &result);

  ck_assert_int_eq(code, 0);
  ck_assert_int_eq(result.bits[0], 46);
  ck_assert_int_eq(result.bits[1], 0);
  ck_assert_int_eq(result.bits[2], 0);
  ck_assert_int_eq(result.bits[3], 2147811328);
}
END_TEST

START_TEST(mul_float_4) {
  s21_decimal value_1 = {{2, 0, 0, 0}};
  s21_decimal value_2 = {{23, 0, 0, 0}};
  s21_set_sign(&value_2, 1);
  s21_set_sign(&value_1, 1);
  s21_decimal result = {0};
  int code;
  s21_set_exp(&value_1, 2);
  s21_set_exp(&value_2, 3);
  code = s21_mul(value_1, value_2, &result);

  ck_assert_int_eq(code, 0);
  ck_assert_int_eq(result.bits[0], 46);
  ck_assert_int_eq(result.bits[1], 0);
  ck_assert_int_eq(result.bits[2], 0);
  ck_assert_int_eq(result.bits[3], 327680);
}
END_TEST

START_TEST(mul_neg_inf) {
  s21_decimal value_1 = {{4294967295, 4294967295, 4294967295, 0}};
  s21_decimal value_2 = {{23, 0, 0, 0}};
  s21_set_sign(&value_1, 1);
  s21_decimal result = {0};
  int code;

  code = s21_mul(value_1, value_2, &result);

  ck_assert_int_eq(code, 2);
}
END_TEST

START_TEST(mul_bank_round_1) {
  s21_decimal value_1 = {{156, 0, 0, 0}};
  s21_decimal value_2 = {{342, 0, 0, 0}};
  s21_set_exp(&value_2, 5);
  s21_set_exp(&value_1, 24);
  s21_decimal result = {0};
  int code;

  code = s21_mul(value_1, value_2, &result);
  ck_assert_int_eq(result.bits[0], 5335);
  ck_assert_int_eq(result.bits[1], 0);
  ck_assert_int_eq(result.bits[2], 0);
  ck_assert_int_eq(result.bits[3], 1835008);
  ck_assert_int_eq(code, 0);
}
END_TEST

START_TEST(mul_bank_round_2) {
  s21_decimal value_1 = {{156, 0, 0, 0}};
  s21_decimal value_2 = {{342, 0, 0, 0}};
  s21_set_exp(&value_2, 5);
  s21_set_exp(&value_1, 24);
  s21_decimal result = {0};
  int code;

  code = s21_mul(value_1, value_2, &result);
  ck_assert_int_eq(result.bits[0], 5335);
  ck_assert_int_eq(result.bits[1], 0);
  ck_assert_int_eq(result.bits[2], 0);
  ck_assert_int_eq(result.bits[3], 1835008);
  ck_assert_int_eq(code, 0);
}
END_TEST

START_TEST(mul_bank_round_4) {
  s21_decimal value_1 = {{155, 0, 0, 0}};
  s21_decimal value_2 = {{355, 0, 0, 0}};
  s21_set_exp(&value_2, 5);
  s21_set_exp(&value_1, 24);
  s21_set_sign(&value_1, 1);
  s21_decimal result = {0};
  int code;

  code = s21_mul(value_1, value_2, &result);
  ck_assert_int_eq(result.bits[0], 5502);
  ck_assert_int_eq(result.bits[1], 0);
  ck_assert_int_eq(result.bits[2], 0);
  ck_assert_int_eq(result.bits[3], 2149318656);
  ck_assert_int_eq(code, 0);
}
END_TEST

START_TEST(mul_one) {
  s21_decimal value_1 = {{155, 0, 0, 0}};
  s21_decimal value_2 = {{1, 0, 0, 0}};
  s21_decimal result = {0};
  int code;

  code = s21_mul(value_1, value_2, &result);
  ck_assert_int_eq(result.bits[0], 155);
  ck_assert_int_eq(result.bits[1], 0);
  ck_assert_int_eq(result.bits[2], 0);
  ck_assert_int_eq(result.bits[3], 0);
  ck_assert_int_eq(code, 0);
}
END_TEST

// ----------------------------------------------

START_TEST(s21_div_test_1) {  // проверка деления на 0
  s21_decimal var1 = {{50, 0, 0, MINUS}};
  unsigned int n = 1 << 16;
  s21_decimal var2 = {{0, 0, 0, n}};
  s21_decimal result = {{0, 0, 0, 0}};
  int ret = s21_div(var1, var2, &result);
  ck_assert_int_eq(ret, 3);
  ck_assert_uint_eq(result.bits[0], 0);
}
END_TEST

START_TEST(div_int_1) {
  s21_decimal value_1 = {{15, 0, 0, 0}};
  s21_decimal value_2 = {{3, 0, 0, 0}};
  s21_decimal result = {0};
  int code;

  code = s21_div(value_1, value_2, &result);

  ck_assert_int_eq(code, 0);
  ck_assert_int_eq(result.bits[0], 5);
  ck_assert_int_eq(result.bits[1], 0);
  ck_assert_int_eq(result.bits[2], 0);
  ck_assert_int_eq(result.bits[3], 0);
}
END_TEST

START_TEST(div_int_2) {
  s21_decimal value_1 = {{15, 0, 0, 0}};
  s21_decimal value_2 = {{3, 0, 0, 0}};
  s21_set_sign(&value_1, 1);
  s21_decimal result = {0};
  int code;

  code = s21_div(value_1, value_2, &result);

  ck_assert_int_eq(code, 0);
  ck_assert_int_eq(result.bits[0], 5);
  ck_assert_int_eq(result.bits[1], 0);
  ck_assert_int_eq(result.bits[2], 0);
  ck_assert_int_eq(result.bits[3], 2147483648);
}
END_TEST

START_TEST(div_int_3) {
  s21_decimal value_1 = {{15, 0, 0, 0}};
  s21_decimal value_2 = {{3, 0, 0, 0}};
  s21_set_sign(&value_2, 1);
  s21_decimal result = {0};
  int code;

  code = s21_div(value_1, value_2, &result);

  ck_assert_int_eq(code, 0);
  ck_assert_int_eq(result.bits[0], 5);
  ck_assert_int_eq(result.bits[1], 0);
  ck_assert_int_eq(result.bits[2], 0);
  ck_assert_int_eq(result.bits[3], 2147483648);
}
END_TEST

START_TEST(div_int_4) {
  s21_decimal value_1 = {{15, 0, 0, 0}};
  s21_decimal value_2 = {{3, 0, 0, 0}};
  s21_set_sign(&value_2, 1);
  s21_set_sign(&value_1, 1);
  s21_decimal result = {0};
  int code;

  code = s21_div(value_1, value_2, &result);

  ck_assert_int_eq(code, 0);
  ck_assert_int_eq(result.bits[0], 5);
  ck_assert_int_eq(result.bits[1], 0);
  ck_assert_int_eq(result.bits[2], 0);
  ck_assert_int_eq(result.bits[3], 0);
}
END_TEST

START_TEST(div_float_1) {
  s21_decimal value_1 = {{15, 0, 0, 0}};
  s21_decimal value_2 = {{3, 0, 0, 0}};
  s21_decimal result = {0};
  int code;
  s21_set_exp(&value_1, 7);
  s21_set_exp(&value_2, 3);
  code = s21_div(value_1, value_2, &result);

  ck_assert_int_eq(code, 0);
  ck_assert_int_eq(result.bits[0], 5);
  ck_assert_int_eq(result.bits[1], 0);
  ck_assert_int_eq(result.bits[2], 0);
  ck_assert_int_eq(result.bits[3], 262144);
}
END_TEST

START_TEST(div_float_2) {
  s21_decimal value_1 = {{15, 0, 0, 0}};
  s21_decimal value_2 = {{3, 0, 0, 0}};
  s21_set_sign(&value_1, 1);
  s21_set_exp(&value_1, 3);
  s21_set_exp(&value_2, 7);
  s21_decimal result = {0};
  int code;

  code = s21_div(value_1, value_2, &result);

  ck_assert_int_eq(code, 0);
  ck_assert_int_eq(result.bits[0], 50000);
  ck_assert_int_eq(result.bits[1], 0);
  ck_assert_int_eq(result.bits[2], 0);
  ck_assert_int_eq(result.bits[3], 2147483648);
}
END_TEST

START_TEST(div_float_3) {
  s21_decimal value_1 = {{15, 0, 0, 0}};
  s21_decimal value_2 = {{3, 0, 0, 0}};
  s21_decimal result = {0};
  int code;
  s21_set_exp(&value_1, 33);
  s21_set_exp(&value_2, 3);
  code = s21_div(value_1, value_2, &result);

  ck_assert_int_eq(code, 0);
  ck_assert_int_eq(result.bits[0], 0);
  ck_assert_int_eq(result.bits[1], 0);
  ck_assert_int_eq(result.bits[2], 0);
  ck_assert_int_eq(result.bits[3], 1835008);
}
END_TEST

START_TEST(div_float_4) {
  s21_decimal value_1 = {{3, 0, 0, 0}};
  s21_decimal value_2 = {{15, 0, 0, 0}};
  s21_set_sign(&value_2, 1);
  s21_set_sign(&value_1, 1);
  s21_decimal result = {0};
  int code;
  code = s21_div(value_1, value_2, &result);

  ck_assert_int_eq(code, 0);
  ck_assert_int_eq(result.bits[0], 2);
  ck_assert_int_eq(result.bits[1], 0);
  ck_assert_int_eq(result.bits[2], 0);
  ck_assert_int_eq(result.bits[3], 65536);
}
END_TEST

START_TEST(div_zero) {
  s21_decimal value_1 = {{156, 0, 0, 0}};
  s21_decimal value_2 = {{0, 0, 0, 0}};
  s21_set_exp(&value_2, 5);
  s21_set_exp(&value_1, 25);
  s21_decimal result = {0};
  int code;

  code = s21_div(value_1, value_2, &result);
  ck_assert_int_eq(code, 3);
}
END_TEST

START_TEST(div_one) {
  s21_decimal value_1 = {{156, 0, 0, 0}};
  s21_decimal value_2 = {{1, 0, 0, 0}};
  s21_decimal result = {0};
  int code;

  code = s21_div(value_1, value_2, &result);
  ck_assert_int_eq(code, 0);
  ck_assert_int_eq(result.bits[0], 156);
  ck_assert_int_eq(result.bits[1], 0);
  ck_assert_int_eq(result.bits[2], 0);
  ck_assert_int_eq(result.bits[3], 0);
}
END_TEST

START_TEST(div_one_1) {
  s21_decimal value_1 = {{156, 0, 0, 0}};
  s21_decimal value_2 = {{156, 0, 0, 0}};
  s21_decimal result = {0};
  int code;

  code = s21_div(value_1, value_2, &result);
  ck_assert_int_eq(code, 0);
  ck_assert_int_eq(result.bits[0], 1);
  ck_assert_int_eq(result.bits[1], 0);
  ck_assert_int_eq(result.bits[2], 0);
  ck_assert_int_eq(result.bits[3], 0);
}
END_TEST

Suite *arithmetic_suite(void) {
  Suite *s;
  TCase *tc_arithmetic;

  s = suite_create("Arithmetic");

  tc_arithmetic = tcase_create("Arithmetic functions");

  // ---------- Arithmetic functions ----------

  suite_add_tcase(s, tc_arithmetic);
  tcase_add_test(tc_arithmetic, s21_add_test_1);
  tcase_add_test(tc_arithmetic, s21_add_test_2);
  tcase_add_test(tc_arithmetic, s21_add_test_3);
  tcase_add_test(tc_arithmetic, s21_add_test_4);
  // s21_arithmetic_sub
  tcase_add_test(tc_arithmetic, s21_sub_pos_pos);
  tcase_add_test(tc_arithmetic, s21_sub_pos_pos2);
  tcase_add_test(tc_arithmetic, s21_sub_pos_posE);
  tcase_add_test(tc_arithmetic, s21_sub_posE_posE);
  tcase_add_test(tc_arithmetic, s21_sub_neg_neg);
  tcase_add_test(tc_arithmetic, s21_sub_neg_negE);
  tcase_add_test(tc_arithmetic, s21_sub_negE_negE);
  tcase_add_test(tc_arithmetic, s21_sub_pos_neg);
  tcase_add_test(tc_arithmetic, s21_sub_neg_pos);
  tcase_add_test(tc_arithmetic, s21_sub_0_0);
  tcase_add_test(tc_arithmetic, s21_sub_max);
  tcase_add_test(tc_arithmetic, s21_sub_pos29_pos);
  // s21_arithmetic_add
  tcase_add_test(tc_arithmetic, s21_add_pos_pos);
  tcase_add_test(tc_arithmetic, s21_add_pos_posE);
  tcase_add_test(tc_arithmetic, s21_add_posE_posE);
  tcase_add_test(tc_arithmetic, s21_add_neg_neg);
  tcase_add_test(tc_arithmetic, s21_add_neg_negE);
  tcase_add_test(tc_arithmetic, s21_add_negE_negE);
  tcase_add_test(tc_arithmetic, s21_add_pos_neg);
  tcase_add_test(tc_arithmetic, s21_add_neg_pos);
  tcase_add_test(tc_arithmetic, s21_add_0_0);
  tcase_add_test(tc_arithmetic, s21_add_max);
  // s21_arithmetic_mul
  tcase_add_test(tc_arithmetic, s21_mul_test_1);
  tcase_add_test(tc_arithmetic, mul_int_1);
  tcase_add_test(tc_arithmetic, mul_int_2);
  tcase_add_test(tc_arithmetic, mul_int_3);
  tcase_add_test(tc_arithmetic, mul_int_4);
  tcase_add_test(tc_arithmetic, mul_float_1);
  tcase_add_test(tc_arithmetic, mul_float_2);
  tcase_add_test(tc_arithmetic, mul_float_3);
  tcase_add_test(tc_arithmetic, mul_float_4);
  tcase_add_test(tc_arithmetic, mul_neg_inf);
  tcase_add_test(tc_arithmetic, mul_bank_round_1);
  tcase_add_test(tc_arithmetic, mul_bank_round_2);
  tcase_add_test(tc_arithmetic, mul_bank_round_4);
  tcase_add_test(tc_arithmetic, mul_one);
  // s21_arithmetic_div
  tcase_add_test(tc_arithmetic, s21_div_test_1);
  tcase_add_test(tc_arithmetic, div_int_1);
  tcase_add_test(tc_arithmetic, div_int_2);
  tcase_add_test(tc_arithmetic, div_int_3);
  tcase_add_test(tc_arithmetic, div_int_4);
  tcase_add_test(tc_arithmetic, div_float_1);
  tcase_add_test(tc_arithmetic, div_float_2);
  tcase_add_test(tc_arithmetic, div_float_3);
  tcase_add_test(tc_arithmetic, div_float_4);
  tcase_add_test(tc_arithmetic, div_zero);
  tcase_add_test(tc_arithmetic, div_one);
  tcase_add_test(tc_arithmetic, div_one_1);
  return s;
}
