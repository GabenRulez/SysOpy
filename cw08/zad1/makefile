CC= gcc
ATRYBUTY= -Wall -ggdb3

all: build test clean

build:
	$(CC) $(ATRYBUTY) -o main main.c -lpthread


test:
	./main 10 sign plik1 plik2

clean:
	rm -f main

