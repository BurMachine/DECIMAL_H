#include "s21_decimal.h"

/**
 * @brief Предварительные проверки для check_for_add
 *
 * @param num1 первое слагаемое
 * @param num2 второе слагаемое
 * @return s21_decimal число с выставленным value_type
 */
/*
num1      num2
+inf      +-normal  = +inf    - в простой проверке
-inf      +-normal  = -inf    - в простой проверке
nan       any       = nan     - в простой проверке
+inf      -inf      = nan     - отдельная
*/
s21_decimal res_for_add(s21_decimal num1, s21_decimal num2) {
    s21_decimal result = {{0, 0, 0, 0}, s21_NORMAL_VALUE};



    return result;
}