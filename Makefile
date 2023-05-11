CC := gcc
CFLAGS := -Wall -O2

.PHONY: all
all: vec_test

.PHONY: run
run: all
	./vec_test

vec_test:
	$(CC) $(CFLAGS) -o vec_test vec_test.c