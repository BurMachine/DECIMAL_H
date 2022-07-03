#include<stdio.h>
#include <string.h>
#include <math.h>
#define TRUE  1
#define FALSE  0
#define s21_ok 0
#define s21_convert_error  1

typedef struct {
  unsigned int bits[4];
} s21_decimal;

typedef union {
  float fl;
  int ui;
}bitsun;

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

 int get_sign(s21_decimal *a) {
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

int get_bit(const s21_decimal a, int bit_number) {
    int result = 0;
    if (bit_number / 32 < 4) {
        unsigned int mask = 1u << (bit_number % 32);
        result = a.bits[bit_number / 32] & mask;
    }
    return !!result;
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
int getFloatSign(float *src) { 
    return *(int *)src >> 31; 
}

int getFloatExp(float *src) { 
    return ((*(int *)src & ~0x80000000) >> 23) - 127; 
}

int s21_from_decimal_to_float(s21_decimal src, float *dst) {
    char result = FALSE;
    int sign = get_sign(&src);
    if (!dst)
        result = TRUE;
    int exp = get_scale(&src);
    long double tmp = 0;
    for (int i = 0; i < 96; i++)
        tmp += pow(2, i) * get_bit(src, i);
    while (exp--)
        tmp /= 10.0;
    if (sign)
        tmp *= -1.0;
    *dst = tmp;
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
      result = TRUE;
    }
  }
  return result;
}

int s21_floor(s21_decimal value, s21_decimal *result) {
  s21_decimal dec1_copy = value;
  int valid_value = 0;
  int sign_dec1 = get_sign(&value);
  int scale_dec1 = get_scale(&value);
  s21_decimal one = {1, 0, 0, 0};
  s21_decimal ten = {10, 0, 0, 0};
  s21_decimal buf;
  init_struct(&buf);
  for (int i = scale_dec1; i > 0; i--) value = div_only_bits(value, ten, &buf);
  set_scale(&value, 0);
  if (s21_is_equal(value, dec1_copy) == TRUE) valid_value = 0;
  if (sign_dec1 && valid_value) {
    value = s21_add(value, one);
    set_sign(result, 1);
  }
  return s21_ok;  
}
// int s21_round(s21_decimal value, s21_decimal *result) {

// }

int s21_negate(s21_decimal value, s21_decimal *result) {
    for (int i = 0; i < 4; i++)
        result->bits[i] = value.bits[i];
    set_sign(result, !get_sign(&value));
    return s21_ok;
}

 int s21_truncate(s21_decimal value, s21_decimal *result) {
    s21_decimal ten = {10, 0, 0, 0};
    s21_decimal res = {0, 0, 0, 0};
    s21_decimal tmp = {0, 0, 0, 0};
    s21_decimal zero = {0, 0, 0, 0};
    int sign = get_sign(&value);
    int scale = get_scale(&value);
    int solution = 0;
    if (!scale) {
        *result = value;
    } else {
        for (int i = scale; i > 0; i--) {
            *result = division_without_scale(value, ten);
            value = *result;
        }
        if (s21_is_equal(*result, zero)) solution = 1;
    }
    if (sign && solution) set_sign(result, 1);
    return solution;
}


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
 s21_decimal f1 = {1, 0, 0, 0};
 s21_decimal f2;
 s21_negate(f1, &f2);
    print0001(f1);
    printf("\n");
    print0001(f2);
    printf("\n");
}
