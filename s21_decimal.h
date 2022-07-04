/*#ifndef SRC_S21_DECIMAL_H_
#define SRC_S21_DECIMAL_H_*/
#ifndef _S21_DECIMAL_H_
#define _S21_DECIMAL_H_
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
int getFloatSign(float *src);
int getFloatExp(float *src);
/* support bit function(not solution of the task) */
int is_greater_or_equal(s21_decimal dec1, s21_decimal dec2);
int is_equal_b(s21_decimal dec1, s21_decimal dec2);
int is_greater(s21_decimal num1, s21_decimal num2);
int is_less_b(s21_decimal num1, s21_decimal num2);
s21_decimal division_without_scale(s21_decimal num1, s21_decimal num2);
s21_decimal div_only_bits(s21_decimal number_1, s21_decimal number_2, s21_decimal *buf);



int s21_from_int_to_decimal(int src, s21_decimal *dst);
int s21_from_decimal_to_int(s21_decimal src, int *dst);
int s21_from_decimal_to_float(s21_decimal src, float *dst);
int s21_from_float_to_decimal(float src, s21_decimal *dst);

int s21_add(s21_decimal value_1, s21_decimal value_2, s21_decimal *result);
int s21_sub(s21_decimal value_1, s21_decimal value_2, s21_decimal *result);
int s21_mul(s21_decimal value_1, s21_decimal value_2, s21_decimal *result);
int s21_div(s21_decimal value_1, s21_decimal value_2, s21_decimal *result);
int s21_mod(s21_decimal value_1, s21_decimal value_2, s21_decimal *result);


int s21_is_greater(s21_decimal value_1, s21_decimal value_2);
int s21_is_less(s21_decimal dec1, s21_decimal dec2);
int s21_is_equal(s21_decimal value_1, s21_decimal value_2);
int s21_is_not_equal(s21_decimal value_1, s21_decimal value_2);
int s21_is_greater_or_equal(s21_decimal value_1, s21_decimal value_2);
int s21_is_less_or_equal(s21_decimal value_1, s21_decimal value_2);


int s21_truncate(s21_decimal value, s21_decimal *result);


#endif // _S21_DECIMAL_H_
//#endif // SRC_S21_DECIMAL_H_