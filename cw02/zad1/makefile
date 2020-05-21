CC= gcc
ATRYBUTY= -Wall

all: build

build: main.c
	$(CC) $(ATRYBUTY) -o main main.c





	
shared: myLibrary.c main.c
	$(CC) $(ATRYBUTY) -fPIC -c myLibrary.c
	$(CC) $(ATRYBUTY) -fPIC -shared -o myLibrary.so myLibrary.o
	$(CC) $(ATRYBUTY) -o main main.c myLibrary.so -L. -Wl,-rpath=`pwd`
	make test
	make clean

dynamic: main_dynamic.c
	$(CC) $(ATRYBUTY) -fPIC -c myLibrary.c
	$(CC) $(ATRYBUTY) -fPIC -shared -o myLibrary.so myLibrary.o
	$(CC) $(ATRYBUTY) -o main main_dynamic.c -L. -ldl -D DYNAMIC
	make test
	make clean

test:
	@echo "   porownywanie par plikow   " >> raport3a.txt
	@./main 5 create_table 5 compare_pairs ./pliki/a1_duzy.txt:./pliki/a2_duzy.txt ./pliki/b1.txt:./pliki/b2.txt ./pliki/c1.txt:./pliki/c2.txt ./pliki/d1.txt:./pliki/d2.txt ./pliki/a1.txt:./pliki/b1.txt >> raport3a.txt
	@./main 15 create_table 15 compare_pairs ./pliki/a1_duzy.txt:./pliki/a2_duzy.txt ./pliki/b1.txt:./pliki/b2.txt ./pliki/c1.txt:./pliki/c2.txt ./pliki/d1.txt:./pliki/d2.txt ./pliki/a1.txt:./pliki/b1.txt ./pliki/a1.txt:./pliki/a3.txt ./pliki/b1.txt:./pliki/b3.txt ./pliki/c1.txt:./pliki/c3.txt ./pliki/d1.txt:./pliki/d3.txt ./pliki/a1.txt:./pliki/b1.txt ./pliki/a1.txt:./pliki/a4.txt ./pliki/b1.txt:./pliki/b4.txt ./pliki/c1.txt:./pliki/a3.txt ./pliki/d1.txt:./pliki/d4.txt ./pliki/a1.txt:./pliki/c1.txt >> raport3a.txt
	@./main 50 create_table 50 compare_pairs ./pliki/a1_duzy.txt:./pliki/a2_duzy.txt ./pliki/b1.txt:./pliki/b2.txt ./pliki/c1.txt:./pliki/c2.txt ./pliki/d1.txt:./pliki/d2.txt ./pliki/a1.txt:./pliki/b1.txt ./pliki/a1.txt:./pliki/a3.txt ./pliki/b1.txt:./pliki/b3.txt ./pliki/c1.txt:./pliki/c3.txt ./pliki/d1.txt:./pliki/d3.txt ./pliki/a1.txt:./pliki/b1.txt ./pliki/a1.txt:./pliki/a4.txt ./pliki/b1.txt:./pliki/b4.txt ./pliki/c1.txt:./pliki/a3.txt ./pliki/d1.txt:./pliki/d4.txt ./pliki/a1.txt:./pliki/c1.txt ./pliki/a1.txt:./pliki/a2.txt ./pliki/b1.txt:./pliki/b2.txt ./pliki/c1.txt:./pliki/c2.txt ./pliki/d1.txt:./pliki/d2.txt ./pliki/a1.txt:./pliki/b1.txt ./pliki/a1.txt:./pliki/a3.txt ./pliki/b1.txt:./pliki/b3.txt ./pliki/c1.txt:./pliki/c3.txt ./pliki/d1.txt:./pliki/d3.txt ./pliki/a1.txt:./pliki/b1.txt ./pliki/a1.txt:./pliki/a4.txt ./pliki/b1.txt:./pliki/b4.txt ./pliki/c1.txt:./pliki/a3.txt ./pliki/d1.txt:./pliki/d4.txt ./pliki/a1.txt:./pliki/c1.txt ./pliki/a1.txt:./pliki/a2.txt ./pliki/b1.txt:./pliki/b2.txt ./pliki/c1.txt:./pliki/c2.txt ./pliki/d1.txt:./pliki/d2.txt ./pliki/a1.txt:./pliki/b1.txt ./pliki/a1.txt:./pliki/a3.txt ./pliki/b1.txt:./pliki/b3.txt ./pliki/c1.txt:./pliki/c3.txt ./pliki/d1.txt:./pliki/d3.txt ./pliki/a1.txt:./pliki/b1.txt ./pliki/a1.txt:./pliki/a4.txt ./pliki/b1.txt:./pliki/b4.txt ./pliki/c1.txt:./pliki/a3.txt ./pliki/d1.txt:./pliki/d4.txt ./pliki/a1.txt:./pliki/c1.txt ./pliki/a1.txt:./pliki/a2.txt ./pliki/b1.txt:./pliki/b2.txt ./pliki/c1.txt:./pliki/c2.txt ./pliki/d1.txt:./pliki/d2.txt ./pliki/a1.txt:./pliki/b1.txt >> raport3a.txt

	@echo "\n   zapisywanie blokow o roznych rozmiarach   " >> raport3a.txt
	@./main 5 create_table 5 compare_pairs ./pliki/a1_duzy.txt:./pliki/a2_duzy.txt ./pliki/a1_duzy.txt:./pliki/a3_duzy.txt ./pliki/c1.txt:./pliki/a2_duzy.txt ./pliki/d4.txt:./pliki/a4_duzy.txt ./pliki/a1.txt:./pliki/b1.txt >> raport3a.txt

	@echo "\n   usuwanie blokow o roznych rozmiarach   " >> raport3a.txt
	@./main 5 create_table 5 compare_pairs ./pliki/a1_duzy.txt:./pliki/a2_duzy.txt ./pliki/a1_duzy.txt:./pliki/a3_duzy.txt ./pliki/c1.txt:./pliki/a2_duzy.txt ./pliki/d4.txt:./pliki/a4_duzy.txt ./pliki/a1.txt:./pliki/b1.txt remove_block 0 remove_block 0 remove_block 0 remove_block 0 remove_block 0 >> raport3a.txt

	@echo "\n   naprzemienne dodawanie i usuwanie blokow o roznych rozmiarach   " >> raport3a.txt
	@./main 1 create_table 1 compare_pairs ./pliki/b1.txt:./pliki/b2.txt remove_block 0 compare_pairs ./pliki/b1.txt:./pliki/b2.txt remove_block 0 compare_pairs ./pliki/a1_duzy.txt:./pliki/a2_duzy.txt remove_block 0 compare_pairs ./pliki/a1_duzy.txt:./pliki/a2_duzy.txt remove_block 0 >> raport3a.txt
	make full_clean


clean:
	rm -f *.o
	rm -f main

full_clean:
	rm -f wynik_porownania.txt
	rm -f *.a
	rm -f *.so