#include "s21_decimal.h"
#include <stdio.h>

/**
 * @brief Предварительные проверки для check_for_add
 *
 * @param num1 первое слагаемое
 * @param num2 второе слагаемое
 * @return s21_decimal число с выставленным value_type
 */

s21_decimal res_for_add(s21_decimal num1, s21_decimal num2) {
    s21_decimal result = {{0, 0, 0, 0}, s21_NORMAL_VALUE};
    /*
    num1      num2
    +inf      +-normal  = +inf    - в простой проверке
    -inf      +-normal  = -inf    - в простой проверке
    nan       any       = nan     - в простой проверке
    +inf      -inf      = nan     - отдельная
    */

    return result;
}



int get_bit(const s21_decimal a, int bit_number) {
    int result = 0;
    if (bit_number / 32 < 4) {
        unsigned int mask = 1u << (bit_number % 32);
        result = a.bits[bit_number / 32] & mask;
    }
    return !!result;
}

void set_bit(s21_decimal *a, int bit_number, int value) {
    unsigned int mask = 1u << (bit_number % 32);
    if (bit_number / 32 < 4 && value) {
        a->bits[bit_number / 32] = a->bits[bit_number / 32] | mask;
    } else if (bit_number / 32 < 4 && !value) {
        a->bits[bit_number / 32] = a->bits[bit_number / 32] & ~mask;
    }
}

 int get_sign(const s21_decimal *a) {
    unsigned int mask = 1u << 31;
    return !!(a->bits[3] & mask);
}

void set_sign(s21_decimal *a, int sign_value) {
    unsigned int mask = 1u <<31;
    if (sign_value) {
        a->bits[3] |= mask;
    } else {
        a->bits[3] &= ~mask;
    }
}
int get_scale(const s21_decimal *a) {
    return (char)(a->bits[3] >> 16);
}

void set_scale(s21_decimal *varPtr, int scale) {
    if (scale < 0 || scale > 28) {
        printf("wrong scale = %d\n", scale);
    } else {
        int clearMask = ~(0xFF << 16);
        varPtr->bits[3] &= clearMask;
        int mask = scale << 16;
        varPtr->bits[3] |= mask;
    }
}


