#include<stdio.h>
#include <string.h>
#include <math.h>
//#include "s21_decimal.h"
#define TRUE  1
#define FALSE  0

typedef struct {
  unsigned int bits[4];
//  value_type_t value_type;
} s21_decimal;

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
 int get_sign(const s21_decimal *a) {
    unsigned int mask = 1u << 31;
    return !!(a->bits[3] & mask);
}
int get_scale(const s21_decimal *a) {
    return (char)(a->bits[3] >> 16);
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
int s21_from_decimal_to_int(s21_decimal src, int *dst) {
  int result = 1;
    *dst = src.bits[0];
    if (get_sign(&src)) {
        *dst *= -1;
    } else {
        *dst *= 1;
    }
    *dst /= (int)pow(10, get_scale(&src));
    result = 0;
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

