#include "s21_decimal.h"

// div functions
int s21_div_alt(s21_decimal_alt alt_value_1, s21_decimal_alt alt_value_2,
                s21_decimal_alt *alt_result) {
  s21_decimal_alt ten;
  s21_null_decimal_alt(&ten);
  ten.bits[1] = 1;
  ten.bits[3] = 1;
  s21_null_decimal_alt(alt_result);
  int res = 0;
  bool sign = alt_value_1.sign ^ alt_value_2.sign;
  alt_value_1.sign = 0;
  alt_value_2.sign = 0;

  s21_rescale_alt_to_zero(&alt_value_1, &alt_value_2);
  s21_decimal_alt modulo;
  s21_null_decimal_alt(&modulo);
  modulo = s21_div_with_modulo(alt_value_1, alt_value_2, &alt_value_1);
  int exp = 0;
  int status = 0;
  while (!s21_is_null(alt_value_1) || !s21_is_null(modulo)) {
    exp++;
    s21_mul_alt(modulo, ten, &modulo);
    s21_mul_alt(*alt_result, ten, alt_result);
    status = s21_add_alt(*alt_result, alt_value_1, alt_result);
    modulo = s21_div_with_modulo(modulo, alt_value_2, &alt_value_1);
    if (status == 1) break;
  }
  int mod;
  if (status == 1) {
    mod = s21_div_by_ten(alt_result);
    while (s21_last_bit(*alt_result) > 95 && exp > 0) {
      mod = s21_div_by_ten(alt_result);
      exp--;
    }
    s21_bank_rounding(alt_result, mod);
    exp--;
  }
  alt_result->exp = exp - 1;
  mod = 10;
  while (alt_result->exp > 28) mod = s21_div_by_ten(alt_result);
  if (mod != 10) s21_bank_rounding(alt_result, mod);
  if (s21_last_bit(*alt_result) > 95) {
    if (sign)
      res = 2;
    else
      res = 1;
  }
  alt_result->sign = sign;
  return res;
}

void init_decimal(s21_decimal *decimal) {
  for (int i = 0; i < 4; i++) {
    decimal->bits[i] = 0;
  }
}

bool s21_get_bit_int(unsigned int num, int pos) { return (num >> pos) & 1; }

bool s21_get_sign_std(s21_decimal dec) {
  return s21_get_bit_int(dec.bits[3], 31);
}

int s21_get_exp_std(s21_decimal dec) {
  return (dec.bits[3] % 2147483648) >> 16;
}

s21_decimal_alt s21_div_with_modulo(s21_decimal_alt alt_value_1,
                                    s21_decimal_alt alt_value_2,
                                    s21_decimal_alt *alt_result) {
  int denominator_left_bit = s21_last_bit(alt_value_2);
  s21_decimal_alt modulo;
  s21_null_decimal_alt(&modulo);
  s21_null_decimal_alt(alt_result);
  if (s21_compare_bits(alt_value_1, alt_value_2)) {
    s21_align(&alt_value_1, &alt_value_2);
  }
  for (int i = 0; i < 191; i++) {
    if (s21_compare_bits(alt_value_1, alt_value_2)) {
      s21_sub_alt(alt_value_1, alt_value_2, &alt_value_1);
      alt_result->bits[0] = 1;
    }
    if (s21_last_bit(alt_value_2) <= denominator_left_bit) break;
    if (s21_last_bit(alt_value_2) > denominator_left_bit)
      s21_right_shift(&alt_value_2);
    s21_left_shift(alt_result);
  }
  return alt_value_1;
}

void s21_rescale_alt_to_zero(s21_decimal_alt *alt_value_1,
                             s21_decimal_alt *alt_value_2) {
  s21_decimal_alt ten;
  s21_null_decimal_alt(&ten);
  ten.bits[1] = 1;
  ten.bits[3] = 1;
  if (alt_value_1->exp > alt_value_2->exp) {
    alt_value_1->exp -= alt_value_2->exp;
    alt_value_2->exp = 0;
    while (alt_value_1->exp != 0) {
      alt_value_1->exp--;
      s21_mul_alt(*alt_value_2, ten, alt_value_2);
    }
  } else {
    alt_value_2->exp -= alt_value_1->exp;
    alt_value_1->exp = 0;
    while (alt_value_2->exp != 0) {
      alt_value_2->exp--;
      s21_mul_alt(*alt_value_1, ten, alt_value_1);
    }
  }
}

