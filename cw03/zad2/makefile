CC= gcc
ATRYBUTY= -Wall -ggdb3




all: build build_test test full_clean

build: macierz.c
	$(CC) $(ATRYBUTY) -o macierz macierz.c

build_test: macierz.c
	$(CC) $(ATRYBUTY) testy.c -lm -o testy

test:
	./testy 1 15 7
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
