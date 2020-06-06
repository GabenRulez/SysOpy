CC=gcc -Wall

all: build test clean

build: klient.c serwer.c
	$(CC) -o klient klient.c -lrt
	$(CC) -o serwer serwer.c -lrt

test: serwer
	./serwer

clean:
	rm -f klient serwer
