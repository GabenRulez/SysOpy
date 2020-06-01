CC= gcc
ATRYBUTY= -Wall




all: build test full_clean

build:
	$(CC) $(ATRYBUTY) -o main main.c

test: main plik_test
	./main plik_test


full_clean:
	rm -rf main
