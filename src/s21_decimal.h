#ifndef SRC_S21_DECIMAL_H_
#define SRC_S21_DECIMAL_H_

#include <limits.h>  // UINT_MAX
#include <math.h>
#include <stdbool.h>
#include <stdint.h>  // uint64_t, uint32_t, uint16_t
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MINUS 0x80000000
#define EXP 0x00ff0000

typedef struct {
  uint32_t bits[4];
} s21_decimal;

typedef struct {
  uint64_t bits[8];
  uint16_t exp;
} s21_big_decimal;

// Для преобразования из float в decimal
typedef union {
  int uin_t;
  float floa_t;
} s21_float_bits;

int s21_add(s21_decimal value_1, s21_decimal value_2, s21_decimal *result);
int s21_sub(s21_decimal value_1, s21_decimal value_2, s21_decimal *result);
int s21_mul(s21_decimal value_1, s21_decimal value_2, s21_decimal *result);
int s21_div(s21_decimal value_1, s21_decimal value_2, s21_decimal *result);

int s21_is_less(s21_decimal, s21_decimal);
int s21_is_less_or_equal(s21_decimal, s21_decimal);
int s21_is_greater(s21_decimal, s21_decimal);
int s21_is_greater_or_equal(s21_decimal, s21_decimal);
int s21_is_equal(s21_decimal, s21_decimal);
int s21_is_not_equal(s21_decimal, s21_decimal);

int s21_from_int_to_decimal(int src, s21_decimal *dst);
int s21_from_float_to_decimal(float src, s21_decimal *dst);
int s21_from_decimal_to_int(s21_decimal src, int *dst);
int s21_from_decimal_to_float(s21_decimal src, float *dst);

// ------ Другие функции ------
// Округляет указанное Decimal число до ближайшего целого числа в сторону
// отрицательной бесконечности.
int s21_floor(s21_decimal value, s21_decimal *result);
// Округляет Decimal до ближайшего целого числа.
int s21_round(s21_decimal value, s21_decimal *result);
// Возвращает целые цифры указанного Decimal числа; любые дробные цифры
// отбрасываются, включая конечные нули.
int s21_truncate(s21_decimal value, s21_decimal *result);
// Возвращает результат умножения указанного Decimal на -1.
int s21_negate(s21_decimal value, s21_decimal *result);

// ------ Вспомогательные ------
// Проверка мантиссы на 0
bool s21_is_zero(s21_decimal src);
// Проверка экспоненты
bool s21_check_exp(s21_decimal src);

// Получение exp (scale to 10) в десятичном виде
int s21_get_exp(s21_decimal src);
// Установка exp
void s21_set_exp(s21_decimal *src, uint16_t exp);
// Получение знака
bool s21_get_sign(s21_decimal src);
// Установка знака
void s21_set_sign(s21_decimal *src, bool sign);
// Получение бита по индексу от 0 до 31
bool s21_get_bit(unsigned int src, unsigned int index);
// Найти бит
int find_bit(s21_decimal src, int index);

// Конвертирование s21_decimal в s21_big_decimal
s21_big_decimal s21_transfer_to_big(s21_decimal src);
// Конвертирование s21_big_decimal в s21_decimal
s21_decimal s21_transfer_to_decimal(s21_big_decimal src);

// Проверка переполнения
bool s21_check_overflow(s21_big_decimal *src);
// Прогоняет биты на переполнение
bool s21_is_overflow(s21_big_decimal src);
// Приведение к общему множителю (экспоненте)
bool s21_normalize(s21_big_decimal *value_1, s21_big_decimal *value_2);
// Сдвигает число, пока exp != 0
void s21_shift_to_zero(s21_big_decimal *src);
// Сдвиг влево (exp++), т.е. домножение на 10, не меняя числа.
// При переполнении возвращает 1.
bool s21_point_left(s21_big_decimal *src);
// Сдвиг вправо (exp--), т.е. деление на 10, не меняя числа.
uint32_t s21_point_right(s21_big_decimal *src);

