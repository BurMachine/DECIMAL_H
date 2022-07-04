CC=gcc
CFLAGS= -std=c11 -Wall -Werror -Wextra
GCOVFLAGS=-fprofile-arcs -ftest-coverage
LFLAGS= --coverage
CHECKFLAGS=$(shell pkg-config --libs check) #-lcheck -lm -lpthread -lrt -lsubunit
SRC=s21_decimal.c
OBJ=s21_decimal.o
LIB=s21_decimal.a
OBJ_TARG=test_decimal
TARGET=/
.PHONY: help all clean test s21_decimal.a gcov_report
help:
	@echo "Available targets:"
	@echo "  make test"
	@echo "  make s21_decimal.a"
	@echo "  make gcov_report"
clean:
	rm -rf s21_decimal_test test *.gch *.gcda *.gcno *.a *.o *.json \
	coverage.* *.info ./report/ ./test_decimal.dSYM/
all:	build
build: $(LIB)

rebuild: clean build
s21_decimal.a: $(SRC) s21_decimal.h
	$(CC) $(CFLAGS) -c $(SRC) -o s21_decimal.o
	ar -rc libs21_decimal.a s21_decimal.o
	ranlib libs21_decimal.a
	cp libs21_decimal.a s21_decimal.a
s21_decimal_test.o: #s21_decimal_test.check
#   checkmk ./s21_decimal_test.check >s21_decimal_test.c
	$(CC) $(CFLAGS) -c test_decimal.c -o s21_decimal_test.o
test:   clean s21_decimal_test.o s21_decimal.a
	$(CC) $(CFLAGS) -o s21_decimal_test s21_decimal_test.o -L. -ls21_decimal $(CHECKFLAGS)
#   leaks -atExit -- ./s21_decimal_test
#   valgrind --tool=memcheck --track-origins=yes --leak-check=yes ./s21_decimal_test
	./s21_decimal_test
gcov_lib:   s21_decimal.c s21_decimal.h
	$(CC) $(CFLAGS) $(GCOVFLAGS) $(LFLAGS) -c $(SRC)
	ar -rc libs21_decimal.a $(OBJ)
	ranlib libs21_decimal.a
gcov_test:  s21_decimal_test.check gcov_lib
#   checkmk ./s21_decimal_test.check >s21_decimal_test.c
	$(CC) $(CFLAGS) $(GCOVFLAGS) -c s21_decimal_test.c -o s21_decimal_test.o
	$(CC) $(CFLAGS) $(GCOVFLAGS) $(LFLAGS) s21_decimal_test.o -L./ -ls21_decimal $(CHECKFLAGS) -o s21_decimal_test
gcov_report:	clean gcov_lib gcov_test
	./s21_decimal_test
#   valgrind --tool=memcheck --leak-check=yes ./s21_decimal_test
	lcov -t "s21_decimal_test" -o s21_decimal_test.info -c -d .
	genhtml -o report s21_decimal_test.info
#   gcovr --html-details coverage.html
