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
	@echo "" > Times.txt
	./main 1 sign f14.pgm temp >> Times.txt
	./main 1 block f14.pgm temp >> Times.txt
	./main 1 interleaved f14.pgm temp >> Times.txt

	./main 2 sign f14.pgm temp >> Times.txt
	./main 2 block f14.pgm temp >> Times.txt
	./main 2 interleaved f14.pgm temp >> Times.txt

	./main 4 sign f14.pgm temp >> Times.txt
	./main 4 block f14.pgm temp >> Times.txt
	./main 4 interleaved f14.pgm temp >> Times.txt

	./main 8 sign f14.pgm temp >> Times.txt
	./main 8 block f14.pgm temp >> Times.txt
	./main 8 interleaved f14.pgm temp >> Times.txt

	make full_clean

clean:
	rm -f main
	rm -f histogram_sign.txt
	rm -f histogram_block.txt
	rm -f histogram_interleaved.txt

full_clean:
	make clean
	rm -f temp

