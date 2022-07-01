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

#define TRUE  1
#define FALSE  0

typedef struct {
  int bits[4];
//  value_type_t value_type;
} s21_decimal;

typedef enum{
    NORMAL_VALUE,
    INFINITY,
    NEGATIVE_INFINITY,
    NAN
}value_type;

int get_scale(const s21_decimal *varPtr);
void set_sign(s21_decimal *a, int sign_value);
int get_sign(const s21_decimal *a);
void set_bit(s21_decimal *a, int bit_number, int value);
int get_bit(const s21_decimal a, int bit_number);
void set_scale(s21_decimal *varPtr, int scale);
int last_bit(s21_decimal number);
int offset_left(s21_decimal *num_ptr, int value_offset);
void init_struct(s21_decimal *varPtr);
void clear_bits(s21_decimal *varPtr);
void copy_bits(s21_decimal src, s21_decimal *dest);
s21_decimal div_only_bits(s21_decimal number_1, s21_decimal number_2, s21_decimal *buf);
s21_decimal bit_add(s21_decimal *a, s21_decimal *b, int *error_code);
void convert_to_addcode(s21_decimal *number_1);
int scale_equalize(s21_decimal *number1, s21_decimal *number2);
int negative(s21_decimal value_1, s21_decimal value_2);
void check_scale(s21_decimal *value_1, s21_decimal *value_2);

/* support bit function(not solution of the task) */
int is_greater_or_equal(s21_decimal dec1, s21_decimal dec2);
int is_equal_b(s21_decimal dec1, s21_decimal dec2);
int is_greater(s21_decimal num1, s21_decimal num2);
int is_less_b(s21_decimal num1, s21_decimal num2);
s21_decimal division_without_scale(s21_decimal num1, s21_decimal num2);



int s21_add(s21_decimal value_1, s21_decimal value_2, s21_decimal *result);

int s21_is_greater(s21_decimal value_1, s21_decimal value_2);
int s21_is_less(s21_decimal dec1, s21_decimal dec2);

#endif // _S21_DECIMAL_H_
//#endif // SRC_S21_DECIMAL_H_