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

//int main() {
//    union abc uni = {0};
//    uni.c[1] = 0b0001001;
//    print01(uni.i, 32);
//    putchar('\n');
//    print01(uni.c[0], 8);
//    putchar(' ');
//    print01(uni.c[1], 8);
//    putchar(' ');
//    print01(uni.c[2], 8);
//    putchar(' ');
//    print01(uni.c[3], 8);
//
//    s21_decimal a = {{100, 0, 0, 16639}, 0};
//    int b = get_scale(&a);
//
//    //printf("%d", b);
//    return b;
//}
int main() {
    s21_decimal a = {{3, 0, 0, 0}, 0};
//    print01(a.bits[3], 8);
//    printf(" ");
//    print01(a.bits[2], 8);
//    printf(" ");
//    print01(a.bits[1], 8);
//    printf(" ");
//    print01(a.bits[0], 8);
    print0001(a);
    printf("\n%d\n", get_scale(&a));
    set_scale(&a, 28);
    print0001(a);
    printf("\n%d\n", get_scale(&a));
    set_scale(&a, 1);
    print0001(a);
    printf("\n%d\n", get_scale(&a));

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