#include "s21_decimal.h"

/*
 * The function takes a standard 32-bit integer (`src`) as input and converts it
 * into the custom s21_decimal format, storing the result in the provided `dst`
 * pointer. The sign of the integer (positive or negative) is preserved in
 * the resulting decimal representation.
 */

int s21_from_int_to_decimal(int src, s21_decimal *dst) {
  *dst = (s21_decimal){0};
  if (src < 0) {
    dst->bits[3] |= (1 << 31);
    src = -src;
  }
  dst->bits[0] = src;
  return 0;
}

/*
 * This function converts the given `s21_decimal` number (`src`) to its
 * integer representation. If fucntion can convert, it converts the decimal
 * number into an integer and places it in the `dst` pointer.
 */

int s21_from_decimal_to_int(s21_decimal src, int *dst) {
  int returnFlag = 1;
  if (*dst == 0) {
    *dst = src.bits[0];
    *dst = *dst * ((src.bits[3] >> 31) & 1 ? -1 : 1);
    returnFlag = 0;
  }
  return returnFlag;
}

/*
 * This function converts the given single-precision floating-point number
 * (`src`) to its corresponding `s21_decimal` representation, storing the result
 * in the provided `dst` pointer. The function handles the sign, integer, and
 * fractional parts separately and ensures accurate representation in the
 * `s21_decimal` format. If the input number is beyond the representational
 * capacity of `s21_decimal`, the function returns 0.
 */
int s21_from_float_to_decimal(float src, s21_decimal *dst) {
  int returnFlag = 1;
  dst->bits[0] = dst->bits[1] = dst->bits[2] = dst->bits[3] = 0;
  if (!isinf(src) && !isnan(src)) {
    if (src != 0) {
      int sign = *(int *)&src >> 31;
      int exponent = ((*(int *)&src & ~0x80000000) >> 23) - 127;
      double temp = (double)fabs(src);
      int off = 0;

      while (off < 28 && (int)temp / (int)pow(2, 21) == 0) {
        temp *= 10;
        off++;
      }

      temp = round(temp);

      if (off <= 28 && (exponent > -94 && exponent < 96)) {
        s21_float_bits mantissa = {0};
        temp = (float)temp;

        while (fmod(temp, 10) == 0 && off > 0) {
          off--;
          temp /= 10;
        }

        mantissa.floa_t = temp;
        exponent = ((*(int *)&mantissa.floa_t & ~0x80000000) >> 23) - 127;
        dst->bits[exponent / 32] |= 1 << exponent % 32;

        for (int i = exponent - 1, j = 22; j >= 0; i--, j--)
          if ((mantissa.uin_t & (1 << j)) != 0)
            dst->bits[i / 32] |= 1 << i % 32;

        dst->bits[3] = (sign << 31) | (off << 16);
      }
    }
    returnFlag = 0;
  }
  return returnFlag;
}

/*
 * This function takes a given `s21_decimal` number (`src`) and converts it
 * to its float representation, storing the result in the provided `dst` pointer
 * The function assumes that the caller provides a valid memory location.)
 */
int s21_from_decimal_to_float(s21_decimal src, float *dst) {
  double temp = (double)*dst;
  for (int i = 0; i < 96; i++) {
    temp += find_bit(src, i) * pow(2, i);
  }
  int mask = 127 << 16;
  int scale = (mask & src.bits[3]) >> 16;
  temp = temp * pow(10, -scale);
  if (find_bit(src, 127)) temp = temp * (-1);
  *dst = temp;
  return 0;
}
