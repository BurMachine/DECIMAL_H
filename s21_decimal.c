
#include "s21_decimal.h"
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <limits.h>
#include <stdbool.h>


//int s21_add(s21_decimal value_1, s21_decimal value_2, s21_decimal *result) {
//    int solution = 0;
//    int scale = 0;
//    int err = 0;
//    // написать чек на допустимость значения (хз надо ли)
//    // пока обрабатывает только целые числа(scale на подходе)
//    if (!get_sign(&value_1) && !get_sign(&value_2)) {
//        if (get_scale(&value_1) != get_scale(&value_2)) {
//            scale = scale_equalize(&value_1, &value_2);
//        } else {
//            scale = get_scale(&value_1);
//        }
//
//        *result = bit_add(&value_1, &value_2, &err);
//        if (err == 1) {
//            // infinity
//            solution = 1;
//        }
//    } else if (get_sign(&value_1) && !get_sign(&value_2)) { // первое отрицательное, второе положительное
//        s21_decimal tmp_1 = value_1;
//        set_sign(&value_1, 0);
//        set_scale(&tmp_1, get_scale(&value_1));
//        if (get_scale(&tmp_1) != get_scale(&value_2)) {
//            scale = scale_equalize(&tmp_1, &value_2);
//        }
//        s21_sub(value_2, value_1, result);
//        if (s21_is_greater(value_1, value_2) && !s21_is_equal(value_1, value_2)) {
//            set_bit(result, 127, 1);
//        }
//    } else if (!get_sign(&value_1) && get_sign(&value_2)) {
//        s21_decimal tmp_1 = value_2;
//        set_sign(&value_2, 0);
//        set_scale(&tmp_1, get_scale(&value_2));
//        if (get_scale(&tmp_1) != get_scale(&value_1)) {
//            scale = scale_equalize(&tmp_1, &value_1);
//        }
//        s21_sub(value_1, value_2, result);
//        if (!s21_is_greater(value_1, value_2)) {
//            set_bit(result, 127, 1);
//        }
//    } else {
//        // оба отрицательных
//        set_sign(&value_1, 0);
//        set_sign(&value_2, 0);
//        if (get_scale(&value_1) != get_scale(&value_2)) {
//            scale = scale_equalize(&value_1, &value_2);
//        }
//        int err = 0;
//        *result = bit_add(&value_1, &value_2, &err);
//        if (err == 1) {
//            // negative_infinity
//            solution = 2;
//        }
//        set_sign(result, 1);
//    }
//    set_scale(result, scale);
////    scale_equalize(&value_1, &value_2);
////    if (solution == 1) {
////        s21_decimal zero = {{0, 0, 0, 0}};
////        copy_bits(zero, result);
////    }
//    return solution;
//}







int eq_zero(s21_decimal value) {
    return (value.bits[0] == 0 && value.bits[1] == 0 && value.bits[2] == 0);
}
int eq_zerol(s21_decimal value) {
    return (value.bits[0] == 0 && value.bits[1] == 0 && value.bits[2] == 0 &&
            value.bits[3] == 0);
}

int max(int a, int b) {
    return a > b ? a : b;
}
int min(int a, int b) {
    return a < b ? a : b;
}
s21_decimal binary_multiplication(s21_decimal value_1, s21_decimal value_2, int *err) {
    *err = ARITHMETIC_OK;
    s21_decimal result = {0};
    while (!eq_zero(value_2) && !(*err)) {
        if (!eq_zero(bit_and(value_2, get_power_of_ten(0)))) {
            result = binary_addition(result, value_1, err);
            if (*err)
                break;
        }

        *err = shiftl(&value_1);
        shiftr(&value_2);
    }
    return result;
}

void s21_normalize_decimal_pair(s21_decimal *a, s21_decimal *b, int *overflow) {
    int e1 = get_exponent(*a), e2 = get_exponent(*b);
    int cur_exp = min(e1, e2);
    int target_exp = max(e1, e2);

    s21_decimal cur = (e1 < e2) ? *a : *b;

    while (cur_exp != target_exp && cur_exp < 28 && !(*overflow)) {
        cur = binary_multiplication(cur, get_power_of_ten(1), overflow);
        cur_exp++;
        set_exponent(&cur, cur_exp);
    }

    if (!*overflow) {
        if (e1 < e2)
            *a = cur;
        else
            *b = cur;
    } else {
        s21_bank_rounding(&cur, target_exp - cur_exp);
    }
}

int get_exponent(s21_decimal decimal) {
    int exponent = 0;

    for (int i = D_START_EXP, j = 0; i <= D_END_EXP; i++, j++) {
        if (IS_SET(decimal.bits[3], i))
            ADD_BIT(exponent, j);
    }

    return exponent;
}

void set_exponent(s21_decimal *decimal, int new_exponent) {
    if (new_exponent <= 28) {
        short sign = IS_SET(decimal->bits[3], D_SIGN);
        decimal->bits[3] = 0;
        if (sign)
            set_sign(decimal, 1);
        SET_BIT(decimal->bits[3], new_exponent, D_START_EXP);
    }
}


void handle_exponent_add(s21_decimal value_1, s21_decimal value_2, s21_decimal *result, int *code) {
    int exp = s21_normalize(&value_1, &value_2);
    *result = binary_addition(value_1, value_2, code);
    set_exponent(result, exp);
}

