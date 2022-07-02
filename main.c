#include<stdio.h>
#include "s21_decimal.h"

void print0001(s21_decimal a);

void print01(int i, int szf) {
    while (szf--) {
        printf("%d", (i >> szf) & 1 );
    }
}

int main() {
    s21_decimal c = {0, 0, 0, 0};
    s21_decimal num1 = {16, 0, 0, 0};
    s21_decimal num2 = {1, 0, 0, 0};
    set_scale(&num1, 1);
    set_scale(&num2, 2);
//    set_sign(&num2, 1);
//    set_sign(&num1, 1);
     int a = s21_mul(num1, num2, &c);
     printf("%d\n", a);
     print0001(num1);
     printf("\n");
     print0001(num2);
     printf("\n");
     print0001(c);
     printf("\n");
     printf("%d\n%d", get_scale(&num1), get_scale(&num2));
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

