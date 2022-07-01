#include "s21_decimal.h"
#include <stdio.h>
#include <string.h>





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
//********************************************************************************************************//

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





/**
 * @brief Смещение влево не затрагивая 31 и 63 бит
 * @param varPtr
 * @param value_offset
 * @return
 */
int offset_left(s21_decimal *num_ptr, int value_offset) { // Смещение влево не затрагивая 31 и 63 бит
//    value_type type;
    int res = NORMAL_VALUE;
    int lastbit = last_bit(*num_ptr);
    if (lastbit + value_offset > 95) {
        res = INFINITY;
    }
    if (res != INFINITY) {
        for (int i = 0; i < value_offset; i++) {
            int value_31bit = get_bit(*num_ptr, 31);
            int value_63bit = get_bit(*num_ptr, 63);
            num_ptr->bits[0] <<= 1;
            num_ptr->bits[1] <<= 1;
            num_ptr->bits[2] <<= 1;
            if (value_31bit) set_bit(num_ptr, 32, value_31bit);
            if (value_63bit) set_bit(num_ptr, 64, value_63bit);
        }
    }
    return res;
}





/**
 * @brief находит последний бит
 * @param number
 * @return номер последнего бита или -1 если все пустые
 */
int last_bit(s21_decimal number) {
    int last_bit = 95;
    for (; last_bit >= 0 && get_bit(number, last_bit) == 0; last_bit--){
    };
    return  last_bit;
}




int zero_check(s21_decimal num1, s21_decimal num2) {
    int is_zero = TRUE;
    s21_decimal *pt1 = &num1;
    s21_decimal *pt2 = &num2;

    if (pt1 && pt2) {
        if (!num1.bits[0] && !num2.bits[0] && !num1.bits[1] && !num2.bits[1] &&
            !num1.bits[2] && !num2.bits[2])
            is_zero = FALSE;
    }
    return is_zero;
}




int scale_equalize(s21_decimal *number1, s21_decimal *number2) {
    s21_decimal *big = NULL;
    s21_decimal  *small = NULL;
    int process = 1;
    if (get_scale(number1) == get_scale(number2)) {
        process = 0;
    } else if (get_scale(number1) > get_scale(number2)) {
        big = number1;
        small = number2;
    } else {
        big = number2;
        small = number1;
    }

    s21_decimal tmp;
    init_struct(&tmp);
    int small_scale = 0;
    int bigger_scale = 0;

    int type = NORMAL_VALUE;
    int err = 0;
    if (get_bit(*big, 95) && get_bit(*small, 95)) err = 1;
    while (get_scale(number1) != get_scale(number2)) {

        if (!err) {
            small_scale = get_scale(small);
            s21_decimal tmp1;
            s21_decimal tmp2;

            tmp1 = *small;
            tmp2 = *small;
            offset_left(&tmp1, 1);
            offset_left(&tmp2, 3);
            tmp = bit_add(&tmp1, &tmp2, err);
            if (get_bit(tmp, 95)) err = 1;
            if (err != 1) {
                copy_bits(tmp, small);
                set_scale(small, small_scale + 1);
            }
        }
        if (err) {
            // деление большего скейла на 10
            s21_decimal ten = {10, 0, 0, 0};
            s21_decimal zero = {0, 0, 0, 0};

            s21_decimal tmp = {0, 0, 0, 0};
            tmp = division_without_scale(*big, ten);
            if (is_equal_b(tmp, zero) != 0) { // tmp не полностью обрезался
                copy_bits(tmp, big);
            } if (is_equal_b(tmp, zero) == 0) { // обрезался, нужны идеи как быть в этой ситуации(мб остаток от деления ставить)
                copy_bits(tmp, big);
            }
            bigger_scale = get_scale(big);
            set_scale(big, bigger_scale - 1);
        }
    }
    return 0;
}




/**
 * @brief Функция переводит число децимал в доп.код
 * @param number_1 указатель на число децимал
 */
void convert_to_addcode(s21_decimal *number_1) {
    s21_decimal res;
    s21_decimal add = {1, 0, 0, 0};
    number_1->bits[0] = ~number_1->bits[0];
    number_1->bits[1] = ~number_1->bits[1];
    number_1->bits[2] = ~number_1->bits[2];
    res = bit_add(number_1, &add,0);
    number_1->bits[0] = res.bits[0];
    number_1->bits[1] = res.bits[1];
    number_1->bits[2] = res.bits[2];
}




void sub_bits_only(s21_decimal *number1, s21_decimal *number2) {
    s21_decimal *addcode_num1 = number1;

}



/**
 * @brief Временная функция для инициализаци структуры которая хранит число
 * Децимал
 * @param varPtr указатель на число децимал
 */
void init_struct(s21_decimal *varPtr) {
    clear_bits(varPtr);
}



void clear_bits(s21_decimal *varPtr) {
    memset(varPtr->bits, 0, sizeof(varPtr->bits));
}




void copy_bits(s21_decimal src, s21_decimal *dest) {
    dest->bits[0] = src.bits[0];
    dest->bits[1] = src.bits[1];
    dest->bits[2] = src.bits[2];
}




/**
 * @brief Деление битов игнорируется степень // НЕ СВОЯ РЕАЛИЗАЦИЯ!
 * @param a число децимал
 * @param b число децимал
 * @param buf остаток от деления
 * @return s21_decimal результат деление (целая часть)
 */