int s21_add(s21_decimal value_1, s21_decimal value_2, s21_decimal *result) {
    memset(result, 0, sizeof(*result));
    int code = ARITHMETIC_OK;
    int s1 = get_sign(&value_1);
    int s2 = get_sign(&value_2);
    set_sign(&value_1, 0);
    set_sign(&value_2, 0);
    if (s1 == POS && s2 == POS) {
        handle_exponent_add(value_1, value_2, result, &code);
    } else if (s1 == POS && s2 == NEG) {
        if (s21_is_greater(value_1, value_2)) {
            code = s21_sub(value_1, value_2, result);
        } else {
            code = s21_sub(value_2, value_1, result);
            set_sign(result, 1);
        }
    } else if (s1 == NEG && s2 == POS) {
        if (s21_is_greater(value_1, value_2)) {
            code = s21_sub(value_1, value_2, result);
            set_sign(result, 1);
        } else {
            code = s21_sub(value_2, value_1, result);
            set_sign(result, 0);
        }
    } else if (s1 == NEG && s2 == NEG) {
        handle_exponent_add(value_1, value_2, result, &code);
        set_sign(result, 1);
        if (code == 1)
            code = 2;
    }
    return code;
}

//int s21_sub(s21_decimal value_1, s21_decimal value_2, s21_decimal *result) {
//    int a = 0;
//    int result_sign;
//    int flag = 0;
//    if (get_sign(&value_1) != get_sign(&value_2)) { // разные знаки => ++ --
//        result_sign = 0;
//        if (get_sign(&value_1)) result_sign = 1;
//        if (get_sign(&value_2)) result_sign = 0;
//        set_sign(&value_1, 0);
//        set_sign(&value_2, 0);
//        a = s21_add(value_1, value_2, result);
//        set_sign(result, result_sign);
//        if (result_sign) {
//            flag = 1;
//        } else {
//            flag = 2;
//        }
//    } else { // знаки одинаковые => +- or -+
//        if (s21_is_equal(value_1, value_2)) {
//            // нужно занулить result
//        } else {
//            int sign1 = get_sign(&value_1), sign2 = get_sign(&value_2);
//            set_sign(&value_1, 0);
//            set_sign(&value_2, 0);
//            s21_decimal *small, *big;
//
//            if (s21_is_less(value_1, value_2)) {
//                small = &value_1;
//                big = &value_2;
//                result_sign = !sign2;
//            } else {
//                small = &value_2;
//                big = &value_1;
//                result_sign = sign1;
//            }
//
//            convert_to_addcode(small);
//            a = s21_add(*small, *big, result);
//            set_sign(result, result_sign);
//        }
//    }
//    if (flag == 1 && a != 0) {
//        a = 2;
//    } else {
//        a = 1;
//    }
//        return  a;
//}

int s21_sub(s21_decimal value_1, s21_decimal value_2, s21_decimal *result) {
    memset(result, 0, sizeof(*result));
    int code = ARITHMETIC_OK;
    int s1 = get_sign(&value_1);
    int s2 = get_sign(&value_2);
    set_sign(&value_1, 0);
    set_sign(&value_2, 0);
    if (s1 == POS && s2 == POS) {
        if (s21_is_greater(value_1, value_2)) {
            handle_exponent_sub(value_1, value_2, result, &code);
        } else {
            handle_exponent_sub(value_2, value_1, result, &code);
            set_sign(result, 1);
        }
    } else if (s1 == POS && s2 == NEG) {
        code = s21_add(value_1, value_2, result);
    } else if (s1 == NEG && s2 == POS) {
        code = s21_add(value_1, value_2, result);
        set_sign(result, 1);
        if (code == 1)
            code = 2;
    } else if (s1 == NEG && s2 == NEG) {
        if (s21_is_greater(value_1, value_2)) {
            handle_exponent_sub(value_1, value_2, result, &code);
            set_sign(result, 1);
        } else {
            handle_exponent_sub(value_2, value_1, result, &code);
        }
    }
    return code;
}

void handle_exponent_sub(s21_decimal value_1, s21_decimal value_2, s21_decimal *result, int *code) {
    int exp = s21_normalize(&value_1, &value_2);
    *result = binary_subtraction(value_1, value_2, code);
    set_exponent(result, exp);
}
int s21_normalize(s21_decimal *value_1, s21_decimal *value_2) {
    int e1 = get_exponent(*value_1);
    int e2 = get_exponent(*value_2);
    set_exponent(value_1, 0);
    set_exponent(value_2, 0);

    s21_decimal big = e1 > e2 ? *value_2 : *value_1;
    int e_big = min(e1, e2);
    s21_decimal small = e1 > e2 ? *value_1 : *value_2;
    int e_small = max(e1, e2);

    int of_check = 0;
    s21_decimal big_overflow = big;
    while (e_big < e_small) {
        big_overflow =
                binary_multiplication(big_overflow, get_power_of_ten(1), &of_check);
        if (of_check)
            break;
        e_big++;
        big = big_overflow;
    }

    while (e_big < e_small) {
        s21_decimal ten = get_power_of_ten(1);
        s21_decimal last = {0};
        s21_decimal before_last = {0};
        s21_int_mod(small, ten, &last);
        s21_int_div(small, ten, &before_last);
        s21_int_mod(before_last, ten, &before_last);

        int ilast, ibefore_last;
        s21_from_decimal_to_int(last, &ilast);
        s21_from_decimal_to_int(before_last, &ibefore_last);
        int never_error = 0;
        s21_int_div(small, ten, &small);
        if (ilast < 5) {
        } else if (ilast == 5) {
            if (ibefore_last % 2 == 1) {
                small =
                        binary_addition(small, get_power_of_ten(0), &never_error);
            }
        } else {
            small = binary_addition(small, get_power_of_ten(0), &never_error);
        }
        e_small--;
    }
    set_exponent(value_1, e_small);
    set_exponent(value_2, e_small);
    if (e1 > e2) {
        *value_2 = big;
        *value_1 = small;
    } else {
        *value_2 = small;
        *value_1 = big;
    }

    return e_small;
}

