CC= gcc
ATRYBUTY= -Wall

all: static clean

static:
	$(CC) $(ATRYBUTY) -c myLibrary.c
	ar rcs myLibrary.a myLibrary.o
	$(CC) $(ATRYBUTY) -o main main.c myLibrary.a
	make test
	make clean

test:
	@echo "   porownywanie par plikow   " > raport2.txt
	@./main 5 create_table 5 compare_pairs ./pliki/a1_duzy.txt:./pliki/a2_duzy.txt ./pliki/b1.txt:./pliki/b2.txt ./pliki/c1.txt:./pliki/c2.txt ./pliki/d1.txt:./pliki/d2.txt ./pliki/a1.txt:./pliki/b1.txt >> raport2.txt
	@./main 15 create_table 15 compare_pairs ./pliki/a1_duzy.txt:./pliki/a2_duzy.txt ./pliki/b1.txt:./pliki/b2.txt ./pliki/c1.txt:./pliki/c2.txt ./pliki/d1.txt:./pliki/d2.txt ./pliki/a1.txt:./pliki/b1.txt ./pliki/a1.txt:./pliki/a3.txt ./pliki/b1.txt:./pliki/b3.txt ./pliki/c1.txt:./pliki/c3.txt ./pliki/d1.txt:./pliki/d3.txt ./pliki/a1.txt:./pliki/b1.txt ./pliki/a1.txt:./pliki/a4.txt ./pliki/b1.txt:./pliki/b4.txt ./pliki/c1.txt:./pliki/a3.txt ./pliki/d1.txt:./pliki/d4.txt ./pliki/a1.txt:./pliki/c1.txt >> raport2.txt
	@./main 50 create_table 50 compare_pairs ./pliki/a1_duzy.txt:./pliki/a2_duzy.txt ./pliki/b1.txt:./pliki/b2.txt ./pliki/c1.txt:./pliki/c2.txt ./pliki/d1.txt:./pliki/d2.txt ./pliki/a1.txt:./pliki/b1.txt ./pliki/a1.txt:./pliki/a3.txt ./pliki/b1.txt:./pliki/b3.txt ./pliki/c1.txt:./pliki/c3.txt ./pliki/d1.txt:./pliki/d3.txt ./pliki/a1.txt:./pliki/b1.txt ./pliki/a1.txt:./pliki/a4.txt ./pliki/b1.txt:./pliki/b4.txt ./pliki/c1.txt:./pliki/a3.txt ./pliki/d1.txt:./pliki/d4.txt ./pliki/a1.txt:./pliki/c1.txt ./pliki/a1.txt:./pliki/a2.txt ./pliki/b1.txt:./pliki/b2.txt ./pliki/c1.txt:./pliki/c2.txt ./pliki/d1.txt:./pliki/d2.txt ./pliki/a1.txt:./pliki/b1.txt ./pliki/a1.txt:./pliki/a3.txt ./pliki/b1.txt:./pliki/b3.txt ./pliki/c1.txt:./pliki/c3.txt ./pliki/d1.txt:./pliki/d3.txt ./pliki/a1.txt:./pliki/b1.txt ./pliki/a1.txt:./pliki/a4.txt ./pliki/b1.txt:./pliki/b4.txt ./pliki/c1.txt:./pliki/a3.txt ./pliki/d1.txt:./pliki/d4.txt ./pliki/a1.txt:./pliki/c1.txt ./pliki/a1.txt:./pliki/a2.txt ./pliki/b1.txt:./pliki/b2.txt ./pliki/c1.txt:./pliki/c2.txt ./pliki/d1.txt:./pliki/d2.txt ./pliki/a1.txt:./pliki/b1.txt ./pliki/a1.txt:./pliki/a3.txt ./pliki/b1.txt:./pliki/b3.txt ./pliki/c1.txt:./pliki/c3.txt ./pliki/d1.txt:./pliki/d3.txt ./pliki/a1.txt:./pliki/b1.txt ./pliki/a1.txt:./pliki/a4.txt ./pliki/b1.txt:./pliki/b4.txt ./pliki/c1.txt:./pliki/a3.txt ./pliki/d1.txt:./pliki/d4.txt ./pliki/a1.txt:./pliki/c1.txt ./pliki/a1.txt:./pliki/a2.txt ./pliki/b1.txt:./pliki/b2.txt ./pliki/c1.txt:./pliki/c2.txt ./pliki/d1.txt:./pliki/d2.txt ./pliki/a1.txt:./pliki/b1.txt >> raport2.txt

	@echo "\n   zapisywanie blokow o roznych rozmiarach   " >> raport2.txt
	@./main 5 create_table 5 compare_pairs ./pliki/a1_duzy.txt:./pliki/a2_duzy.txt ./pliki/a1_duzy.txt:./pliki/a3_duzy.txt ./pliki/c1.txt:./pliki/a2_duzy.txt ./pliki/d4.txt:./pliki/a4_duzy.txt ./pliki/a1.txt:./pliki/b1.txt >> raport2.txt

	@echo "\n   usuwanie blokow o roznych rozmiarach   " >> raport2.txt
	@./main 5 create_table 5 compare_pairs ./pliki/a1_duzy.txt:./pliki/a2_duzy.txt ./pliki/a1_duzy.txt:./pliki/a3_duzy.txt ./pliki/c1.txt:./pliki/a2_duzy.txt ./pliki/d4.txt:./pliki/a4_duzy.txt ./pliki/a1.txt:./pliki/b1.txt remove_block 0 remove_block 0 remove_block 0 remove_block 0 remove_block 0 >> raport2.txt

	@echo "\n   naprzemienne dodawanie i usuwanie blokow o roznych rozmiarach   " >> raport2.txt
	@./main 1 create_table 1 compare_pairs ./pliki/b1.txt:./pliki/b2.txt remove_block 0 compare_pairs ./pliki/b1.txt:./pliki/b2.txt remove_block 0 compare_pairs ./pliki/a1_duzy.txt:./pliki/a2_duzy.txt remove_block 0 compare_pairs ./pliki/a1_duzy.txt:./pliki/a2_duzy.txt remove_block 0 >> raport2.txt
	make full_clean


clean:
	rm -f *.o

full_clean:
	rm -f main
	rm -f wynik_porownania.txt
	rm -f *.a