s21_decimal div_only_bits(s21_decimal number_1, s21_decimal number_2,
                          s21_decimal *buf) {
//    init_struct(buf);
//    s21_decimal res = {0, 0, 0, 0};
//    for (int i = last_bit(number_1); i >= 0; i--) {
//        if (get_bit(number_1, i))
//            set_bit(buf, 0, 1);
//        if (s21_is_greater_or_equal(*buf, number_2) == TRUE) {
//            *buf = s21_sub(*buf, number_2);
//            if (i != 0) offset_left(buf, 1);
//            if (get_bit(number_1, i - 1)) set_bit(buf, 0, 1);
//            offset_left(&res, 1);
//            set_bit(&res, 0, 1);
//        } else {
//            offset_left(&res, 1);
//            if (i != 0) offset_left(buf, 1);
//            if ((i - 1) >= 0 && get_bit(number_1, i - 1)) set_bit(buf, 0, 1);
//        }
//    }
//    return res;
}






int s21_sub(s21_decimal value_1, s21_decimal value_2, s21_decimal *result) {
    int value_type = NORMAL_VALUE;

}

s21_decimal division_without_scale(s21_decimal num1, s21_decimal num2) {
    s21_decimal result1 = { 0, 0, 0, 0};
    s21_decimal result = { 0, 0, 0, 0};
    s21_decimal sub = {0, 0, 0, 0};
    s21_decimal tmp = {0, 0, 0, 0};
    s21_decimal one = {1, 0, 0, 0};
    s21_decimal null = {0, 0, 0, 0};
    s21_decimal div_num2 = num2;
    convert_to_addcode(&num2);

    int i = last_bit(num1);
    int flag_offset = 0;
    int flag_offset_result = 0;
    int flag_null = 0;
    int flag_ones = 1;
    for (;i >= 0; i--) {
        if (is_less_b(tmp, div_num2) && !is_equal_b(tmp, div_num2)) {  // для деления столбиком нахожу первое число, которое будет не меньше чем то , на которое делим. дальше делим(вычетанием) tmp
            if (flag_offset == 0) {
                set_bit(&tmp, 0, get_bit(num1, i));
                flag_offset = 1;
                if(is_less_b(tmp, div_num2) && !is_equal_b(tmp, div_num2)) continue;
            } else if (is_equal_b(tmp, null)) {
                set_bit(&tmp, 0, get_bit(num1, i));
                if(is_less_b(tmp, div_num2) && !is_equal_b(tmp, div_num2) && i != 0) {flag_null++; continue;}
            } else {
                offset_left(&tmp, 1);
                set_bit(&tmp, 0, get_bit(num1, i));
                if(is_less_b(tmp, div_num2) && !is_equal_b(tmp, div_num2) && i != 0) {flag_null++; continue;}
            }
        }
        if (i == 0 && !get_bit(num1, 0)) {
            flag_null = 1;
        }

        sub = tmp;
        int counter = 0;
        while (is_greater_or_equal(sub, null) != 0 && sub.bits[0] > 0) {
            sub = bit_add(&sub, &num2, 0);
            if (sub.bits[0] >= 0) result1 = bit_add(&result1, &one, 0);
            counter++;
        }
        if (flag_null && !flag_ones) {
            while (flag_null) {
                offset_left(&result, 1);
                set_bit(&result, 0, 0);
                flag_null--;
            }
        }
        flag_ones = 0;
        flag_null = 0;
        tmp = bit_add(&tmp, &num2, 0);
        for(int j = last_bit(result1); j >= 0; j--) {
            if (flag_offset_result == 0) {
                set_bit(&result, 0, get_bit(result1, j));
                flag_offset_result = 1;
            } else {
                offset_left(&result, 1);
                set_bit(&result, 0, get_bit(result1, j));
            }
        }
        clear_bits(&result1);

    }
    return  result;
}


int is_less_b(s21_decimal num1, s21_decimal num2) {
    return is_greater(num2, num1);
}
int is_greater(s21_decimal num1, s21_decimal num2) {
    int is_greater = -1;
    for (int i = 95; i >= 0 && is_greater == -1; i--) {
        int bit_dec1 = get_bit(num1, i);
        int bit_dec2 = get_bit(num2, i);
        if (bit_dec1 && !bit_dec2)
            is_greater = 1;
        if (bit_dec2 && !bit_dec1)
            is_greater = 0;

        if (is_greater != -1) {
            if (get_sign(&num1) && get_sign(&num2))
                is_greater = !is_greater;
        }
    }
    return is_greater;
}

int is_equal_b(s21_decimal dec1, s21_decimal dec2) {
    int is_equal = -1;
    for (int i = 95; i >= 0 && is_equal == -1; i--) {
        int bit_dec1 = get_bit(dec1, i);
        int bit_dec2 = get_bit(dec2, i);
        if (bit_dec1 != bit_dec2) {
            is_equal = FALSE;
            break;
        }
    }

    return (is_equal != FALSE) ? TRUE : FALSE;
}

int is_greater_or_equal(s21_decimal dec1, s21_decimal dec2) {
    return !!(!is_greater(dec1, dec2) || !is_equal_b(dec1, dec2));
}





//int s21_is_greater(s21_decimal value_1, s21_decimal value_2) {
//    int is_greater = -1;
//}