bool s21_is_null(s21_decimal_alt alt) {
  bool result = 0;
  for (int i = 0; i < 192; i++) result += alt.bits[i];
  return !result;
}

void s21_null_decimal_alt(s21_decimal_alt *alt) {
  alt->sign = 0;
  alt->exp = 0;
  for (int i = 0; i < 192; i++) alt->bits[i] = 0;
}

bool s21_compare_bits(s21_decimal_alt alt1, s21_decimal_alt alt2) {
  int i = 191;
  while (i >= 0 && alt1.bits[i] == alt2.bits[i]) {
    i--;
  }
  return i == -1 ? 1 : alt1.bits[i];
}

int s21_last_bit(s21_decimal_alt alt) {
  int i = -1;
  if (!s21_is_null(alt)) {
    for (i = 191; i >= 0; i--)
      if (alt.bits[i] == 1) break;
  }
  return i;
}

void s21_align(s21_decimal_alt *alt_value_1, s21_decimal_alt *alt_value_2) {
  if (!s21_is_null(*alt_value_1) && !s21_is_null(*alt_value_2)) {
    int l1 = s21_last_bit(*alt_value_1);
    int l2 = s21_last_bit(*alt_value_2);
    while (s21_last_bit(*alt_value_1) != s21_last_bit(*alt_value_2))
      if (s21_compare_bits(*alt_value_1, *alt_value_2))
        s21_left_shift(alt_value_2);
      else
        s21_left_shift(alt_value_1);
    if (!s21_compare_bits(*alt_value_1, *alt_value_2))
      s21_left_shift(alt_value_1);
    if (alt_value_1->bits[0] == 0 && alt_value_2->bits[0] == 0 &&
        s21_last_bit(*alt_value_1) > l1 && s21_last_bit(*alt_value_2) > l2) {
      s21_right_shift(alt_value_1);
      s21_right_shift(alt_value_2);
    }
  }
}

int s21_div_by_ten(s21_decimal_alt *alt) {
  s21_decimal_alt result;
  s21_null_decimal_alt(&result);
  int exp = alt->exp;
  int sign = alt->sign;
  result.sign = 0;
  result.exp = 0;
  s21_decimal_alt ten;
  s21_null_decimal_alt(&ten);
  ten.bits[1] = 1;
  ten.bits[3] = 1;
  s21_decimal_alt modulo = s21_div_with_modulo(*alt, ten, alt);
  int res = s21_convert_alt_to_std(modulo).bits[0];
  alt->exp = exp - 1;
  alt->sign = sign;
  return res;
}

void s21_bank_rounding(s21_decimal_alt *alt, int mod) {
  if ((mod == 5 && alt->bits[0]) || mod > 5) {
    s21_decimal_alt one;
    s21_null_decimal_alt(&one);
    one.bits[0] = 1;
    one.exp = alt->exp;
    one.sign = alt->sign;
    s21_add_alt(*alt, one, alt);
    if (s21_last_bit(*alt) > 95) {
      s21_sub_alt(*alt, one, alt);
      mod = s21_div_by_ten(alt);
      s21_bank_rounding(alt, mod);
    }
  }
}

void s21_right_shift(s21_decimal_alt *alt) {
  for (int i = 0; i < 191; i++) alt->bits[i] = alt->bits[i + 1];
  alt->bits[191] = 0;
}

void s21_left_shift(s21_decimal_alt *alt) {
  for (int i = 191; i > 0; i--) alt->bits[i] = alt->bits[i - 1];
  alt->bits[0] = 0;
}

