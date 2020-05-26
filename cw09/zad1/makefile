CC= gcc
ATRYBUTY= -Wall


all: build test full_clean

build: main.c
	$(CC) $(ATRYBUTY) -o main main.c -lpthread -lm

test:
	./main 5 10
	make full_clean

clean:
	rm -f macierz

full_clean:
	rm -rf macierz
	rm -rf testy
	rm -rf temp*
	rm -rf A__*
	rm -rf B__*
	rm -rf C__*