s21_decimal binary_subtraction(s21_decimal value_1, s21_decimal value_2, int *err) {
    *err = ARITHMETIC_OK;
    value_1.bits[3] = 0;
    value_2.bits[3] = 0;
    s21_decimal carry = {0};

    value_2 = binary_addition(bit_not(value_2), get_power_of_ten(0), err);

    while (!eq_zerol(value_2)) {
        memset(&carry, 0, sizeof(s21_decimal));
        carry = bit_land(value_1, value_2);
        lshiftl(&carry);
        value_1 = bit_lxor(value_1, value_2);
        value_2 = carry;
    }

    value_1.bits[3] = 0;
    return value_1;
}

s21_decimal binary_addition(s21_decimal value_1, s21_decimal value_2, int *err) {
    *err = ARITHMETIC_OK;
    s21_decimal carry = {0};

    while (!eq_zero(value_2)) {
        memset(&carry, 0, sizeof(s21_decimal));
        carry = bit_and(value_1, value_2);
        *err = shiftl(&carry);
        value_1 = bit_xor(value_1, value_2);

        value_2 = carry;

        if (*err)
            break;
    }

    return value_1;
}

s21_decimal bit_xor(s21_decimal a, s21_decimal b) {
    s21_decimal result = {0};
    result.bits[0] = a.bits[0] ^ b.bits[0];
    result.bits[1] = a.bits[1] ^ b.bits[1];
    result.bits[2] = a.bits[2] ^ b.bits[2];
    return result;
}

s21_decimal bit_lxor(s21_decimal a, s21_decimal b) {
    s21_decimal result = {0};
    result.bits[0] = a.bits[0] ^ b.bits[0];
    result.bits[1] = a.bits[1] ^ b.bits[1];
    result.bits[2] = a.bits[2] ^ b.bits[2];
    result.bits[3] = a.bits[3] ^ b.bits[3];
    return result;
}

s21_decimal bit_and(s21_decimal a, s21_decimal b) {
    s21_decimal result = {0};
    result.bits[0] = a.bits[0] & b.bits[0];
    result.bits[1] = a.bits[1] & b.bits[1];
    result.bits[2] = a.bits[2] & b.bits[2];
    return result;
}

s21_decimal bit_land(s21_decimal a, s21_decimal b) {
    s21_decimal result = {0};
    result.bits[0] = a.bits[0] & b.bits[0];
    result.bits[1] = a.bits[1] & b.bits[1];
    result.bits[2] = a.bits[2] & b.bits[2];
    result.bits[3] = a.bits[3] & b.bits[3];
    return result;
}

s21_decimal bit_not(s21_decimal a) {
    s21_decimal result = {0};
    result.bits[0] = ~a.bits[0];
    result.bits[1] = ~a.bits[1];
    result.bits[2] = ~a.bits[2];
    return result;
}

int shiftl(s21_decimal *a) {
    // Check if the 31th bit is set
    int b1_tmp = get_bit(*a, 31);
    a->bits[0] <<= 1;

    int b2_tmp = get_bit(*a, 63);
    a->bits[1] <<= 1;
    // binary shift to the left makes last bit equal to zero (because this is pow(a, 2))
    // b1_tmp - remembered sign
    if (b1_tmp)
        set_bit(a, 32, 1);

    int is_overflow = get_bit(*a, 95);
    a->bits[2] <<= 1;

    if (b2_tmp)
        set_bit(a, 64, 1);

    return (is_overflow);
}

int lshiftl(s21_decimal *a) {
    // Check if the 31th bit is set
    int b1_tmp = get_bit(*a, 31);
    a->bits[0] <<= 1;

    int b2_tmp = get_bit(*a, 63);
    a->bits[1] <<= 1;
    // binary shift to the left makes last bit equal to zero (because this is pow(a, 2))
    // b1_tmp - remembered sign
    if (b1_tmp)
        set_bit(a, 32, 1);

    int is_overflow = get_bit(*a, 95);
    a->bits[2] <<= 1;

    if (b2_tmp)
        set_bit(a, 64, 1);

    a->bits[3] <<= 1;
    if (is_overflow)
        set_bit(a, 96, 1);
    return (is_overflow);
}

void shiftr(s21_decimal *a) {
    int b1_tmp = get_bit(*a, 64);
    a->bits[2] >>= 1;
    set_bit(a, 95, 0);

    int b2_tmp = get_bit(*a, 32);
    a->bits[1] >>= 1;
    set_bit(a, 63, 0);
    if (b1_tmp)
        set_bit(a, 63, 0);

    a->bits[0] >>= 1;

    set_bit(a, 31, 0);

    if (b2_tmp)
        set_bit(a, 31, 1);
}


int bank_rounding(int n) {
    int res = 0;

    int rem = n % 10;
    int second_is_odd = (n / 10) % 2;
    if (rem > 5 || (rem == 5 && second_is_odd)) {
        res = 1;
    }

    return res;
}



void s21_bank_rounding(s21_decimal *dec, int times) {
    int code = 0;

    int sign = get_sign(dec);
    int exp = get_exponent(*dec);
    set_sign(dec, 0);

    while (times > 0) {
        s21_decimal mod = {0}, ten = get_power_of_ten(1), hun = get_power_of_ten(2);

        s21_int_mod(*dec, hun, &mod);

        int mask = (127 & mod.bits[0]);

        s21_int_div(*dec, ten, dec);

        set_exponent(dec, exp - 1);

        if (bank_rounding(mask)) {
            s21_decimal one = {{1, 0, 0, 0}};
            s21_decimal copy = *dec;
            *dec = binary_addition(copy, one, &code);
            set_exponent(dec, exp - 1);
        }

        times--;
    }

    set_sign(dec, sign);
}


