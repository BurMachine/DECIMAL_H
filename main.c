#include<stdio.h>
#include "s21_decimal.h"

void print0001(s21_decimal a);
int is_greater(s21_decimal num1, s21_decimal num2);
int is_less_b(s21_decimal num1, s21_decimal num2);
s21_decimal division_without_scale(s21_decimal num1, s21_decimal num2);
int is_equal_b(s21_decimal dec1, s21_decimal dec2);


union abc {
    int i;
    char c[4];
};

void print01(int i, int szf) {
    while (szf--) {
        printf("%d", (i >> szf) & 1 );
    }
}


int main() {
//    s21_decimal a = {3, 0, 0, 0};
//    s21_decimal b = {0, 3, 2, 0};
    s21_decimal c = {0, 0, 0, 0};
//    s21_add(a, b, &c);
//    print0001(c);
    s21_decimal num1 = {123, 0, 0, 0};
    s21_decimal num2 = {3, 0, 0, 0};
    c = division_without_scale(num1, num2);
    print0001(num1);
    printf("\n");
    print0001(num2);
    printf("\n");
    print0001(c);
    printf("\n");
}

void print0001(s21_decimal a) {
    print01(a.bits[3], 32);
    printf(" ");
    print01(a.bits[2], 32);
    printf(" ");
    print01(a.bits[1], 32);
    printf(" ");
    print01(a.bits[0], 32);
}

s21_decimal division_without_scale(s21_decimal num1, s21_decimal num2) {
    s21_decimal result1 = { 0, 0, 0, 0};
    s21_decimal result = { 0, 0, 0, 0};
    s21_decimal tmp = {0, 0, 0, 0};
    s21_decimal one = {1, 0, 0, 0};
    s21_decimal div_num2 = num2;
    convert_to_addcode(&num2);

    int i = last_bit(num1);
    int bit = 0;
    int flag_offset = 0;
    for (;i >= 0; i--) {
        if(is_less_b(tmp, div_num2) && !is_equal_b(tmp, div_num2)) {  // для деления столбиком нахожу первое число, которое будет не меньше чем то , на которое делим. дальше делим(вычетанием) tmp
            if (flag_offset == 0) {
                set_bit(&tmp, 0, get_bit(num1, i));
                flag_offset = 1;
                continue;
            } else {
                offset_left(&tmp, 1);
                set_bit(&tmp, 0, get_bit(num1, i));
                continue;
            }
        }
//        while (tmp.bits[0] >= 0) {
//            tmp = bit_add(&tmp, &num2, 0);
//            result1 = bit_add(&result1, &one, 0); // ПРОГНАТЬ НА ДЕБЕГЕРЕ!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
//        }
        flag_offset = 0;
        for(int j = last_bit(result1); j >= 0; j--) {
            if (flag_offset == 0) {
                set_bit(&result, 0, get_bit(result1, j));
                flag_offset = 1;
                continue;
            } else {
                offset_left(&result, 1);
                set_bit(&result, 0, get_bit(result1, j));
                continue;
            }
        }

    }
    result = tmp;
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
            is_greater = TRUE;
        if (bit_dec2 && !bit_dec1)
            is_greater = FALSE;

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
        if (bit_dec1 != bit_dec2) is_equal = FALSE;
    }
    return (is_equal != FALSE) ? TRUE : FALSE;
}