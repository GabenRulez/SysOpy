CC=gcc -Wall

all: build test clean

build: klient.c serwer.c
	$(CC) -o klient klient.c
	$(CC) -o serwer serwer.c

test: serwer
	./serwer

clean:
	rm -f klient serwer
