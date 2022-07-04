#include<stdio.h>
#include <string.h>
#include <math.h>
#include "s21_decimal.h"


void print01(int i, int szf) {
    while (szf--) {
        printf("%d", (i >> szf) & 1 );
    }
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


// int s21_round(s21_decimal value, s21_decimal *result) {
//   int valid_value = TRUE;
//   s21_decimal res = {0, 0, 0, 0};
//   s21_decimal one = {1, 0, 0, 0};
//   s21_decimal five = {5, 0, 0, 0};
//   int sign = get_sign(&value);
//   set_sign(&value, 0);
//   s21_decimal trunc = s21_truncate(value, result);
//   s21_decimal buf = s21_sub(value, trunc);
//   set_scale(&five, 1);
//   if (valid_value) {
//     res = trunc;
//     if (s21_is_greater_or_equal(buf, five) == TRUE) {
//       res = s21_add(res, one);
//     }
//     set_sign(&res, sign);
//   } else {
//     res.value_type = dec1.value_type;
//   }
//   return res;
// }


int main() {
    // int d1 = 2147483650;
    // s21_decimal val_d1;
    // s21_from_int_to_decimal(d1, &val_d1);
    // print0001(val_d1);
    // printf("\n");
    // int d2;
    // s21_decimal val_d2 = {101, 1, 0, 0};
    // s21_from_decimal_to_int(val_d2, &d2);
    // printf("%d\n", d2);
    // float f1;
    // s21_decimal val_f1 = {121, 0, 0, 0};
    // set_scale(&val_f1, 3);
    // print0001(val_f1);
    // printf("\n");
    // s21_from_decimal_to_float(val_f1, &f1);
    // printf("%.5f\n", f1);
    // float f2 = 345423.3242;
    // s21_decimal val_f2;
    // s21_from_float_to_decimal(f2, &val_f2);
    // print0001(val_f2);
    // printf("\n");
   // s21_decimal c = {0, 0, 0, 0};
    // s21_decimal num1 = {16, 0, 0, 0};
    // s21_decimal num2 = {1, 0, 0, 0};
//    set_bit(&num1, 95, 1);
//    set_bit(&num2, 95, 1);
   // set_scale(&num1, 4);
//    set_scale(&num2, 1);
//    set_sign(&num2, 1);
//    scale_equalize(&num1, &num2);
//    s21_add(num1, num2, &c);
//    print0001(num1);
    // printf("\n");
    // print0001(num2);
    // printf("\n");
    // print0001(c);
    // printf("\n");
    // printf("%d\n%d", get_scale(&num1), get_scale(&num2));
 s21_decimal f1 = {132, 0, 0, 0};
 set_scale(&f1, 1);
 s21_decimal f2;
 s21_floor(f1, &f2);
    print0001(f1);
    printf("\n");
    print0001(f2);
    printf("\n");
}
