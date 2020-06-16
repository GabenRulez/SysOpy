CC=gcc -Wall

all: build test clean

build: klient.c serwer.c
	$(CC) -o klient klient.c -lpthread
	$(CC) -o serwer serwer.c -lpthread

test: klient serwer
	./serwer 6965 sock

test2:
	./klient tester2 127.0.0.1 6965

test3:
	./klient tester1 LOCAL sock

clean:
	rm -f klient
	rm -f serwer
	rm -f sock
