CFLAGS = -Wall -Wextra -Werror -pedantic -std=c11
GCOVFLAGS = -fprofile-arcs -ftest-coverage

SOURCES_ALL = calc_compliments.c \
create_matrix.c \
determinant.c \
inverse_matrix.c \
matrix_comparison.c \
mult_matrix.c \
mult_number.c \
remove_matrix.c \
sub_matrix.c \
sum_matrix.c \
transpose.c

OBJ_LIBRARY := $(patsubst %.c, %.o, $(SOURCES_ALL))

%.o: %.c $(HEADERS)
	gcc $(CFLAGS) -c $< -o $@ -lm

all: check s21_matrix.a test valgrind gcov_report 

s21_matrix.a: $(OBJ_LIBRARY) $(HEADERS)
	ar rcs s21_matrix.a $(OBJ_LIBRARY)
	ranlib s21_matrix.a
	rm -rf ./*.o

test: s21_matrix.a test.c
	gcc $(CFLAGS) test.c s21_matrix.a -o test.out -lcheck -lsubunit -lm
	./test.out

gcov_report:
	$(MAKE) clean
	$(MAKE) OBJ_LIBRARY="$(patsubst %.c, %.o, $(SOURCES_ALL))" CFLAGS="$(CFLAGS) $(GCOVFLAGS)" s21_matrix.a
	gcc $(GCOVFLAGS) test.c s21_matrix.a -o report.out -lcheck -lsubunit -lm
	./report.out
	gcov -f $(SOURCES_ALL)
	lcov -t "gcov_report" -o Coverage_Report.info -c -d .
	genhtml -o ./report Coverage_Report.info

check:
	clang-format -n *.c
	clang-format -i *.c
	clang-format -n *.h
	clang-format -i *.h

rebuild: clean all

valgrind: test
	valgrind --tool=memcheck --leak-check=yes ./test.out

clean:
	find . -name "*.o" -delete
	find . -name "*.a" -delete
	find . -name "*.gcno" -delete
	find . -name "*.gcda" -delete
	find . -name "*.info" -delete
	find . -name "*.out" -delete
	find . -name "*.gcov" -delete
	rm -rf ./report
