CC=gcc
CFLAGS=-c -Wall -Wextra -Werror -std=c11
LC=lcov
AFLAGS=ar rc
GCOVFLAGS=--coverage

SOURCES=$(wildcard *.c)
TESTS=$(wildcard ./tests/*test.c)
OBJECTS=$(patsubst %.c, %.o, $(SOURCES))
TESTOBJ=$(patsubst %.c, %.o, $(TESTS))
LIBSRC=$(wildcard s21_*.c)
LIBOBJ=$(patsubst %.c, %.o, $(LIBSRC))

ifeq ($(shell uname -s),Linux)
OPEN_CMD=xdg-open
OFLAGS=-lcheck -lsubunit -lm -lrt -lpthread -lgcov -o 
endif
ifeq ($(shell uname -s),Darwin) # Mac OS
OPEN_CMD=open
OFLAGS= -lcheck -lm -lpthread -lgcov -o   
endif

build: all

all: s21_decimal.a test gcov_report

%.o: %.c
	$(CC) $(CFLAGS) $< $(GCOVFLAGS) -o $@

s21_test%.o: $(TESTS)
	$(CC) $(CFLAGS) $< $(GCOVFLAGS) -o $@

test: $(OBJECTS) $(TESTOBJ)
	$(CC) $(OBJECTS) $(TESTOBJ) $(OFLAGS) $@
	./test

s21_decimal.a:
	$(CC) $(CFLAGS) $(LIBSRC)
	$(AFLAGS) s21_decimal.a $(LIBOBJ)
	rm -rf *.o

gcov_report: test
	./test
	$(LC) -c -d . -o report.info
	lcov --remove report.info -o filtered_report.info
	genhtml filtered_report.info -o ./coverage
	$(OPEN_CMD) ./coverage/index.html

clang:
	cp ../materials/linters/.clang-format .
	clang-format -i *.h *.c tests/*.c
	rm .clang-format
	echo "clang OK"


clean:
	rm -rf *.o *.a *.gcno *.gcda *.info *.html *.css *.png *.out.dSYM main.c *.out
	rm -rf ./coverage
	rm -rf ./tests/*.o ./tests/*.gcda ./tests/*.gcno
	rm -rf test src

rebuild: clean all