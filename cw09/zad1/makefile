CC= gcc
ATRYBUTY= -Wall


all: build test clean

build: main.c
	$(CC) $(ATRYBUTY) -o main main.c -lpthread -lm

test:
	@echo " Test przepelnienia "
	./main 2 10

	@echo " Test luzny "
	./main 15 10

clean:
	rm -rf main
