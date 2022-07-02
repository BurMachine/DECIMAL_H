#include<stdio.h>
#include <string.h>
#include <math.h>
#define TRUE  1
#define FALSE  0

typedef struct {
  unsigned int bits[4];
} s21_decimal;

typedef enum {
  s21_NORMAL_VALUE = 0,
  s21_INFINITY = 1,
  s21_NEGATIVE_INFINITY = 2,
  s21_NAN = 3,
  s21_ADDCODE = 4
}value_type;

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
int is_NAN(s21_decimal *dec1, s21_decimal *dec2) {
  int vt_dec1 = dec1;
  int vt_dec2 = dec2;
  return (vt_dec1 == s21_NAN || vt_dec2 == s21_NAN) ? TRUE : FALSE;
}

int is_inf(s21_decimal *dec1, s21_decimal *dec2) {
  int who_is_inf = 0;
  int vt_dec1 = dec1->value_type;
  int vt_dec2 = dec2->value_type;

  if (vt_dec1 == s21_INFINITY && vt_dec2 != s21_INFINITY) {
    who_is_inf = 1;
  } else if (vt_dec1 != s21_INFINITY && vt_dec2 == s21_INFINITY) {
    who_is_inf = -1;
  } else if (vt_dec1 == s21_INFINITY && vt_dec2 == s21_INFINITY) {
    who_is_inf = 2;
  }
  return who_is_inf;
}

int is_neg_inf(s21_decimal *dec1, s21_decimal *dec2) {
  int who_is_inf = 0;
  int vt_dec1 = dec1->value_type;
  int vt_dec2 = dec2->value_type;

  if (vt_dec1 == s21_NEGATIVE_INFINITY && vt_dec2 != s21_NEGATIVE_INFINITY) {
    who_is_inf = 1;
  }
  if (vt_dec1 != s21_NEGATIVE_INFINITY && vt_dec2 == s21_NEGATIVE_INFINITY) {
    who_is_inf = -1;
  }
  if (vt_dec1 == s21_NEGATIVE_INFINITY && vt_dec2 == s21_NEGATIVE_INFINITY) {
    who_is_inf = 2;
  }

  return who_is_inf;
}
int s21_from_decimal_to_float(s21_decimal src, float *dst) {
   int result = FALSE;
    double temp = 0;
    int off = 0;
    for (int i = 0; i < 96; i++)
      if ((src.bits[i / 32] & (1 << i % 32)) != 0) temp += pow(2, i);
      if ((off = (src.bits[3] & ~0x80000000) >> 16) > 0) {
      for (int i = off; i > 0; i--) temp /= 10.0;
    *dst = (float)temp;
    *dst *= src.bits[3] >> 31 ? -1 : 1;
    result = TRUE;
  }
  return result;
}
int getFloatSign(float *src) { return *(int *)src >> 31; }

int getFloatExp(float *src) { return ((*(int *)src & ~0x80000000) >> 23) - 127; }

int s21_from_float_to_decimal(float src, s21_decimal *dst) {
dst->bits[0] = dst->bits[1] = dst->bits[2] = dst->bits[3] = 0;
  int result = FALSE, sign = getFloatSign(&src), exp = getFloatExp(&src);
  if (isinf(src) && !sign)
    dst->value_type = s21_INFINITY;
  else if (isinf(src) && sign)
    dst->value_type = s21_NEGATIVE_INFINITY;
  else if (isnan(src))
    dst->value_type = s21_NAN;

  if (dst && dst->value_type == s21_NORMAL_VALUE && src != 0) {
    double temp = (double)fabs(src);
    int off = 0;
    for (; off < 28 && (int)temp / (int)pow(2, 21) == 0; temp *= 10, off++) {
    }
    temp = round(temp);
    if (off <= 28 && (exp > -94 && exp < 96)) {
      temp = (float)temp;
      for (; fmod(temp, 10) == 0 && off > 0; off--, temp /= 10) {
      }
      float fl = temp;
      int ui;
      exp = getFloatExp(&fl);
      dst->bits[exp / 32] |= 1 << exp % 32;
      for (int i = exp - 1, j = 22; j >= 0; i--, j--)
        if ((ui & (1 << j)) != 0) dst->bits[i / 32] |= 1 << i % 32;
      dst->bits[3] = (sign << 31) | (off << 16);
      result = TRUE;
    }
  }
  return result;
}

int main() {
    int d1 = 2147483650;
    s21_decimal val_d1;
    s21_from_int_to_decimal(d1, &val_d1);
    print0001(val_d1);
    printf("\n");
    int d2;
    s21_decimal val_d2 = {101, 1, 0, 0};
    s21_from_decimal_to_int(val_d2, &d2);
    printf("%d\n", d2);
    float f1;
    s21_decimal val_f1 = {5, 0, 0, 0};
    print0001(val_f1);
    printf("\n");
    s21_from_decimal_to_float(val_f1, &f1);
    printf("%.32f\n", f1);
    float f2 = 0.5;
    s21_decimal val_f2;
    s21_from_float_to_decimal(f2, &val_f2);
    print0001(val_f2);
    printf("\n");
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
