CC= gcc
ATRYBUTY= -Wall
wyniki = "wyniki.txt"

a1B = 45000
a4B = 40000
a512B = 30000
a1024B = 20000
a4096B = 15000
a8192B = 10000
b1B = 15000
b4B = 20000
b512B = 12000
b1024B = 10000
b4096B = 8000
b8192B = 5500


all: build test clean

build: main.c
	$(CC) $(ATRYBUTY) -o main main.c

test:
	@echo " " > wyniki.txt
	@echo "     Generowanie plikow do testow     "
	@echo "     Generowanie plikow do testow     "      >>$(wyniki)
	@echo "Pliki A:"                                    >>$(wyniki)
	@./main generate tekst_1B_a $(a1B) 1                >>$(wyniki)
	@./main generate tekst_4B_a $(a4B) 4                >>$(wyniki)
	@./main generate tekst_512B_a $(a512B) 512          >>$(wyniki)
	@./main generate tekst_1024B_a $(a1024B) 1024       >>$(wyniki)
	@./main generate tekst_4096B_a $(a4096B) 4096       >>$(wyniki)
	@./main generate tekst_8192B_a $(a8192B) 8192       >>$(wyniki)
	@echo "Pliki B:"                                    >>$(wyniki)
	@./main generate tekst_1B_b $(b1B) 1                >>$(wyniki)
	@./main generate tekst_4B_b $(b4B) 4                >>$(wyniki)
	@./main generate tekst_512B_b $(b512B) 512          >>$(wyniki)
	@./main generate tekst_1024B_b $(b1024B) 1024       >>$(wyniki)
	@./main generate tekst_4096B_b $(b4096B) 4096       >>$(wyniki)
	@./main generate tekst_8192B_b $(b8192B) 8192       >>$(wyniki)

	@echo " "                                           >>$(wyniki)

	@echo "     Kopiowanie sys     "
	@echo "     Kopiowanie sys     "                                    >>$(wyniki)
	@echo "Pliki A:"                                                    >>$(wyniki)
	@./main copy tekst_1B_a tekst_1B_a_sys $(a1B) 1 sys                 >>$(wyniki)
	@./main copy tekst_4B_a tekst_4B_a_sys $(a4B) 4 sys                 >>$(wyniki)
	@./main copy tekst_512B_a tekst_512B_a_sys $(a512B) 512 sys         >>$(wyniki)
	@./main copy tekst_1024B_a tekst_1024B_a_sys $(a1024B) 1024 sys     >>$(wyniki)
	@./main copy tekst_4096B_a tekst_4096B_a_sys $(a4096B) 4096 sys     >>$(wyniki)
	@./main copy tekst_8192B_a tekst_8192B_a_sys $(a8192B) 8192 sys     >>$(wyniki)
	@echo "Pliki B:"                                                    >>$(wyniki)
	@./main copy tekst_1B_b tekst_1B_b_sys $(b1B) 1 sys                 >>$(wyniki)
	@./main copy tekst_4B_b tekst_4B_b_sys $(b4B) 4 sys                 >>$(wyniki)
	@./main copy tekst_512B_b tekst_512B_b_sys $(b512B) 512 sys         >>$(wyniki)
	@./main copy tekst_1024B_b tekst_1024B_b_sys $(b1024B) 1024 sys     >>$(wyniki)
	@./main copy tekst_4096B_b tekst_4096B_b_sys $(b4096B) 4096 sys     >>$(wyniki)
	@./main copy tekst_8192B_b tekst_8192B_b_sys $(b8192B) 8192 sys     >>$(wyniki)
	
	@echo " "                                                           >>$(wyniki)

	@echo "     Kopiowanie lib     "
	@echo "     Kopiowanie lib     "                                    >>$(wyniki)
	@echo "Pliki A:"                                                    >>$(wyniki)
	@./main copy tekst_1B_a tekst_1B_a_lib $(a1B) 1 lib                 >>$(wyniki)
	@./main copy tekst_4B_a tekst_4B_a_lib $(a4B) 4 lib                 >>$(wyniki)
	@./main copy tekst_512B_a tekst_512B_a_lib $(a512B) 512 lib         >>$(wyniki)
	@./main copy tekst_1024B_a tekst_1024B_a_lib $(a1024B) 1024 lib     >>$(wyniki)
	@./main copy tekst_4096B_a tekst_4096B_a_lib $(a4096B) 4096 lib     >>$(wyniki)
	@./main copy tekst_8192B_a tekst_8192B_a_lib $(a8192B) 8192 lib     >>$(wyniki)
	@echo "Pliki B:"                                                    >>$(wyniki)
	@./main copy tekst_1B_b tekst_1B_b_lib $(b1B) 1 lib                 >>$(wyniki)
	@./main copy tekst_4B_b tekst_4B_b_lib $(b4B) 4 lib                 >>$(wyniki)
	@./main copy tekst_512B_b tekst_512B_b_lib $(b512B) 512 lib         >>$(wyniki)
	@./main copy tekst_1024B_b tekst_1024B_b_lib $(b1024B) 1024 lib     >>$(wyniki)
	@./main copy tekst_4096B_b tekst_4096B_b_lib $(b4096B) 4096 lib     >>$(wyniki)
	@./main copy tekst_8192B_b tekst_8192B_b_lib $(b8192B) 8192 lib     >>$(wyniki)
	
	@echo " "                                                           >>$(wyniki)

	@echo "     Sortowanie sys     "
	@echo "     Sortowanie sys     "                        >>$(wyniki)
	@echo "Pliki A:"                                        >>$(wyniki)
	@./main sort tekst_1B_a_sys $(a1B) 1 sys                >>$(wyniki)
	@./main sort tekst_4B_a_sys $(a4B) 4 sys                >>$(wyniki)
	@./main sort tekst_512B_a_sys $(a512B) 512 sys          >>$(wyniki)
	@./main sort tekst_1024B_a_sys $(a1024B) 1024 sys       >>$(wyniki)
	@./main sort tekst_4096B_a_sys $(a4096B) 4096 sys       >>$(wyniki)
	@./main sort tekst_8192B_a_sys $(a8192B) 8192 sys       >>$(wyniki)
	@echo "Pliki B:"                                        >>$(wyniki)
	@./main sort tekst_1B_b_sys $(b1B) 1 sys                >>$(wyniki)
	@./main sort tekst_4B_b_sys $(b4B) 4 sys                >>$(wyniki)
	@./main sort tekst_512B_b_sys $(b512B) 512 sys          >>$(wyniki)
	@./main sort tekst_1024B_b_sys $(b1024B) 1024 sys       >>$(wyniki)
	@./main sort tekst_4096B_b_sys $(b4096B) 4096 sys       >>$(wyniki)
	@./main sort tekst_8192B_b_sys $(b8192B) 8192 sys       >>$(wyniki)
	
	@echo " "                                               >>$(wyniki)	

	@echo "     Sortowanie lib     "
	@echo "     Sortowanie lib     "                        >>$(wyniki)
	@echo "Pliki A:"                                        >>$(wyniki)
	@./main sort tekst_1B_a_lib $(a1B) 1 lib                >>$(wyniki)
	@./main sort tekst_4B_a_lib $(a4B) 4 lib                >>$(wyniki)
	@./main sort tekst_512B_a_lib $(a512B) 512 lib          >>$(wyniki)
	@./main sort tekst_1024B_a_lib $(a1024B) 1024 lib       >>$(wyniki)
	@./main sort tekst_4096B_a_lib $(a4096B) 4096 lib       >>$(wyniki)
	@./main sort tekst_8192B_a_lib $(a8192B) 8192 lib       >>$(wyniki)
	@echo "Pliki B:"                                        >>$(wyniki)
	@./main sort tekst_1B_b_lib $(b1B) 1 lib                >>$(wyniki)
	@./main sort tekst_4B_b_lib $(b4B) 4 lib                >>$(wyniki)
	@./main sort tekst_512B_b_lib $(b512B) 512 lib          >>$(wyniki)
	@./main sort tekst_1024B_b_lib $(b1024B) 1024 lib       >>$(wyniki)
	@./main sort tekst_4096B_b_lib $(b4096B) 4096 lib       >>$(wyniki)
	@./main sort tekst_8192B_b_lib $(b8192B) 8192 lib       >>$(wyniki)



clean:
	rm -f main
	rm -f tekst_*