// Сложение big decimal
bool s21_big_add(s21_big_decimal value_1, s21_big_decimal value_2,
                 s21_big_decimal *result);
// Вычитание big decimal
void s21_big_sub(s21_big_decimal value_1, s21_big_decimal value_2,
                 s21_big_decimal *result);

// Если есть заполненные биты старше 3 и exp != 0, то делим на 10
// Если поделить не получилось, то return 1
bool s21_big_okrug(s21_big_decimal *src);

// Банковское округление
uint32_t s21_bank_round(uint32_t src, uint16_t num[], uint16_t index);

// Установка бита в большом децимале
void s21_set_bit_big(s21_big_decimal *dec, int pos, int n);
// Получение бита в большом децимале
int s21_get_bit_big(s21_big_decimal dec, int pos);
// Большой шифт влево
int s21_big_shift_left(s21_big_decimal *dec, int num);
// Большой шифт вправо
void s21_big_shift_right(s21_big_decimal *dec, int num);
// Поиск децимала с наибольшей мантиссой
void s21_find_highest_big_decimal(s21_big_decimal value_1,
                                  s21_big_decimal value_2, int *bit_1,
                                  int *bit_2);
// Обнуление большого децимала
void s21_zero_big_decimal(s21_big_decimal *dst);
// Проверка на нулевой децимал
int s21_is_big_decimal_not_empty(s21_big_decimal dst);
// Уменьшение скейла
// void s21_decreace_scale_big_decimal(s21_big_decimal *dst, int n);
// Увеличение скейла
// void s21_increase_scale_big_decimal(s21_big_decimal *dst, int n);
// Проверка большего большого децимала
int s21_is_greater_big_decimal(s21_big_decimal value_1,
                               s21_big_decimal value_2);
// Проверка большего или равного большого децимала
int s21_is_greater_or_equal_big_decimal(s21_big_decimal value_1,
                                        s21_big_decimal value_2);

// ------ Функции для деления ------
typedef struct {
  bool bits[192];
  bool sign;
  int exp;
} s21_decimal_alt;

int s21_div_alt(s21_decimal_alt alt_value_1, s21_decimal_alt alt_value_2,
                s21_decimal_alt *alt_result);
void init_decimal(s21_decimal *decimal);
bool s21_get_bit_int(unsigned int num, int pos);
bool s21_get_sign_std(s21_decimal dec);
int s21_get_exp_std(s21_decimal dec);
s21_decimal_alt s21_div_with_modulo(s21_decimal_alt alt_value_1,
                                    s21_decimal_alt alt_value_2,
                                    s21_decimal_alt *alt_result);
void s21_rescale_alt_to_zero(s21_decimal_alt *alt_value_1,
                             s21_decimal_alt *alt_value_2);
bool s21_is_null(s21_decimal_alt alt);
void s21_null_decimal_alt(s21_decimal_alt *alt);
bool s21_compare_bits(s21_decimal_alt alt1, s21_decimal_alt alt2);
int s21_last_bit(s21_decimal_alt alt);
void s21_align(s21_decimal_alt *alt_value_1, s21_decimal_alt *alt_value_2);
int s21_div_by_ten(s21_decimal_alt *alt);
void s21_bank_rounding(s21_decimal_alt *alt, int mod);
void s21_right_shift(s21_decimal_alt *alt);
void s21_left_shift(s21_decimal_alt *alt);
s21_decimal_alt s21_convert_std_to_alt(s21_decimal std);
s21_decimal s21_convert_alt_to_std(s21_decimal_alt alt);
int s21_mul_alt(s21_decimal_alt alt_value_1, s21_decimal_alt alt_value_2,
                s21_decimal_alt *alt_result);
int s21_add_alt(s21_decimal_alt alt_value_1, s21_decimal_alt alt_value_2,
                s21_decimal_alt *alt_result);
int s21_sub_alt(s21_decimal_alt alt_value_1, s21_decimal_alt alt_value_2,
                s21_decimal_alt *alt_result);

#endif  // SRC_S21_DECIMAL_H_
