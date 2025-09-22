#include "s21_decimal.h"

int s21_add(s21_decimal value_1, s21_decimal value_2, s21_decimal *result) {
  int res = 0;

  bool s21_check_exp_1 = s21_check_exp(value_1);
  bool s21_check_exp_2 = s21_check_exp(value_2);

  if (s21_check_exp_1 || s21_check_exp_2) return 2;

  bool check_1 = s21_is_zero(value_1);
  bool check_2 = s21_is_zero(value_2);

  if (check_1 && check_2) {
    *result = value_1;

  } else if (check_1 || check_2) {
    *result = check_1 ? value_2 : value_1;
  } else {
    bool sign_1 = s21_get_sign(value_1);
    bool sign_2 = s21_get_sign(value_2);

    s21_big_decimal copy_1 = {0};
    s21_big_decimal copy_2 = {0};
    s21_big_decimal buf = {0};

    copy_1 = s21_transfer_to_big(value_1);
    copy_2 = s21_transfer_to_big(value_2);

    res = s21_normalize(&copy_1, &copy_2);

    if (sign_1 == sign_2) {
      res = s21_big_add(copy_1, copy_2, &buf);
    } else {
      s21_decimal temp_1 = {0};
      s21_decimal temp_2 = {0};

      if (sign_1) {
        s21_negate(value_1, &temp_1);
        temp_2 = value_2;
      } else {
        s21_negate(value_2, &temp_2);
        temp_1 = value_1;
      }

      if (s21_is_less(temp_1, temp_2)) {
        s21_big_sub(copy_2, copy_1, &buf);
        sign_1 = 0;
      } else {
        s21_big_sub(copy_1, copy_2, &buf);
        sign_2 = 0;
      }
    }
    *result = s21_transfer_to_decimal(buf);
    bool sign = sign_1 | sign_2;
    s21_set_sign(result, sign);
  }
  if (res == 1 && s21_get_sign(*result) == 1) {
    res = 2;
  }
  return res;
}

int s21_sub(s21_decimal value_1, s21_decimal value_2, s21_decimal *result) {
  int res = 0;
  s21_decimal copy = {0};

  bool sign_1 = s21_get_sign(value_1);
  bool sign_2 = s21_get_sign(value_2);

  if (sign_1 == sign_2) {
    s21_negate(value_2, &copy);
    res = s21_add(value_1, copy, result);
  } else {
    s21_decimal temp_1 = {0};
    s21_decimal temp_2 = {0};

    if (sign_1) {
      s21_negate(value_1, &temp_1);
      temp_2 = value_2;
    } else {
      s21_negate(value_2, &temp_2);
      temp_1 = value_1;
    }

    res = s21_add(temp_1, temp_2, result);
    s21_set_sign(result, sign_1);
  }

  return res;
}

int s21_mul(s21_decimal value_1, s21_decimal value_2, s21_decimal *result) {
  int res = 0;
  s21_decimal_alt alt_value_1 = s21_convert_std_to_alt(value_1);
  s21_decimal_alt alt_value_2 = s21_convert_std_to_alt(value_2);
  s21_decimal_alt alt_result;
  s21_null_decimal_alt(&alt_result);
  res = s21_mul_alt(alt_value_1, alt_value_2, &alt_result);
  *result = s21_convert_alt_to_std(alt_result);
  return res;
}

int s21_div(s21_decimal value_1, s21_decimal value_2, s21_decimal *result) {
  int res = 0;
  if (value_2.bits[0] == 0 && value_2.bits[1] == 0 && value_2.bits[2] == 0) {
    res = 3;
  } else if (value_1.bits[0] == 0 && value_1.bits[1] == 0 &&
             value_1.bits[2] == 0) {
    init_decimal(result);
  } else {
    s21_decimal_alt alt_value_1 = s21_convert_std_to_alt(value_1);
    s21_decimal_alt alt_value_2 = s21_convert_std_to_alt(value_2);
    s21_decimal_alt alt_result;
    s21_null_decimal_alt(&alt_result);
    while (alt_value_1.bits[0] == 0 && alt_value_2.bits[0] == 0) {
      s21_right_shift(&alt_value_1);
      s21_right_shift(&alt_value_2);
    }
    res = s21_div_alt(alt_value_1, alt_value_2, &alt_result);
    *result = s21_convert_alt_to_std(alt_result);
  }
  return res;
}
