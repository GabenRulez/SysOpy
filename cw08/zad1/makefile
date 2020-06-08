CC= gcc
ATRYBUTY= -Wall -ggdb3

all: build test clean

build:
	$(CC) $(ATRYBUTY) -o main main.c -lpthread


test:
	./main 4 sign f14.pgm histogram_sign.txt
	./main 4 block f14.pgm histogram_block.txt
	./main 4 interleaved f14.pgm histogram_interleaved.txt

full_test:
	./main 1 sign f14.pgm histogram1.txt
	./main 2 sign f14.pgm histogram2.txt
	./main 4 sign f14.pgm histogram4.txt

clean:
	rm -f main

full_clean:
	make clean
	rm -f histogram.txt
	rm -f histogram1.txt
	rm -f histogram2.txt
	rm -f histogram4.txt
