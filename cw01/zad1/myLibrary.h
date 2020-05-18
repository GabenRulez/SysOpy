//
// Created by wojtek on 15.05.2020.
//

#ifndef SYSOPY_MYLIBRARY_H
#define SYSOPY_MYLIBRARY_H


#include <stdio.h>

struct blok_operacji_edycyjnych{
    char** operacje_edycyjne;
    int ilosc_operacji;
    int index_wolnej_operacji;

};

struct tablica_glowna{
    struct blok_operacji_edycyjnych** bloki;
    int ilosc_blokow;
    int index_wolnego_bloku;
};

struct tablica_glowna* stworz_tablice_glowna(int ilosc_blokow);

struct blok_operacji_edycyjnych* stworz_blok_operacji(int ilosc_operacji);

FILE* porownaj_pliki(char* sciezka_1, char* sciezka_2);

int utworz_blok_operacji_z_pliku(FILE* plik_tymczasowy, struct tablica_glowna* tablica);

int ile_operacji_w_bloku(struct blok_operacji_edycyjnych* blok);

void usun_blok(int index, struct tablica_glowna* tablica);

void usun_operacje_z_bloku(int index, struct blok_operacji_edycyjnych* blok);

#endif //SYSOPY_MYLIBRARY_H
