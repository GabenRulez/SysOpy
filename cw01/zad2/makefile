CC= gcc
ATRYBUTY= -Wall

all: static shared clean

static:
	$(CC) $(ATRYBUTY) -c myLibrary.c
	ar rcs myLibrary.a myLibrary.o
	make clean














clean:
	rm -f *.o