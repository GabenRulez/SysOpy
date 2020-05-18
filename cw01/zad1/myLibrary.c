//
// Created by wojtek on 15.05.2020.
//

#include "myLibrary.h"
#include <stdlib.h>

struct blok_operacji_edycyjnych{
    char** operacje_edycyjne;
    int ilosc_operacji;

};

struct tablica_glowna{
    struct blok_operacji_edycyjnych** bloki;
    int ilosc_blokow;

};

struct tablica_glowna* stworz_tablice_glowna(int ilosc_blokow){
    struct tablica_glowna* tablica = malloc(sizeof(struct tablica_glowna));
    tablica->ilosc_blokow = ilosc_blokow;
    tablica->bloki = (struct blok_operacji_edycyjnych**) calloc(ilosc_blokow, sizeof(struct blok_operacji_edycyjnych*));
}

struct blok_operacji_edycyjnych* stworz_blok_operacji(int ilosc_operacji){
    struct blok_operacji_edycyjnych* blok = malloc(sizeof(struct blok_operacji_edycyjnych));
    blok->ilosc_operacji = ilosc_operacji;
    blok->operacje_edycyjne = (char**) calloc(ilosc_operacji, sizeof(char*));
}