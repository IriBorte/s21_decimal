#include "s21_decimal.h"

// Округляет указанное Decimal число до ближайшего целого числа
// в сторону отрицательной бесконечности.
int s21_floor(s21_decimal value, s21_decimal *result) {
  // Получаем знак
  bool sign = s21_get_sign(value);

  // Берем целое из числа
  s21_truncate(value, result);

  // Округляем в сторону -бесконечности
  if (sign) {
    s21_decimal one = {{1, 0, 0, 0}};
    s21_sub(*result, one, result);
  }
  return 0;
}

// Округляет Decimal до ближайшего целого числа.
int s21_round(s21_decimal value, s21_decimal *result) {
  // Копируем в big decimal
  s21_big_decimal copy = s21_transfer_to_big(value);

  // Нормализуем число, пока exp != 0
  int fraction_part = 0;
  while (s21_get_exp(s21_transfer_to_decimal(copy)))
    fraction_part = s21_point_right(&copy);

  // Берем целое из числа
  s21_truncate(value, result);

  // Округляем в сторону +бесконечности
  if (fraction_part > 4) {
    s21_decimal one = {{1, 0, 0, 0}};
    s21_add(*result, one, result);
  }
  return 0;
}

// Возвращает целые цифры указанного Decimal числа.
// любые дробные цифры отбрасываются, включая конечные нули.
int s21_truncate(s21_decimal value, s21_decimal *result) {
  // Копируем в big decimal и получаем знак
  s21_big_decimal temp = s21_transfer_to_big(value);
  bool sign = s21_get_sign(value);

  // Нормализуем число, пока exp != 0
  while (s21_get_exp(s21_transfer_to_decimal(temp))) s21_point_right(&temp);

  // Переводим в decimal и ставим знак
  *result = s21_transfer_to_decimal(temp);
  s21_set_sign(result, sign);
  return 0;
}

// Возвращает результат умножения указанного Decimal на -1.
int s21_negate(s21_decimal value, s21_decimal *result) {
  *result = value;
  result->bits[3] ^= 1 << 31;
  return 0;
}