//int s21_mod(s21_decimal value_1, s21_decimal value_2, s21_decimal *result) {
//    memset(result, 0, sizeof(*result));
//    if (eq_zero(value_2))
//        return S21_NAN;
//    int code = 0;
//    int exp = max(get_exponent(value_1), get_exponent(value_2));
//    (void) s21_normalize(&value_1, &value_2);
//
//    code = s21_int_mod(value_1, value_2, result);
//    set_exponent(result, exp);
//
//    return code;
//}
int s21_mod(s21_decimal value_1, s21_decimal value_2, s21_decimal *result) {
    s21_decimal tmp;
    s21_decimal tmp1;
    int res = 0;
    int div = 0, mul = 0, sub = 0;
    init_struct(&tmp);
    init_struct(&tmp1);
    div = s21_div(value_1, value_2, &tmp);
    s21_truncate(tmp, &tmp1);
    tmp = tmp1;
    clear_bits(&tmp1);
    mul = s21_mul(tmp, value_2, &tmp1);
    tmp = tmp1;
    sub = s21_sub(value_1, tmp, result);
    if (div != 0) {
        res = div;
    } else if (mul != 0) {
        res = mul;
    } else if (sub != 0) {
//        res = sub;
    } else {}

    return res;
}


int s21_int_mod(s21_decimal dividend, s21_decimal divisor, s21_decimal *result) {
    if (eq_zero(divisor))
        return S21_NAN;

    result->bits[0] = 1;
    result->bits[1] = 0;
    result->bits[2] = 0;
    result->bits[3] = 0;

    set_sign(result, get_sign(&dividend));
    set_sign(&dividend, 0);
    set_sign(&divisor, 0);

    *result = s21_integer_mod_private(dividend, divisor);

    return ARITHMETIC_OK;
}

s21_decimal s21_integer_mod_private(s21_decimal dividend, s21_decimal divisor) {
    s21_decimal original_divisor = divisor;
    s21_decimal res = {0};
    s21_decimal modified_dividend = {0};
    s21_decimal zero = {0};
//    int code = 0;

    if (s21_is_equal(dividend, divisor)) {
        return zero;
    } else if (s21_is_less_basic(dividend, divisor)) {
        return dividend;
    }

    while ((s21_is_less_basic(divisor, dividend) || s21_is_equal(divisor, dividend)) &&
           !get_bit(divisor, 95)) {
        // printf("1\n");
        shiftl(&divisor);
    }

    if (s21_is_less_basic(dividend, divisor)) {
        // printf("2\n");
        shiftr(&divisor);
    }

    res = s21_integer_mod_private(modified_dividend, original_divisor);

    return res;
}

int s21_div(s21_decimal value_1, s21_decimal value_2, s21_decimal *result) {
    memset(result, 0, sizeof(*result));
    if (eq_zero(value_2))
        return S21_NAN;

    int code = ARITHMETIC_OK;

    int s1 = get_sign(&value_1);
    int s2 = get_sign(&value_2);

    set_sign(&value_1, 0);
    set_sign(&value_2, 0);


    handle_exponent_div(value_1, value_2, result, &code);

    if (s1 != s2)
        set_sign(result, 1);

    return code;
}

int s21_int_div(s21_decimal dividend, s21_decimal divisor, s21_decimal *result) {
    if (eq_zero(divisor))
        return S21_NAN;
    int code = ARITHMETIC_OK;

    *result = get_power_of_ten(0);
    int s1 = get_sign(&dividend), s2 = get_sign(&divisor);

    set_sign(&dividend, 0);
    set_sign(&divisor, 0);

    int final_exp = get_exponent(dividend) - get_exponent(divisor);
    s21_normalize(&dividend, &divisor);
    set_exponent(&dividend, 0);
    set_exponent(&divisor, 0);

    memset(result, 0, sizeof(s21_decimal));
    result->bits[0] = 1;

    *result = s21_integer_div_private(dividend, divisor, result);

    set_exponent(result, final_exp);

    if (s1 != s2)
        set_sign(result, 1);

    return code;
}

void handle_exponent_div(s21_decimal value_1, s21_decimal value_2, s21_decimal *result, int *code) {
    s21_normalize_decimal_pair(&value_1, &value_2, code);
    if (s21_is_equal(value_2, get_power_of_ten(0))) {
        *result = value_1;
        *code = ARITHMETIC_OK;
    } else {
        memset(result, 0, sizeof(s21_decimal));
        result->bits[0] = 1;
        *result = s21_integer_div_private(value_1, value_2, result);
    }
}

s21_decimal s21_integer_div_private(s21_decimal dividend, s21_decimal divisor, s21_decimal *result) {
    s21_decimal original_divisor = divisor;
    s21_decimal modified_dividend = {0};
    s21_decimal one = {{1}};
    int unused_code = 0;

    if (s21_is_equal(dividend, divisor))
        return one;
    else if (s21_is_less_basic(dividend, divisor))
        return modified_dividend;
    while ((s21_is_less_basic(divisor, dividend) || s21_is_equal(divisor, dividend)) &&
           !get_bit(divisor, 95)) {
        shiftl(&divisor);
        shiftl(result);
    }

    if (s21_is_less_basic(dividend, divisor)) {
        // printf("4\n");
        shiftr(&divisor);
        shiftr(result);
    }

    modified_dividend = binary_subtraction(dividend, divisor, &unused_code);

    one = s21_integer_div_private(modified_dividend, original_divisor, &one);

    *result = binary_addition(one, *result, &unused_code);

    return *result;
}

//int s21_is_less(s21_decimal a, s21_decimal b) {
//    if (both_all_zeroes(a, b)) {
//        return false;
//    }
//
//    bool sign1 = get_sign(a), sign2 = get_sign(b);
//
//    /* POS < NEG */
//    if (!sign1 && sign2) {
//        return false;
//    }
//
//    /* NEG < NEG */
//    if (sign1 && sign2) {
//        set_sign(&a, 0);
//        set_sign(&b, 0);
//        return s21_is_greater(a, b);
//    }
//
//    /* NEG < POS */
//    if (sign1 && !sign2) {
//        return true;
//    }
//
//    int res = s21_is_less_positive(a, b);
//
//    return res;
//}

