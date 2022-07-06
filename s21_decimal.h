
/*#ifndef SRC_S21_DECIMAL_H_
#define SRC_S21_DECIMAL_H_*/
#ifndef _S21_DECIMAL_H_
#define _S21_DECIMAL_H_
//#define TRUE  1
//#define FALSE  0
#define s21_ok 0
#define s21_convert_error  1

#define SIGNMASK 0x80000000
#define OVERFLOW_INT 0x7FFFFFFF
#define MINUS_INFINITY 0xFF800000
#define PLUS_INFINITY 0x7F800000

#include<stdbool.h>
typedef struct {
    unsigned int bits[4];
} s21_decimal;

typedef union {
    float fl;
    int ui;
}bitsun;

typedef enum{
    S21_NORMAL_VALUE,
//    S21_INFINITY,
//    S21_NEGATIVE_INFINITY,
//    S21_NAN
}value_type;

static const char *const binary_powers_of_ten[29] = {
        [0] =
        "000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000001",
        [1] =
        "000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000001010",
        [2] =
        "000000000000000000000000000000000000000000000000000000000000000000000000000000000000000001100100",
        [3] =
        "000000000000000000000000000000000000000000000000000000000000000000000000000000000000001111101000",
        [4] =
        "000000000000000000000000000000000000000000000000000000000000000000000000000000000010011100010000",
        [5] =
        "000000000000000000000000000000000000000000000000000000000000000000000000000000011000011010100000",
        [6] =
        "000000000000000000000000000000000000000000000000000000000000000000000000000011110100001001000000",
        [7] =
        "000000000000000000000000000000000000000000000000000000000000000000000000100110001001011010000000",
        [8] =
        "000000000000000000000000000000000000000000000000000000000000000000000101111101011110000100000000",
        [9] =
        "000000000000000000000000000000000000000000000000000000000000000000111011100110101100101000000000",
        [10] =
        "000000000000000000000000000000000000000000000000000000000000001001010100000010111110010000000000",
        [11] =
        "000000000000000000000000000000000000000000000000000000000001011101001000011101101110100000000000",
        [12] =
        "000000000000000000000000000000000000000000000000000000001110100011010100101001010001000000000000",
        [13] =
        "000000000000000000000000000000000000000000000000000010010001100001001110011100101010000000000000",
        [14] =
        "000000000000000000000000000000000000000000000000010110101111001100010000011110100100000000000000",
        [15] =
        "000000000000000000000000000000000000000000000011100011010111111010100100110001101000000000000000",
        [16] =
        "000000000000000000000000000000000000000000100011100001101111001001101111110000010000000000000000",
        [17] =
        "000000000000000000000000000000000000000101100011010001010111100001011101100010100000000000000000",
        [18] =
        "000000000000000000000000000000000000110111100000101101101011001110100111011001000000000000000000",
        [19] =
        "000000000000000000000000000000001000101011000111001000110000010010001001111010000000000000000000",
        [20] =
        "000000000000000000000000000001010110101111000111010111100010110101100011000100000000000000000000",
        [21] =
        "000000000000000000000000001101100011010111001001101011011100010111011110101000000000000000000000",
        [22] =
        "000000000000000000000010000111100001100111100000110010011011101010110010010000000000000000000000",
        [23] =
        "000000000000000000010101001011010000001011000111111000010100101011110110100000000000000000000000",
        [24] =
        "000000000000000011010011110000100001101111001110110011001110110110100001000000000000000000000000",
        [25] =
        "000000000000100001000101100101010001011000010100000000010100100001001010000000000000000000000000",
        [26] =
        "000000000101001010110111110100101101110011001000000011001101001011100100000000000000000000000000",
        [27] =
        "000000110011101100101110001111001001111111010000100000000011110011101000000000000000000000000000",
        [28] =
        "001000000100111111001110010111100011111000100101000000100110000100010000000000000000000000000000",
};

//''''''''''''''''''''''''''''
typedef enum {
    D_START_EXP = 16,  // Beginning of the Exponent Bits
    D_END_EXP = 23,    // End of the Exponent Bits
    D_MAX_EXP_VAL = 28,
    D_SIGN = 31,  // Sign Bits
} s21_decimal_const;

typedef enum {
    s21_NORMAL_VALUE = 0,
    s21_INFINITY = 1,
    s21_NEGATIVE_INFINITY = 2,
    s21_NAN = 3
} value_type_t;

typedef union {
    unsigned int ui;
    float fl;
} floatbits;

typedef struct {
    unsigned int bits[4];
    value_type_t value_type;
} s22_decimal;



typedef enum {
    ARITHMETIC_OK = 0,
    S21_INFINITY = 1,
    S21_NEGATIVE_INFINITY = 2,
    S21_NAN = 3
} arithmetic_result;

typedef enum {
    POS = 0,
    NEG = 1
} arithmetic_consts;

typedef enum {
    FALSE = 0,
    TRUE = 1,
} comparison_result;

typedef enum {
    CONVERTATION_OK = 0,
    CONVERTATION_ERROR = 1
} convertation_result;

