CC= gcc
ATRYBUTY= -Wall

all: build test std_clean

build:
	$(CC) $(ATRYBUTY) -o main main.c
	$(CC) $(ATRYBUTY) -o producent producent.c
	$(CC) $(ATRYBUTY) -o konsument konsument.c

test:
	./main


std_clean:
	rm -rf main
	rm -rf producent
	rm -rf konsument
	rm -rf temp_potok

clean:
	make std_clean
	rm -rf wynik.txt