bool s21_is_less_basic(s21_decimal a, s21_decimal b) {
    for (int i = 95; i >= 0; i--) {
        if ((get_bit(a, i) && get_bit(b, i)) || (!get_bit(a, i) && !get_bit(b, i))) {
            continue;
        }

        if (!get_bit(a, i) && get_bit(b, i)) {
            return true;
        } else {
            return false;
        }
    }

    return false;
}

bool s21_is_less_positive(s21_decimal a, s21_decimal b) {
    s21_normalize(&a, &b);

    for (int i = 95; i >= 0; i--) {
        if ((get_bit(a, i) && get_bit(b, i)) || (!get_bit(a, i) && !get_bit(b, i))) {
            continue;
        }
        if (!get_bit(a, i) && get_bit(b, i)) {
            return true;
        } else {
            return false;
        }
    }

    return false;
}
//====================================


int s21_mul(s21_decimal value_1, s21_decimal value_2, s21_decimal *result) {
    int sign_result;
    if (get_sign(&value_1) != get_sign(&value_2)) {
        sign_result = 1;
    } else {
        sign_result = 0;
    }
    int last_bit_1 = last_bit(value_1);
    s21_decimal tmpRes = {{0, 0, 0, 0}};
    int err = 0;
    for (int i = 0; i <= last_bit_1; i++) {
        init_struct(&tmpRes);
        int value_bit = get_bit(value_1, i);
        if (value_bit) {
            tmpRes = value_2;
            offset_left(&tmpRes, i);
            *result = bit_add(result, &tmpRes, &err);
            if (err != 0) break;
        }
    }
    int scale_res = get_scale(&value_1) + get_scale(&value_2);
    set_scale(result, scale_res);


    if (err || scale_res > 28 || scale_res < 0) {
        s21_decimal *big;
        if (is_greater(value_1, value_2)) {
            big = &value_1;
        } else {
            big = &value_2;
        }
        while(err && (scale_res <= 28) &&  (scale_res >= 0)) {
            if (err && (scale_res <= 28) && (scale_res >= 0)) {
                s21_decimal ten = {{10, 0, 0, 0}};
                s21_decimal zero = {{0, 0, 0, 0}};

                s21_decimal tmp = {{0, 0, 0, 0}};
                tmp = division_without_scale(*big, ten);
                if (is_equal_b(tmp, zero) != 0) { // tmp не полностью обрезался
                    copy_bits(tmp, big);
                } if (is_equal_b(tmp, zero) == 0) { // обрезался, нужны идеи как быть в этой ситуации(мб остаток от деления ставить)
                    copy_bits(tmp, big);
                }
                int bigger_scale = get_scale(big);
                set_scale(big, bigger_scale - 1);
                scale_res--;
            }
        }
    }
    set_sign(result, sign_result);
    s21_decimal zero = {{0, 0, 0, 0}};
    if (scale_res > 28 || scale_res < 0) {
        copy_bits(zero, result);
    }
    return err;
}

//int s21_div(s21_decimal value_1, s21_decimal value_2, s21_decimal *result) {
//    int beginScale = get_scale(&value_1) - get_scale(&value_2);
//    int Sign = get_sign(&value_1) != get_sign(&value_2);
//    int res = 0;
//    s21_decimal remainder, tmp;
//    set_scale(&value_1, 0);
//    set_scale(&value_2, 0);
//    set_sign(&value_1, 0);
//    set_sign(&value_2, 0);
//
//    tmp = div_only_bits(value_1, value_2, &remainder);
//    copy_bits(tmp, result);
//
//    s21_decimal border = {{-1, -1, -1, 0}};
//    s21_decimal  ten = {{10, 0, 0, 0}};
//    s21_decimal zero = {{0, 0, 0, 0}};
//    set_scale(&border, 1);
//    int insideScale = 0;
//    if (s21_is_equal(value_2, zero)){
//        copy_bits(zero, result);
//        res = 3;
//    } else {
//        for (; insideScale <= 27 && s21_is_equal(remainder, zero) == FALSE;) {
//            if (s21_is_less(*result, border) == FALSE) {
//                break;
//            }
//            s21_decimal tmp1 = {{0, 0, 0, 0}};
//            s21_mul(remainder, ten, &tmp1);
//            remainder = tmp1;
//            clear_bits(&tmp1);
//            tmp = div_only_bits(remainder, value_2, &tmp1);
//            remainder = tmp1;
//            clear_bits(&tmp1);
//            s21_mul(*result, ten, &tmp1);
//            *result = tmp1;
//            clear_bits(&tmp1);
//            s21_add(*result, tmp, &tmp1);
//            *result = tmp1;
//            insideScale++;
//        }
//        s21_decimal trash;
//        int endScale = beginScale + insideScale;
//        for (; endScale > 28;) {
//            *result = div_only_bits(*result, ten, &trash);
//            endScale--;
//        }
//        for (; endScale < 0;) {
//            s21_mul(*result, ten, result);
//            endScale++;
//        }
//
//        set_scale(result, endScale);
//        set_sign(result, Sign);
//    }
//    return res;
//}

//int s21_mod(s21_decimal value_1, s21_decimal value_2, s21_decimal *result) {
//    s21_decimal tmp;
//    s21_decimal tmp1;
//    int res = 0;
//    int div = 0, mul = 0, sub = 0;
//    init_struct(&tmp);
//    init_struct(&tmp1);
//    div = s21_div(value_1, value_2, &tmp);
//    s21_truncate(tmp, &tmp1);
//    tmp = tmp1;
//    clear_bits(&tmp1);
//    mul = s21_mul(tmp, value_2, &tmp1);
//    tmp = tmp1;
//    sub = s21_sub(value_1, tmp, result);
//    if (div != 0) {
//        res = div;
//    } else if (mul != 0) {
//        res = mul;
//    } else if (sub != 0) {
////        res = sub;
//    } else {}
//
//    return res;
//}

