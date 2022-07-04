#include<stdio.h>
#include "s21_decimal.h"
#include <string.h>

void print0001(s21_decimal a);

void print01(int i, int szf) {
    while (szf--) {
        printf("%d", (i >> szf) & 1 );
    }
}

//int main() {
//    s21_decimal x = {{0, 0, 3000000000, 0}};
//    s21_decimal y = {{0, 0, 3000000000, 0}};
//    s21_decimal z = {{0, 0, 0, 0}};
//    s21_add(x, y, &z);
//    char res1[1000], res2[1000] = "0 0 0 0";
//    snprintf(res1, sizeof(char)*1000, "%u %u %u %u", z.bits[0], z.bits[1], z.bits[2], z.bits[3]);
//    int a = strcmp(res1, res2);
//    printf("%d\n", a);
//}


int main () {
    s21_decimal x = {{142, 0, 0, 0}};
    s21_decimal y = {{0, 0, 0, 0}};
    s21_decimal z = {{0, 0, 0, 0}};
    set_scale(&x, 2);
    int a = s21_floor(x, &y);
    printf("%d\n", a);
    print0001(y);
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

