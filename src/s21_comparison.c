#include "s21_decimal.h"
/*
 * Function checks if two given `s21_decimal` numbers are equal.
 * The function returns int variable `equal`.
 * If the numbers are equal function returns 1 and 0 otherwise.
 */
int s21_is_equal(s21_decimal value_1, s21_decimal value_2) {
  int equal = 1;
  int check = 0;
  // Check if mantissa is zero
  for (int i = 0; i < 3 && equal == 1; i++) {  // Проверяем мантиссу на 0
    if ((value_1.bits[i] != 0) && (value_2.bits[i] != 0)) {
      equal = 0;
    }
  }

  // If sign is different, then numbers are definitely different too.
  if (equal == 0 && check == 0) {
    if (s21_get_sign(value_1) != s21_get_sign(value_2)) check = 1;
  }
  // Normalization process
  if (equal == 0 && check == 0) {
    s21_big_decimal copy_1 = s21_transfer_to_big(value_1);
    s21_big_decimal copy_2 = s21_transfer_to_big(value_2);
    s21_normalize(&copy_1, &copy_2);
    int test = 4;
    for (int i = 3; i >= 0 && test == i + 1; i--) {
      if (copy_1.bits[i] == copy_2.bits[i]) {
        test--;
      }
    }
    if (test == 0) equal = 1;
  }
  return equal;
}

/*
 * Function checks if two given `s21_decimal` numbers are not equal.
 * The function returns inverted result of s21_is_equal.
 * If the numbers are not equal function returns 1 and 0 otherwise.
 */
int s21_is_not_equal(s21_decimal value_1, s21_decimal value_2) {
  return !(s21_is_equal(value_1, value_2));
}

/*
 * This function checks if first `s21_decimal` number is STRICTLY
 * less than second `s21_decimal` number.
 * If first value is strictly less, than function returns 1,
 * if not - returns 0.
 */
int s21_is_less(s21_decimal value_1, s21_decimal value_2) {
  int less = 1;
  // If numbers are equal - this is 0 flag
  // If numbers are not equal -> check their signs
  // If signs are equal -> normalize and check mantissa
  if (s21_is_equal(value_1, value_2) == 1) {
    less = 0;
  } else if (s21_get_sign(value_1) == 0 && s21_get_sign(value_2) == 1) {
    less = 0;
  } else if (s21_get_sign(value_1) == 1 && s21_get_sign(value_2) == 0) {
    less = 1;
  } else {
    int sign = s21_get_sign(value_1);
    s21_big_decimal copy_1 = s21_transfer_to_big(value_1);
    s21_big_decimal copy_2 = s21_transfer_to_big(value_2);
    s21_normalize(&copy_1, &copy_2);
    int test = 0;
    // Check if number is bigger based on mantissa with negative sign
    for (int i = 2; sign == 1 && i >= 0 && test == 0; i--) {
      if (copy_1.bits[i] > copy_2.bits[i])
        test = 1;
      else if (copy_1.bits[i] < copy_2.bits[i])
        test = 2;
    }
    // Check if number is less based on mantissa with positive sign
    for (int i = 2; sign == 0 && i >= 0 && test == 0; i--) {
      if (copy_1.bits[i] < copy_2.bits[i])
        test = 1;
      else if (copy_1.bits[i] > copy_2.bits[i])
        test = 2;
    }
    if (test == 1)
      less = 1;
    else
      less = 0;
  }
  return less;
}

/*
 * This function checks if first `s21_decimal` number is equal to or
 * less than second `s21_decimal` number.
 * If first value is strictly less or equal, than function returns 1,
 * if not - returns 0.
 */

int s21_is_less_or_equal(s21_decimal value_1, s21_decimal value_2) {
  int less_or_equal = 0;
  // Check if equal. If not -> check if less
  if (s21_is_equal(value_1, value_2) == 1) {
    less_or_equal = 1;
  } else {
    less_or_equal = s21_is_less(value_1, value_2);
  }
  return less_or_equal;
}

/*
 * This function checks if first `s21_decimal` number is STRICTLY
 * greater than second `s21_decimal` number.
 * If first value is strictly greater, than function returns 1,
 * if not - returns 0.
 */
int s21_is_greater(s21_decimal value_1, s21_decimal value_2) {
  int greater = 1;
  // If numbers are equal - this is 0 flag
  // If numbers are not equal -> check their signs
  // If signs are equal -> normalize and check mantissa
  if (s21_is_equal(value_1, value_2) == 1) {
    greater = 0;
  } else if (s21_get_sign(value_1) == 0 && s21_get_sign(value_2) == 1) {
    greater = 1;
  } else if (s21_get_sign(value_1) == 1 && s21_get_sign(value_2) == 0) {
    greater = 0;
  } else {
    int sign = s21_get_sign(value_1);
    s21_big_decimal copy_1 = s21_transfer_to_big(value_1);
    s21_big_decimal copy_2 = s21_transfer_to_big(value_2);
    s21_normalize(&copy_1, &copy_2);
    int test = 0;
    // Checks similar to previous s21_is_less function
    for (int i = 2; sign == 1 && i >= 0 && test == 0; i--) {
      if (copy_1.bits[i] < copy_2.bits[i])
        test = 1;
      else if (copy_1.bits[i] > copy_2.bits[i])
        test = 2;
    }
    for (int i = 2; sign == 0 && i >= 0 && test == 0; i--) {
      if (copy_1.bits[i] > copy_2.bits[i])
        test = 1;
      else if (copy_1.bits[i] < copy_2.bits[i])
        test = 2;
    }
    if (test == 1)
      greater = 1;
    else
      greater = 0;
  }
  return greater;
}
/*
 * This function checks if first `s21_decimal` number is equal to or
 * greater than second `s21_decimal` number.
 * If first value is strictly greater or equal, than function returns 1,
 * if not - returns 0.
 */
int s21_is_greater_or_equal(s21_decimal value_1, s21_decimal value_2) {
  int greater_or_equal = 1;
  // Check if equal. If not -> check if greater
  if (s21_is_equal(value_1, value_2) == 1) {
    greater_or_equal = 1;
  } else {
    greater_or_equal = s21_is_greater(value_1, value_2);
  }
  return greater_or_equal;
}