////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                   СРАВНЕНИЯ                                                            //
////////////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 *
 * @param value_1
 * @param value_2
 * @return 0 или 1
 */
int s21_is_greater(s21_decimal value_1, s21_decimal value_2) {
    int is_greater = -1;

    int negative = -1;
    int sign_value_1 = get_sign(&value_1);
    int sign_value_2 = get_sign(&value_2);

    if (sign_value_1 && !sign_value_2) negative = 0;
    if (!sign_value_1 && sign_value_2) negative = 1;
    if (!sign_value_1 && !sign_value_2) negative = 2;
    if (negative == 0) is_greater = 0;
    if (negative == 1) is_greater = 1;
    check_scale(&value_1, &value_2);

    for (int i = 95; i >= 0 && is_greater == -1; i--) {
        int bit_dec1 = get_bit(value_1, i);
        int bit_dec2 = get_bit(value_2, i);
        if (bit_dec1 && !bit_dec2) is_greater = 1;
        if (bit_dec2 && !bit_dec1) is_greater = 0;

        if (is_greater != -1) {
            if (get_sign(&value_1) && get_sign(&value_2)) is_greater = !is_greater;
        }
    }
    if (is_greater == -1) is_greater = 0;
    return is_greater;
}


/**
 * @brief
 * @param dec1 Первое число децимал
 * @param dec2 Второе число децимал
 * @return 0 - меньше, 1 - больше
 */
int s21_is_less(s21_decimal dec1, s21_decimal dec2) {
    return !!s21_is_greater(dec2, dec1);
}

/**
 *
 * @param value_1
 * @param value_2
 * @return 0 - false 1 - true
 */
int s21_is_equal(s21_decimal value_1, s21_decimal value_2) {
    int equal = -1;

    int negative = 0;
    int sign_value_1 = get_sign(&value_1);
    int sign_value_2 = get_sign(&value_2);

    if (sign_value_1 && !sign_value_2) negative = 0;
    if (!sign_value_1 && sign_value_2) negative = 1;
    if (!sign_value_1 && !sign_value_2) negative = 2;
    if (sign_value_1 && sign_value_2) negative = 3;
    if (negative == 0 || negative == 1) equal = 0;

    check_scale(&value_1, &value_2);
    for (int i = 95; i >= 0; i--) {
        int bit1 = get_bit(value_1, i);
        int bit2 = get_bit(value_2, i);
        if (bit1 != bit2) equal = 0;
    }

    return (equal != 0) ? 1 : 0;
}


/**
 *
 * @param value_1
 * @param value_2
 * @return 1 - true, 0 - false
 */
int s21_is_not_equal(s21_decimal value_1, s21_decimal value_2) {
    return (!s21_is_equal(value_1, value_2));
}


int s21_is_greater_or_equal(s21_decimal value_1, s21_decimal value_2) {
    return !!(s21_is_greater(value_1, value_2) || s21_is_equal(value_1, value_2));
}

