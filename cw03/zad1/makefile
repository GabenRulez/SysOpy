CC= gcc
ATRYBUTY= -Wall




all: build test clean full_clean

build: main.c
	$(CC) $(ATRYBUTY) -o main main.c

test:
	mkdir fajnych
	mkdir fajnych/mamy
	mkdir fajnych/mamy/na
	mkdir fajnych/mamy/na/AGH
	mkdir fajnych/mamy/na/AGH/prowadzacych
	touch fajnych/dbaja
	touch fajnych/mamy/nas
	touch fajnych/mamy/na/o
	touch fajnych/mamy/na/AGH/dobrze
	touch fajnych/mamy/na/AGH/prowadzacych/oni

	./main . -maxdepth 6



clean:
	rm -f main

full_clean:
	rm -rf fajnych