s21_decimal_alt s21_convert_std_to_alt(s21_decimal std) {
  s21_decimal_alt alt;
  s21_null_decimal_alt(&alt);
  alt.sign = s21_get_sign_std(std);
  alt.exp = s21_get_exp_std(std);
  int i = 0;
  for (int j = 0; j < 3; j++) {
    for (int k = 0; k < 32; k++)
      alt.bits[i++] = s21_get_bit_int(std.bits[j], k);
  }
  return alt;
}

s21_decimal s21_convert_alt_to_std(s21_decimal_alt alt) {
  s21_decimal std;
  init_decimal(&std);
  int k = 95;
  for (int i = 2; i >= 0; i--)
    for (int j = 31; j >= 0; j--)
      std.bits[i] = (std.bits[i] << 1) + alt.bits[k--];
  std.bits[3] = alt.sign;
  std.bits[3] <<= 7;
  k = 128;
  for (int i = 7; i >= 0; i--) {
    std.bits[3] <<= 1;
    if (alt.exp >= k) {
      alt.exp -= k;
      std.bits[3]++;
    }
    k >>= 1;
  }
  std.bits[3] <<= 16;
  return std;
}

int s21_mul_alt(s21_decimal_alt alt_value_1, s21_decimal_alt alt_value_2,
                s21_decimal_alt *alt_result) {
  int res = 0;
  s21_null_decimal_alt(alt_result);
  if (!s21_is_null(alt_value_1) && !(s21_is_null(alt_value_2))) {
    for (int i = 0; i < 192; i++) {
      if (alt_value_2.bits[i] == 1)
        s21_add_alt(*alt_result, alt_value_1, alt_result);
      s21_left_shift(&alt_value_1);
    }
    alt_result->exp = alt_value_1.exp + alt_value_2.exp;
    alt_result->sign = alt_value_1.sign ^ alt_value_2.sign;
    if (s21_last_bit(*alt_result) > 95 || alt_result->exp > 28) {
      int mod = 0;
      while (alt_result->exp > 0 &&
             (s21_last_bit(*alt_result) > 95 || alt_result->exp > 28)) {
        if (alt_result->exp == 0) break;
        mod = s21_div_by_ten(alt_result);
      }
      if (alt_result->exp > 0)
        s21_bank_rounding(alt_result, mod);
      else if (alt_result->sign)
        res = 2;
      else
        res = 1;
    }
  }
  return res;
}

int s21_add_alt(s21_decimal_alt alt_value_1, s21_decimal_alt alt_value_2,
                s21_decimal_alt *alt_result) {
  int res = 0;
  bool t_bit = 0;
  s21_null_decimal_alt(alt_result);
  for (int i = 0; i < 192; i++) {
    alt_result->bits[i] = (alt_value_1.bits[i] ^ alt_value_2.bits[i]) ^ t_bit;
    if ((alt_value_1.bits[i] & alt_value_2.bits[i]) ||
        (alt_value_1.bits[i] & t_bit) || (alt_value_2.bits[i] & t_bit))
      t_bit = 1;
    else
      t_bit = 0;
  }
  alt_result->exp = alt_value_1.exp;
  alt_result->sign = alt_value_1.sign;
  if (s21_last_bit(*alt_result) > 95) {
    if (alt_result->exp <= 0) {
      res = 1;
    } else {
      int mod = s21_div_by_ten(alt_result);
      s21_bank_rounding(alt_result, mod);
    }
  }
  return res;
}

int s21_sub_alt(s21_decimal_alt alt_value_1, s21_decimal_alt alt_value_2,
                s21_decimal_alt *alt_result) {
  int res = 0;
  bool t_bit = 0;
  for (int i = 0; i < 192; i++) {
    alt_result->bits[i] = alt_value_1.bits[i] ^ alt_value_2.bits[i] ^ t_bit;
    if (alt_value_1.bits[i] == 0 && alt_value_2.bits[i] == 1)
      t_bit = 1;
    else if (alt_value_1.bits[i] == 1 && alt_value_2.bits[i] == 0)
      t_bit = 0;
  }
  return res;
}