int s21_is_less_or_equal(s21_decimal value_1, s21_decimal value_2) {
    return !!(s21_is_less(value_1, value_2) || s21_is_equal(value_1, value_2));
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                     КОНВЕРТЕРЫ                                                                    //
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


int s21_from_int_to_decimal(int src, s21_decimal *dst) {
    char result = FALSE;
    if (dst) {
        dst->bits[0] = dst->bits[1] = dst->bits[2] = dst->bits[3] = 0;
        if (src < 0) {
            set_sign(dst, 1);
            src *= -1;
        }
        dst->bits[0] = src;
    } else {
        result = TRUE;
    }
    return result;
}
int s21_from_decimal_to_int(s21_decimal src, int *dst) {
    s21_decimal truncated = {0};
    int result = s21_ok;
    int code = s21_truncate(src, &truncated);
    int sign = get_sign(&src);
    if (!dst || code == s21_convert_error)
        result = s21_convert_error;
    if (get_bit(truncated, 31) || truncated.bits[1] || truncated.bits[2])
        result = s21_convert_error;
    int tmp = 0;
    for (int i = 0; i < 31; i++)
        tmp += pow(2, i) * get_bit(truncated, i);
    if (tmp == INT_MIN && sign)
        result = s21_convert_error;
    if (sign)
        tmp = -tmp;
    *dst = tmp;
    return result;
}
int getFloatSign(float *src) {
    return *(int *)src >> 31;
}

int getFloatExp(float *src) {
    return ((*(int *)src & ~0x80000000) >> 23) - 127;
}

int s21_from_decimal_to_float(s21_decimal src, float *dst) {
    int result = s21_ok;
    int sign = get_sign(&src);
    if (!dst) {
        result = s21_convert_error;
    } else {
        int scale = get_scale(&src);
        long double tmp = 0;
        for (int i = 0; i < 96; i++)
            tmp += pow(2, i) * get_bit(src, i);
        while (scale--)
            tmp /= 10.0;
        if (sign)
            tmp *= -1.0;
        *dst = tmp;
    }
    return result;
}

int s21_from_float_to_decimal(float src, s21_decimal *dst) {
    dst->bits[0] = dst->bits[1] = dst->bits[2] = dst->bits[3] = 0;
    int result = FALSE, sign = getFloatSign(&src), exp = getFloatExp(&src);
    if (isinf(src) && !sign)
        result = s21_convert_error;
    else if (isinf(src) && sign)
        result = s21_convert_error;
    else if (isnan(src))
        result = s21_convert_error;

    if (dst && src != 0) {
        double temp = (double)fabs(src);
        int off = 0;
        for (; off < 28 && (int)temp / (int)pow(2, 21) == 0; temp *= 10, off++) {
        }
        temp = round(temp);
        if (off <= 28 && (exp > -94 && exp < 96)) {
            bitsun mant;
            temp = (float)temp;
            for (; fmod(temp, 10) == 0 && off > 0; off--, temp /= 10) {
            }
            mant.fl = temp;
            exp = getFloatExp(&mant.fl);
            dst->bits[exp / 32] |= 1 << exp % 32;
            for (int i = exp - 1, j = 22; j >= 0; i--, j--)
                if ((mant.ui & (1 << j)) != 0) dst->bits[i / 32] |= 1 << i % 32;
            dst->bits[3] = (sign << 31) | (off << 16);
            result = s21_ok;
        }
    }
    return result;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////
//                              ДРУГИЕ ФУНКЦИИ                                                       //
///////////////////////////////////////////////////////////////////////////////////////////////////////


int s21_truncate(s21_decimal value, s21_decimal *result) {
    s21_decimal ten = {{10, 0, 0, 0}};
    // s21_decimal res = {{0, 0, 0, 0}};
    s21_decimal tmp = {{0, 0, 0, 0}};
    s21_decimal zero = {{0, 0, 0, 0}};
    int sign = get_sign(&value);
    int scale = get_scale(&value);
    int solution = 0;
    if (!scale) {
        *result = value;
    } else {
        for (int i = scale; i > 0; i--) {
//            *result = division_without_scale(value, ten);
            *result = div_only_bits(value, ten, &tmp);
            value = *result;
        }
        if (s21_is_equal(*result, zero)) solution = 1;
    }
    if (sign && solution) set_sign(result, 1);
    return solution;
}


//********************************************************************************************************//
//********************************************************************************************************//
//********************************************************************************************************//
//********************************************************************************************************//
//********************************************************************************************************//
//********************************************************************************************************//
//********************************************************************************************************//
//********************************************************************************************************//
//********************************************************************************************************//

s21_decimal bit_add(s21_decimal *a, s21_decimal *b, int *error_code) {
    s21_decimal result = {{0 ,0, 0, 0}};
    size_t buffer = 0;
    int flag = 0;
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
                buffer = 1;
            } else {
                set_bit(&result, i, 0);
                buffer = 1;
            }
        }

        if (i == 95 && buffer == 1 && (current_bit_a || current_bit_b) && *error_code != -1) {
            *error_code = 1;
        } else if (*error_code == -1 && flag != 1) {
            flag = 1;
        }
        flag = 0;
    }
    if (flag == 1) *error_code = 0;
    return result;
}


