/*#ifndef SRC_S21_DECIMAL_H_
#define SRC_S21_DECIMAL_H_*/
#ifndef _S21_DECIMAL_H_
#define _S21_DECIMAL_H_

//typedef enum {
//  s21_NORMAL_VALUE = 0,
//  s21_INFINITY = 1,
//  s21_NEGATIVE_INFINITY = 2,
//  s21_NAN = 3,
////   s21_ADDCODE = 4
//} value_type_t;

typedef struct {
  int bits[4];
//  value_type_t value_type;
} s21_decimal;

int get_scale(const s21_decimal *varPtr);
void set_sign(s21_decimal *a, int sign_value);
int get_sign(const s21_decimal *a);
void set_bit(s21_decimal *a, int bit_number, int value);
int get_bit(const s21_decimal a, int bit_number);
void set_scale(s21_decimal *varPtr, int scale);

#endif // _S21_DECIMAL_H_
//#endif // SRC_S21_DECIMAL_H_