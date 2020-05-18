CC= gcc
ATRYBUTY= -Wall

all: static clean

static:
	$(CC) $(ATRYBUTY) -c myLibrary.c
	ar rcs myLibrary.a myLibrary.o
	$(CC) $(ATRYBUTY) -o main main.c myLibrary.a

	make clean

test:
	@echo "   porownywanie par plikow   "
	@./main 1 create_table 5 compare_pairs ./files/a.txt:./files/b.txt
	
	




clean:
	rm -f *.o