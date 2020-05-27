CC= gcc
ATRYBUTY= -Wall

all: build test_fork test_exec clean

build:
	$(CC) $(ATRYBUTY) -o main main.c
	$(CC) $(ATRYBUTY) -o exec exec.c

test_fork:
	@echo "---Test Fork---"
	./main fork ignore
	./main fork handler
	./main fork mask
	./main fork pending

test_exec:
	@echo "---Test Exec---"
	./main exec ignore
	./main exec mask
	./main exec pending

clean:
	rm -rf main
	rm -rf exec