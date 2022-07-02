#include<stdio.h>
#include "s21_decimal.h"

void print0001(s21_decimal a);

int s21_is_less(s21_decimal dec1, s21_decimal dec2);

void print01(int i, int szf) {
    while (szf--) {
        printf("%d", (i >> szf) & 1 );
    }
}
void set_sign(s21_decimal *a, int sign_value) {
    unsigned int mask = 1u <<31;
    if (sign_value) {
        a->bits[3] |= mask;
    } else {
        a->bits[3] &= ~mask;
    }
}
int s21_from_int_to_decimal(int src, s21_decimal *dst) {
  char result = TRUE;
  if (dst) {
    dst->bits[0] = dst->bits[1] = dst->bits[2] = dst->bits[3] = 0;
    if (src < 0) {
      set_sign(dst, 1);
      src *= -1;
    }
    dst->bits[0] = src;
  } else {
    result = FALSE;
  }
  return result;
}

int main() {
    int val = 2147483650;
    s21_decimal val_d;
    s21_from_int_to_decimal(val, &val_d);
    print0001(val_d);
    int val_1;
    s21_decimal val_d1 = {101, 1, 0, 0};
    s21_from_decimal_to_int(val_d1, &val_1);
    printf("%d\n", val_1);

//     int val = 100;
//     s21_decimal val_d;
//     s21_from_int_to_decimal(val, &val_d);
//     print0001(val_d);
//     s21_decimal c = {0, 0, 0, 0};
//     s21_decimal num1 = {16, 0, 0, 0};
//     s21_decimal num2 = {1, 0, 0, 0};
// //    set_bit(&num1, 95, 1);
// //    set_bit(&num2, 95, 1);
//     set_scale(&num1, 4);
//    set_scale(&num2, 1);
//    set_sign(&num2, 1);
//    scale_equalize(&num1, &num2);
    // s21_add(num1, num2, &c);
    // print0001(num1);
    // printf("\n");
    // print0001(num2);
    // printf("\n");
    // print0001(c);
    // printf("\n");
    // printf("%d\n%d", get_scale(&num1), get_scale(&num2));
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

//int main() {
//    s21_decimal num1 = {100000, 0, 0, 0};
//    s21_decimal num2 = {1, 0, 0, 0};
//    set_scale(&num1, 3);
//    set_scale(&num2, 6);
////    set_sign(&num1, 1);
//    int a = s21_is_greater(num1, num2);
//    int b = s21_is_less(num1, num2);
//    printf("%d\n%d\n", a, b);
//    print0001(num1);
//    printf("\n");
//    print0001(num2);
//    printf("\n");
//}

