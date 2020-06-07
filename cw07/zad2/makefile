CC= gcc
ATRYBUTY= -Wall -ggdb3

all: build test clean

build:
	$(CC) $(ATRYBUTY) -o main main.c -pthread -lrt
	$(CC) $(ATRYBUTY) -o pracownik_1 pracownik_1.c -pthread -lrt
	$(CC) $(ATRYBUTY) -o pracownik_2 pracownik_2.c -pthread -lrt
	$(CC) $(ATRYBUTY) -o pracownik_3 pracownik_3.c -pthread -lrt

test:
	./main 2 2 2

clean:
	rm -f main
	rm -f pracownik_1
	rm -f pracownik_2
	rm -f pracownik_3

