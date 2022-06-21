#include "s21_decimal.h"
#include <stdio.h>

/**
 * @brief Предварительные проверки для check_for_add
 *
 * @param num1 первое слагаемое
 * @param num2 второе слагаемое
 * @return s21_decimal число с выставленным value_type
 */



s21_decimal bit_add(s21_decimal *a, s21_decimal *b, int error_code) {
    s21_decimal result = {0 ,0, 0, 0};
    size_t buffer = 0;
    for (int i = 0; i < 96; i++) {
        int current_bit_a = get_bit(*a, i);
        int current_bit_b = get_bit(*b, i);

        if (!current_bit_a && !current_bit_b) { // оба бита выключены
            if (buffer) { // если в буфере что-то есть значит мы повышаем порядок, т.е. прошлый бит 0 , а этот будет 1
                set_bit(&result, i, 1);
                buffer = 0;
            } else {
                set_bit(&result, i, 0);
            }
        } else if (current_bit_a != current_bit_b) { // Один включен
            if (buffer) {
                set_bit(&result, i, 0);
                buffer = 1;
            } else {
                set_bit(&result, i, 1);
            }
        } else { // Оба вкл
            if (buffer) {
                set_bit(&result, i, 1);
            } else {
                set_bit(&result, i, 0);
                buffer = 1;
            }
        }

        if (i == 95 && buffer == 1) {
            error_code = 1;
        }
    }

    return result;
}

int s21_add(s21_decimal value_1, s21_decimal value_2, s21_decimal *result) {
    int solution = 0;
    // написать чек на допустимость значения (хз надо ли)
    // пока обрабатывает только целые числа(scale на подходе)
    if (!get_sign(&value_1) && !get_sign(&value_2)) {

        if (get_scale(&value_1) != get_scale(&value_2)) {
            // scale...
        }
        int err;
        *result = bit_add(&value_1, &value_2, err);
        if (err == 1) {
            // infinity
            solution = 1;
        }
    } else if (get_sign(&value_1) && !get_sign(&value_2)) {
        // нужно вычитание
    } else if (!get_sign(&value_1) && get_sign(&value_2)) {
        // нужно вычитание
    } else {
        // оба отрицательных
        set_sign(&value_1, 0);
        set_sign(&value_2, 0);
        // нужно сложить каким-то образом(bit_add)
        set_sign(&result, 1);
    }


    return solution;
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