#define IS_SET(X, POS) ((X >> POS) & 1U)
#define ADD_BIT(X, POS) ((X) |= (1U << (POS)))
#define SET_BIT(X, BIT, POS) (X |= (BIT << POS))
#define INIT_ZERO(X) (X &= (0U << 32))
#define ZERO_BIT(X, POS) (X &= ~(1UL << POS));
//'''''''''''''''''''''''''''''

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
s21_decimal get_power_of_ten(int pow);
void handle_exponent_sub(s21_decimal value_1, s21_decimal value_2, s21_decimal *result, int *code);
void handle_exponent_add(s21_decimal value_1, s21_decimal value_2, s21_decimal *result, int *code);
int s21_normalize(s21_decimal *value_1, s21_decimal *value_2);
s21_decimal binary_subtraction(s21_decimal value_1, s21_decimal value_2, int *err);
s21_decimal binary_addition(s21_decimal value_1, s21_decimal value_2, int *err);
s21_decimal bit_and(s21_decimal a, s21_decimal b);
s21_decimal bit_xor(s21_decimal a, s21_decimal b);
s21_decimal bit_lxor(s21_decimal a, s21_decimal b);
s21_decimal bit_land(s21_decimal a, s21_decimal b);
s21_decimal bit_not(s21_decimal a);
void shiftr(s21_decimal *a);
int lshiftl(s21_decimal *a);
int shiftl(s21_decimal *a);
void set_exponent(s21_decimal *decimal, int new_exponent);
int get_exponent(s21_decimal decimal);
void s21_normalize_decimal_pair(s21_decimal *a, s21_decimal *b, int *overflow);
s21_decimal binary_multiplication(s21_decimal value_1, s21_decimal value_2, int *err);
int eq_zero(s21_decimal value);
int eq_zerol(s21_decimal value);
void s21_bank_rounding(s21_decimal *dec, int times);
int bank_rounding(int n);
int s21_int_mod(s21_decimal dividend, s21_decimal divisor, s21_decimal *result);
s21_decimal s21_integer_mod_private(s21_decimal dividend, s21_decimal divisor);
s21_decimal s21_integer_div_private(s21_decimal dividend, s21_decimal divisor, s21_decimal *result);
void handle_exponent_div(s21_decimal value_1, s21_decimal value_2, s21_decimal *result, int *code);
int s21_int_div(s21_decimal dividend, s21_decimal divisor, s21_decimal *result);
bool s21_is_less_positive(s21_decimal a, s21_decimal b);
bool s21_is_less_basic(s21_decimal a, s21_decimal b);
void handle_exponent_mul(s21_decimal value_1, s21_decimal value_2, s21_decimal *result, int *code);
int min(int a, int b);
int max(int a, int b);


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
int s21_negate(s21_decimal value, s21_decimal *result);
int s21_floor(s21_decimal value, s21_decimal *result);
int s21_round(s21_decimal value, s21_decimal *result);




void shift(s22_decimal *a);
void bit_on(s22_decimal *a, int position);
void bit_off(s22_decimal *a, int position);
int no_bits(s22_decimal a);
int decbit(s22_decimal a, int position);
void return_infinity(s22_decimal *a);
char scale(s22_decimal a);
void to_one_scale(s22_decimal *a, s22_decimal *b);
s22_decimal minus(s22_decimal a, s22_decimal b);
int sign_minus(s22_decimal a);
int s21s22_is_equal(s22_decimal dec1, s22_decimal dec2);
s22_decimal s21s22_sub(s22_decimal a, s22_decimal b);
int s21s22_is_less(s22_decimal dec1, s22_decimal dec2);
int is_greater_module(s22_decimal a, s22_decimal b);
int s21s22_is_greater_or_equal(s22_decimal dec1, s22_decimal dec2);
s22_decimal errors_s21mul(s22_decimal a, s22_decimal b);
s22_decimal s21s22_mul(s22_decimal a, s22_decimal b);
int len_of_digit(s22_decimal dig);
s22_decimal add_only_scale(s22_decimal a, int scale);
s22_decimal div_int(s22_decimal a, s22_decimal b, s22_decimal *buf);
int float_errors(float src, s22_decimal *dst);
char getbinexp(float src);
int s21s22_from_float_to_decimal(float src, s22_decimal *dst);
s22_decimal minus_scale(s22_decimal a);
s22_decimal plus(s22_decimal a, s22_decimal b);
s22_decimal s21s22_floor(s22_decimal a);
int detect_minus_result(s22_decimal *a, s22_decimal *b);
s22_decimal s21s22_truncate(s22_decimal a);
s22_decimal s21s22_mod(s22_decimal a, s22_decimal b);
s22_decimal not_a_number(void);
s22_decimal s21s22_round(s22_decimal a);
int last_bit1(s22_decimal a);
s22_decimal plus_scale(s22_decimal a);
s22_decimal s21s22_div(s22_decimal a, s22_decimal b);

#endif // _S21_DECIMAL_H_
//#endif // SRC_S21_DECIMAL_H_