int get_bit(const s21_decimal a, int bit_number) {
    int result = 0;
    if (bit_number / 32 < 4) {
        unsigned int mask = 1u << (bit_number % 32);
        result = a.bits[bit_number


                        / 32] & mask;
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
    int res = S21_NORMAL_VALUE;
    int lastbit = last_bit(*num_ptr);
    if (lastbit + value_offset > 95) {
        res = S21_INFINITY;
    }
    if (res != S21_INFINITY) {
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
/**
 *
 * @param number1 перавое число
 * @param number2 второе соответственно
 * @return общий скейл
 */

int scale_equalize(s21_decimal *number1, s21_decimal *number2) {
    s21_decimal *big = NULL;
    s21_decimal  *small = NULL;
    // int process = 1;
    if (get_scale(number1) == get_scale(number2)) {
        // process = 0;
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
            tmp = bit_add(&tmp1, &tmp2, &err);
//            if (get_bit(tmp, 96)) err = 1;
            if (err != 1) {
                copy_bits(tmp, small);
                set_scale(small, small_scale + 1);
            }
        } else {
            // деление большего скейла на 10
            s21_decimal ten = {{10, 0, 0, 0}};
            s21_decimal remainder;
            s21_decimal zero = {{0, 0, 0, 0}};

            s21_decimal tmp = {{0, 0, 0, 0}};
//            tmp = division_without_scale(*big, ten);
            tmp = div_only_bits(*big, ten, &remainder);
            if (is_equal_b(tmp, zero) != 0) { // tmp не полностью обрезался
                copy_bits(tmp, big);
            } else { // обрезался, нужны идеи как быть в этой ситуации(мб остаток от деления ставить)
                copy_bits(remainder, big);
            }
            bigger_scale = get_scale(big);
            set_scale(big, bigger_scale - 1);
        }
    }
    return get_scale(number1);
}


//int scale_equalize(s21_decimal *number1, s21_decimal *number2) {
//    s21_decimal *big = NULL;
//    s21_decimal  *small = NULL;
//    int process = 1;
//    if (get_scale(number1) == get_scale(number2)) {
//        process = 0;
//    } else if (get_scale(number1) > get_scale(number2)) {
//        big = number1;
//        small = number2;
//    } else {
//        big = number2;
//        small = number1;
//    }
//
//    s21_decimal tmp;
//    init_struct(&tmp);
//    int small_scale = 0;
//    int bigger_scale = 0;
//
//
//    int err = 0;
//    int flag = 0;
//    if (get_bit(*big, 95) && get_bit(*small, 95)) err = 1;
//    while (get_scale(number1) != get_scale(number2)) {
//        if (!err) {
//            bigger_scale = get_scale(big);
//            s21_decimal tmp1;
//            s21_decimal tmp2;
//            tmp1 = *big;
//            tmp2 = *big;
//            if (!flag)  {
//                offset_left(&tmp1, 1);
//                offset_left(&tmp2, 3);
//                tmp = bit_add(&tmp1, &tmp2, &err);
//                copy_bits(tmp, big);
//                tmp1 = *big;
//                tmp2 = *big;
//                flag = 1;
//            }
//            offset_left(&tmp1, 1);
//            offset_left(&tmp2, 3);
//            tmp = bit_add(&tmp1, &tmp2, &err);
//            if (get_bit(tmp, 95)) err = 1;
//            if (err != 1) {
//                copy_bits(tmp, big);
//                set_scale(big, bigger_scale - 1);
//            }
//        }
//        if (err) {
//            // деление большего скейла на 10
//            s21_decimal ten = {{10, 0, 0, 0}};
//            s21_decimal zero = {{0, 0, 0, 0}};
//
//            s21_decimal tmp12 = {{0, 0, 0, 0}};
//            tmp12 = division_without_scale(*big, ten);
//            if (is_equal_b(tmp12, zero) != 0) { // tmp не полностью обрезался
//                copy_bits(tmp12, small);
//            } if (is_equal_b(tmp12, zero) == 0) { // обрезался, нужны идеи как быть в этой ситуации(мб остаток от деления ставить)
//                copy_bits(tmp12, small);
//            }
//            bigger_scale = get_scale(big);
//            set_scale(small, small_scale + 1);
//        }
//    }
//    return get_scale(number1);
//}
/**
 * @brief Функция переводит число децимал в доп.код
 * @param number_1 указатель на число децимал
 */
void convert_to_addcode(s21_decimal *number_1) {
    s21_decimal res;
    int err = -1;
    s21_decimal add = {{1, 0, 0, 0}};
    number_1->bits[0] = ~number_1->bits[0];
    number_1->bits[1] = ~number_1->bits[1];
    number_1->bits[2] = ~number_1->bits[2];
    res = bit_add(number_1, &add,&err);
    number_1->bits[0] = res.bits[0];
    number_1->bits[1] = res.bits[1];
    number_1->bits[2] = res.bits[2];
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
s21_decimal division_without_scale(s21_decimal num1, s21_decimal num2) {
    s21_decimal result1 = {{ 0, 0, 0, 0}};
    s21_decimal result = {{ 0, 0, 0, 0}};
    s21_decimal sub = {{0, 0, 0, 0}};
    s21_decimal tmp = {{0, 0, 0, 0}};
    s21_decimal one = {{1, 0, 0, 0}};
    s21_decimal null = {{0, 0, 0, 0}};
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
        int err = -1;
        while (is_greater_or_equal(sub, null) != 0 && sub.bits[0] > 0) {
            sub = bit_add(&sub, &num2, &err);
            if (sub.bits[0]) result1 = bit_add(&result1, &one, 0);
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
        tmp = bit_add(&tmp, &num2, &err);
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

int negative(s21_decimal value_1, s21_decimal value_2) {
    int negative = 0;
    int sign_value_1 = get_sign(&value_1);
    int sign_value_2 = get_sign(&value_2);

    if (!sign_value_1 && sign_value_2) negative = 0;
    if (sign_value_1 && !sign_value_2) negative = 1;
    if (!sign_value_1 && !sign_value_2) negative = 2;
    return negative;
}

void check_scale(s21_decimal *value_1, s21_decimal *value_2) {
    int scale_1 = get_scale(value_1);
    int scale_2 = get_scale(value_2);

    if (scale_1 != scale_2) {
        scale_equalize(value_1, value_2);
    }
}

/**
 * @brief Деление битов игнорируется степень
 * @param a число децимал
 * @param b число децимал
 * @param buf остаток от деления
 * @return s21_decimal результат деление (целая часть)
 */
s21_decimal div_only_bits(s21_decimal number_1, s21_decimal number_2,
                          s21_decimal *buf) {
    init_struct(buf);
    s21_decimal res = {{0, 0, 0, 0}};
    for (int i = last_bit(number_1); i >= 0; i--) {
        if (get_bit(number_1, i)) set_bit(buf, 0, 1);
        if (s21_is_greater_or_equal(*buf, number_2) == TRUE) {
            s21_decimal tmp1 = {{0, 0, 0, 0}};


            s21_sub(*buf, number_2, &tmp1);
            *buf = tmp1;
            if (i != 0) offset_left(buf, 1);
            if (get_bit(number_1, i - 1)) set_bit(buf, 0, 1);
            offset_left(&res, 1);
            set_bit(&res, 0, 1);
        } else {
            offset_left(&res, 1);
            if (i != 0) offset_left(buf, 1);
            if ((i - 1) >= 0 && get_bit(number_1, i - 1)) set_bit(buf, 0, 1);
        }
    }
    return res;
}
int s21_negate(s21_decimal value, s21_decimal *result) {
    for (int i = 0; i < 4; i++)
        result->bits[i] = value.bits[i];
    set_sign(result, !get_sign(&value));
    return s21_ok;
}
s21_decimal get_power_of_ten(int pow) {
    s21_decimal result = {0};
    set_scale(&result, 0);
    for (int i = 0; i < 96; ++i) {
        if (binary_powers_of_ten[pow][95 - i] == '1') {
            set_bit(&result, 1, i);
        } else {
            set_bit(&result, 0, i);
        }
    }
    return result;
}
int s21_floor(s21_decimal value, s21_decimal *result) {
    int sign = get_sign(&value);
    memset(result, 0, sizeof(*result));
    int never_error = 0;

    s21_decimal one = get_power_of_ten(0);
    s21_decimal zero = {0};
    s21_decimal mod_res = {0};

    s21_mod(value, one, &mod_res);
    s21_truncate(value, result);

    if (get_sign(&value) && s21_is_not_equal(value, zero)  && s21_is_not_equal(mod_res, zero)) {
        *result = bit_add(result, &one, &never_error);
    }

    set_sign(result, sign);
    return s21_ok;
}
