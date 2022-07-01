#include<stdio.h>
#include "s21_decimal.h"

void print0001(s21_decimal a);



union abc {
    int i;
    char c[4];
};

void print01(int i, int szf) {
    while (szf--) {
        printf("%d", (i >> szf) & 1 );
    }
}


int main() {
//    s21_decimal a = {3, 0, 0, 0};
//    s21_decimal b = {0, 3, 2, 0};
    s21_decimal c = {0, 0, 0, 0};
//    s21_add(a, b, &c);
//    print0001(c);
    s21_decimal num1 = {10, 0, 0, 0};
    s21_decimal num2 = {1, 0, 0, 0};
    set_bit(&num1, 95, 1);
    set_bit(&num2, 95, 1);
    set_scale(&num1, 4);
    set_scale(&num2, 1);
    scale_equalize(&num1, &num2);
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

