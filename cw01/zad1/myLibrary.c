//
// Created by wojtek on 15.05.2020.
//

#include "myLibrary.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>

#define max_dlugosc_komendy 131072 //standardowa dla Linuxa
#define mierna_dlugosc_komendy 1024
#define max_ilosc_operacji 131072
#define max_znakow_na_operacje 131072


struct tablica_glowna* stworz_tablice_glowna(int ilosc_blokow){
    struct tablica_glowna* tablica = malloc(sizeof(struct tablica_glowna));
    tablica->ilosc_blokow = ilosc_blokow;
    tablica->bloki = (struct blok_operacji_edycyjnych**) calloc(ilosc_blokow, sizeof(struct blok_operacji_edycyjnych*));
    tablica->index_wolnego_bloku = 0;
    return tablica;
}

struct blok_operacji_edycyjnych* stworz_blok_operacji(int ilosc_operacji){
    struct blok_operacji_edycyjnych* blok = malloc(sizeof(struct blok_operacji_edycyjnych));
    blok->ilosc_operacji = ilosc_operacji;
    blok->operacje_edycyjne = (char**) calloc(ilosc_operacji, sizeof(char*));
    blok->index_wolnej_operacji = 0;
    return blok;
}

FILE* porownaj_pliki(char* sciezka_1, char* sciezka_2){
    char komenda[mierna_dlugosc_komendy] = "diff ";
    strcat(komenda, sciezka_1);
    strcat(komenda, " ");
    strcat(komenda, sciezka_2);
    strcat(komenda, " > wynik_porownania.txt");

    system(komenda);
    FILE* wynik_porownania = fopen("wynik_porownania.txt", "r");
    return wynik_porownania;
}

int utworz_blok_operacji_z_pliku(FILE* plik_tymczasowy, struct tablica_glowna* tablica){

    char* lineptr = NULL;
    size_t n = 0;

    struct blok_operacji_edycyjnych* blok = stworz_blok_operacji(max_ilosc_operacji);

    while ( getline(&lineptr, &n, plik_tymczasowy ) != -1) {

        if( !(lineptr[0] == '<' || lineptr[0] == '>' || lineptr[0] == '-') ){
            blok->operacje_edycyjne[blok->index_wolnej_operacji] = (char*) calloc(max_znakow_na_operacje, sizeof(char));
            strcpy( blok->operacje_edycyjne[ blok->index_wolnej_operacji ], lineptr );
            blok->index_wolnej_operacji++;
        }
        else{
            strcat(blok->operacje_edycyjne[ blok->index_wolnej_operacji-1 ], lineptr);
        }
    }
    free(lineptr);
    blok->index_wolnej_operacji++;
    tablica->bloki[ tablica->index_wolnego_bloku ] = blok;
    tablica->index_wolnego_bloku++;

    return tablica->index_wolnego_bloku-1;
}

int ile_operacji_w_bloku(struct blok_operacji_edycyjnych* blok){
    return blok->index_wolnej_operacji;
}

void usun_blok(int index, struct tablica_glowna* tablica){
    free( tablica->bloki[ index ]->operacje_edycyjne );
    free( tablica->bloki[ index ] );

    if (index != tablica->index_wolnego_bloku-1){   // zamiana ostatniego bloku w wolne miejsce
        tablica->bloki[ index ] = tablica->bloki[ tablica->index_wolnego_bloku-1 ];
    }

    tablica->index_wolnego_bloku--;
}

void usun_operacje_z_bloku(int index, struct blok_operacji_edycyjnych* blok){
    free(blok->operacje_edycyjne[index]);
    if (index != blok->index_wolnej_operacji-1){
        blok->operacje_edycyjne[index] = blok->operacje_edycyjne[ blok->index_wolnej_operacji-1 ];
    }
    blok->index_wolnej_operacji--;
}