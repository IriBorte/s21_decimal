#include "s21_decimal.h"

// Проверка мантиссы на 0
bool s21_is_zero(s21_decimal src) {
  return (src.bits[0] == 0 && src.bits[1] == 0 && src.bits[2] == 0) ? 1 : 0;
}

// Проверка экспоненты
bool s21_check_exp(s21_decimal src) { return (s21_get_exp(src) > 28) ? 1 : 0; }

// Получение exp (scale to 10) в десятичном виде
int s21_get_exp(s21_decimal src) { return (src.bits[3] & EXP) >> 16; }

// Установка exp
void s21_set_exp(s21_decimal *src, uint16_t exp) {
  src->bits[3] |= (exp << 16);
}

// Получение знака
bool s21_get_sign(s21_decimal src) { return s21_get_bit(src.bits[3], 31); }

// Установка знака
void s21_set_sign(s21_decimal *src, bool sign) {
  src->bits[3] |= sign ? MINUS : 0;
}

// Получение бита по индексу от 0 до 31
bool s21_get_bit(unsigned int src, unsigned int index) {
  return (src >> index) & 1;
}

// Найти бит
int find_bit(s21_decimal src, int index) {
  int mask = 1u << (index % 32);
  return (src.bits[index / 32] & mask) != 0;
}

// Конвертирование s21_decimal в s21_big_decimal
s21_big_decimal s21_transfer_to_big(s21_decimal src) {
  s21_big_decimal res = {0};
  uint16_t exp = s21_get_exp(src);
  for (int i = 0; i < 3; i++) res.bits[i] = src.bits[i] & UINT_MAX;
  res.exp = exp;
  return res;
}

// Конвертирование s21_big_decimal в s21_decimal
s21_decimal s21_transfer_to_decimal(s21_big_decimal src) {
  s21_decimal res = {0};
  for (int i = 0; i < 3; i++) res.bits[i] = src.bits[i] & UINT_MAX;
  s21_set_exp(&res, src.exp);
  return res;
}

// Приведение к общему множителю (экспоненте)
bool s21_normalize(s21_big_decimal *value_1, s21_big_decimal *value_2) {
  bool res = 0;
  if (value_1->exp == value_2->exp) return res;
  if (value_1->exp > value_2->exp)
    while (value_2->exp < value_1->exp) res = s21_point_left(value_2);
  else
    while (value_1->exp < value_2->exp) res = s21_point_left(value_1);
  return res;
}

// Сдвиг влево (exp++), или домножение на 10, не меняя числа.
// При переполнении возвращает 1.
bool s21_point_left(s21_big_decimal *src) {
  bool overflow = 0;
  s21_big_decimal copy = *src;
  for (int i = 0; i < 7; i++) copy.bits[i] *= 10;
  copy.exp++;
  overflow = s21_check_overflow(&copy);
  if (!overflow && src->exp < 28) *src = copy;
  return overflow;
}

// Сдвиг вправо (exp--), т.е. деление на 10, не меняя числа.
uint32_t s21_point_right(s21_big_decimal *src) {
  uint32_t remainder = 0;
  if (s21_get_exp(s21_transfer_to_decimal(*src)) > 0) {
    s21_big_decimal copy = *src;
    for (int i = 6; i >= 0; i--) {
      copy.bits[i] += (uint64_t)remainder << 32;
      remainder = copy.bits[i] % 10;
      copy.bits[i] /= 10;
    }
    copy.exp--;
    *src = copy;
  }
  return remainder;
}

// Проверка переполнения
bool s21_check_overflow(s21_big_decimal *src) {
  uint32_t overflow = 0;
  for (int i = 0; i < 7; i++) {
    src->bits[i] += overflow;
    overflow = src->bits[i] >> 32;
    src->bits[i] &= UINT_MAX;
  }
  return s21_is_overflow(*src);
}

// Прогоняет биты на переполнение
bool s21_is_overflow(s21_big_decimal src) {
  bool res = 0;
  for (int i = 3; i < 7 && !res; i++) res = src.bits[i] ? 1 : 0;
  return res;
}

// Сложение big decimal
bool s21_big_add(s21_big_decimal value_1, s21_big_decimal value_2,
                 s21_big_decimal *result) {
  bool res = 0;
  s21_big_decimal copy = {{0}, 0};
  if (value_1.exp >= value_2.exp) {
    copy.exp = value_1.exp;
  } else {
    copy.exp = value_2.exp;
  }

  int rem = 0, ovf = 0;
  for (int i = 0; i < 256; i++) {
    rem = s21_get_bit_big(value_1, i) + s21_get_bit_big(value_2, i) + ovf;
    ovf = rem / 2;
    s21_set_bit_big(&copy, i, rem % 2);
  }
  res = s21_check_overflow(&copy);
  if (res) res = s21_big_okrug(&copy);
  *result = copy;

  return res;
}

// Вычитание big decimal
void s21_big_sub(s21_big_decimal value_1, s21_big_decimal value_2,
                 s21_big_decimal *result) {
  s21_big_decimal copy = value_1;
  bool save = 0;

  for (int i = 0; i < 3; i++) {
    copy.bits[i] += save ? ~(value_2.bits[i] + 1) + 1 : ~value_2.bits[i] + 1;
    save = 0;
    if (value_1.bits[i] < value_2.bits[i]) save = 1;
  }
  *result = copy;
}

// Если есть заполненные биты старше 3 и exp != 0, то делим на 10
// Если поделить не получилось, то return 1
bool s21_big_okrug(s21_big_decimal *src) {
  bool res = 0;
  s21_big_decimal copy = *src;
  uint16_t remainders[28] = {0};
  uint16_t i = 0;
  while ((copy.exp && s21_is_overflow(copy)) || copy.exp > 28)
    remainders[i++] = s21_point_right(&copy);
  if (copy.exp == 0 && s21_is_overflow(copy)) {
    res = 1;
  } else {
    copy.bits[0] = s21_bank_round(copy.bits[0], remainders, i);
    *src = copy;
  }
  return res;
}

// Банковское округление
uint32_t s21_bank_round(uint32_t src, uint16_t num[], uint16_t index) {
  uint32_t res = 0;
  if (num[index] > 5) {
    res = src + 1;
  } else if (num[index] < 5) {
    res = src;
  } else {
    while (index)
      if (num[--index]) res = src + 1;
    if (src & 0)  // 1 если четное
      res = src;
    else
      res = src + 1;
  }
  return res;
}

// Установка бита в большом децимале
void s21_set_bit_big(s21_big_decimal *dec, int pos, int n) {
  if (n == 1) {
    dec->bits[pos / 32] |= 1u << (pos % 32);
  } else {
    dec->bits[pos / 32] &= ~(1u << (pos % 32));
  }
}

// Получение бита в большом децимале
int s21_get_bit_big(s21_big_decimal dec, int pos) {
  return ((dec.bits[pos / 32] >> (pos % 32)) & 1u);
}