CC= gcc
ATRYBUTY=-Wall

all: init

init:
	@echo "   " > raport3b.txt
	@echo "\n ________ Flaga -O1 ________ " >> raport3b.txt
	make FLAG=1 every_one
	@echo "\n ________ Flaga -O2 ________ " >> raport3b.txt
	make FLAG=2 every_one
	@echo "\n ________ Flaga -O3 ________ " >> raport3b.txt
	make FLAG=3 every_one


every_one:
	@echo "   " >> raport3b.txt
	@echo "\n ____ Biblioteka statyczna ____ " >> raport3b.txt
	make FLAG=$(FLAG) static
	@echo "\n ____ Biblioteka dzielona ____ " >> raport3b.txt
	make FLAG=$(FLAG) shared
	@echo "\n ____ Biblioteka dynamiczna ____ " >> raport3b.txt
	make FLAG=$(FLAG) dynamic


static: myLibrary.c main.c
	$(CC) $(ATRYBUTY) -O$(FLAG) -c myLibrary.c
	ar rcs myLibrary.a myLibrary.o
	$(CC) $(ATRYBUTY) -O$(FLAG) -o main main.c myLibrary.a
	make test
	make clean
	
shared: myLibrary.c main.c
	$(CC) $(ATRYBUTY) -O$(FLAG) -fPIC -c myLibrary.c
	$(CC) $(ATRYBUTY) -O$(FLAG) -fPIC -shared -o myLibrary.so myLibrary.o
	$(CC) $(ATRYBUTY) -O$(FLAG) -o main main.c myLibrary.so -L. -Wl,-rpath=`pwd`
	make test
	make clean

dynamic: main_dynamic.c
	$(CC) $(ATRYBUTY) -O$(FLAG) -fPIC -c myLibrary.c
	$(CC) $(ATRYBUTY) -O$(FLAG) -fPIC -shared -o myLibrary.so myLibrary.o
	$(CC) $(ATRYBUTY) -O$(FLAG) -o main main_dynamic.c -L. -ldl -D DYNAMIC
	make test
	make clean

test:
	@echo "   porownywanie par plikow   " >> raport3b.txt
	@./main 5 create_table 5 compare_pairs ./pliki/a1_duzy.txt:./pliki/a2_duzy.txt ./pliki/b1.txt:./pliki/b2.txt ./pliki/c1.txt:./pliki/c2.txt ./pliki/d1.txt:./pliki/d2.txt ./pliki/a1.txt:./pliki/b1.txt >> raport3b.txt
	@./main 15 create_table 15 compare_pairs ./pliki/a1_duzy.txt:./pliki/a2_duzy.txt ./pliki/b1.txt:./pliki/b2.txt ./pliki/c1.txt:./pliki/c2.txt ./pliki/d1.txt:./pliki/d2.txt ./pliki/a1.txt:./pliki/b1.txt ./pliki/a1.txt:./pliki/a3.txt ./pliki/b1.txt:./pliki/b3.txt ./pliki/c1.txt:./pliki/c3.txt ./pliki/d1.txt:./pliki/d3.txt ./pliki/a1.txt:./pliki/b1.txt ./pliki/a1.txt:./pliki/a4.txt ./pliki/b1.txt:./pliki/b4.txt ./pliki/c1.txt:./pliki/a3.txt ./pliki/d1.txt:./pliki/d4.txt ./pliki/a1.txt:./pliki/c1.txt >> raport3b.txt
	@./main 50 create_table 50 compare_pairs ./pliki/a1_duzy.txt:./pliki/a2_duzy.txt ./pliki/b1.txt:./pliki/b2.txt ./pliki/c1.txt:./pliki/c2.txt ./pliki/d1.txt:./pliki/d2.txt ./pliki/a1.txt:./pliki/b1.txt ./pliki/a1.txt:./pliki/a3.txt ./pliki/b1.txt:./pliki/b3.txt ./pliki/c1.txt:./pliki/c3.txt ./pliki/d1.txt:./pliki/d3.txt ./pliki/a1.txt:./pliki/b1.txt ./pliki/a1.txt:./pliki/a4.txt ./pliki/b1.txt:./pliki/b4.txt ./pliki/c1.txt:./pliki/a3.txt ./pliki/d1.txt:./pliki/d4.txt ./pliki/a1.txt:./pliki/c1.txt ./pliki/a1.txt:./pliki/a2.txt ./pliki/b1.txt:./pliki/b2.txt ./pliki/c1.txt:./pliki/c2.txt ./pliki/d1.txt:./pliki/d2.txt ./pliki/a1.txt:./pliki/b1.txt ./pliki/a1.txt:./pliki/a3.txt ./pliki/b1.txt:./pliki/b3.txt ./pliki/c1.txt:./pliki/c3.txt ./pliki/d1.txt:./pliki/d3.txt ./pliki/a1.txt:./pliki/b1.txt ./pliki/a1.txt:./pliki/a4.txt ./pliki/b1.txt:./pliki/b4.txt ./pliki/c1.txt:./pliki/a3.txt ./pliki/d1.txt:./pliki/d4.txt ./pliki/a1.txt:./pliki/c1.txt ./pliki/a1.txt:./pliki/a2.txt ./pliki/b1.txt:./pliki/b2.txt ./pliki/c1.txt:./pliki/c2.txt ./pliki/d1.txt:./pliki/d2.txt ./pliki/a1.txt:./pliki/b1.txt ./pliki/a1.txt:./pliki/a3.txt ./pliki/b1.txt:./pliki/b3.txt ./pliki/c1.txt:./pliki/c3.txt ./pliki/d1.txt:./pliki/d3.txt ./pliki/a1.txt:./pliki/b1.txt ./pliki/a1.txt:./pliki/a4.txt ./pliki/b1.txt:./pliki/b4.txt ./pliki/c1.txt:./pliki/a3.txt ./pliki/d1.txt:./pliki/d4.txt ./pliki/a1.txt:./pliki/c1.txt ./pliki/a1.txt:./pliki/a2.txt ./pliki/b1.txt:./pliki/b2.txt ./pliki/c1.txt:./pliki/c2.txt ./pliki/d1.txt:./pliki/d2.txt ./pliki/a1.txt:./pliki/b1.txt >> raport3b.txt

	@echo "\n   zapisywanie blokow o roznych rozmiarach   " >> raport3b.txt
	@./main 5 create_table 5 compare_pairs ./pliki/a1_duzy.txt:./pliki/a2_duzy.txt ./pliki/a1_duzy.txt:./pliki/a3_duzy.txt ./pliki/c1.txt:./pliki/a2_duzy.txt ./pliki/d4.txt:./pliki/a4_duzy.txt ./pliki/a1.txt:./pliki/b1.txt >> raport3b.txt

	@echo "\n   usuwanie blokow o roznych rozmiarach   " >> raport3b.txt
	@./main 5 create_table 5 compare_pairs ./pliki/a1_duzy.txt:./pliki/a2_duzy.txt ./pliki/a1_duzy.txt:./pliki/a3_duzy.txt ./pliki/c1.txt:./pliki/a2_duzy.txt ./pliki/d4.txt:./pliki/a4_duzy.txt ./pliki/a1.txt:./pliki/b1.txt remove_block 0 remove_block 0 remove_block 0 remove_block 0 remove_block 0 >> raport3b.txt

	@echo "\n   naprzemienne dodawanie i usuwanie blokow o roznych rozmiarach   " >> raport3b.txt
	@./main 1 create_table 1 compare_pairs ./pliki/b1.txt:./pliki/b2.txt remove_block 0 compare_pairs ./pliki/b1.txt:./pliki/b2.txt remove_block 0 compare_pairs ./pliki/a1_duzy.txt:./pliki/a2_duzy.txt remove_block 0 compare_pairs ./pliki/a1_duzy.txt:./pliki/a2_duzy.txt remove_block 0 >> raport3b.txt
	make full_clean




clean:
	rm -f *.o
	rm -f main

full_clean:
	rm -f wynik_porownania.txt
	rm -f *.a
	rm -f *.so