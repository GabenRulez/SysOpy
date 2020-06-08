#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <math.h>
#include <pthread.h>
#include <unistd.h>

#include "pomocnicze.h"
#include "funkcje_tekstowe.h"

void wyjscie_z_bledem(char* komunikat) {
    wypisz_linie_gwiazdek();
    printf("\nError: %s\n", komunikat);
    printf("Errno: %d\n\n", errno);
    wypisz_linie_gwiazdek();
    exit(EXIT_FAILURE);
}

struct argumenty{
    int start;
    int koniec;
    unsigned int* ilosc_wystapien;
}; typedef struct argumenty argumenty;

unsigned short int* tablica_pliku_wejsciowego;
int szerokosc;
int wysokosc;
int liczba_watkow = -1;


void* histogram_czesc_sign(void* arg){
    int i = *((int*) arg);
    int start = sufit(i * 256, liczba_watkow);
    int koniec = sufit((i+1) * 256, liczba_watkow) - 1;
    printf("Start %d, Koniec %d\n\n", start, koniec);
}

int stworz_histogram(int tryb){
    switch(tryb){
        case 1:
        {
            pthread_t* watki = (pthread_t*)calloc(liczba_watkow, sizeof(pthread_t));
            unsigned int ilosc_wystapien[256] = {0};
            for(int i=0; i<liczba_watkow; i++){
                argumenty* argumenty_watku = malloc(sizeof(argumenty));

                argumenty_watku->start = sufit(i * 256, liczba_watkow);
                argumenty_watku->koniec = sufit((i+1) * 256, liczba_watkow) - 1;
                argumenty_watku->ilosc_wystapien = ilosc_wystapien;
                int* i_temp = malloc(sizeof(int));
                *(i_temp) = i;

                pthread_create(&watki[i], NULL, histogram_czesc_sign, *argumenty_watku);
                usleep(1);
            }
            break;
        }

        case 2:
        {
            break;
        }

        case 3:
        {
            break;
        }

        default:
        {
            wyjscie_z_bledem("Nie ma takiego trybu! Nie powinienes sie tu znalezc.");
            break;
        }
    }
}

int main(int argc, char** argv){
    if (argc<5){
        printf("Zbyt malo argumentow. Podaj <liczba_watkow> <sign / block / interleaved> <plik wejsciowy> <plik wyjsciowy>\n");
        return 1;
    }
    liczba_watkow = (int) strtol(argv[1], (char**)NULL, 10);
    int tryb = 0;
    if(strcmp(argv[2], "sign") == 0) tryb = 1;                  // sign = 1, block = 2, interleaved = 3
    else if(strcmp(argv[2], "block") == 0) tryb = 2;
    else if(strcmp(argv[2], "interleaved") == 0) tryb = 3;
    else wyjscie_z_bledem("Nie istnieje taki tryb.");

    stworz_histogram(1);

    ////////////////////////////////////////////////////////////////////////////////
    
    FILE* plik_wejsciowy = fopen(argv[3], "r");
    if(plik_wejsciowy == NULL) wyjscie_z_bledem("Nie moge otworzyc pliku wejsciowego.");
    
    char* linia;
    size_t len;
    int indeks_wiersza = 0;
    int indeks_kolumny = 0;
    
    while( getline(&linia, &len, plik_wejsciowy) != -1 ){
        if(indeks_wiersza == 1){        // Wczytanie headera z pliku
            char* temp = strtok(linia, " ");
            szerokosc = (int) strtol(temp, (char**)NULL, 10);
            temp = strtok(NULL, "\n");
            wysokosc = (int) strtol(temp, (char**)NULL, 10);
            
            tablica_pliku_wejsciowego = (unsigned short int*)calloc(szerokosc * wysokosc, sizeof(unsigned short int));
        }
        else if(indeks_wiersza > 2){    //wczytanie zawartości z pliku
            char* temp = strtok(linia, "\n\t");
            while(temp != NULL){
                unsigned short int wartosc_piksela = (int) strtol(temp, (char**)NULL, 10);
                if(wartosc_piksela>255) wartosc_piksela = 255;  //dla pewnosci
                tablica_pliku_wejsciowego[indeks_kolumny + szerokosc*indeks_wiersza] = wartosc_piksela;
                temp = strtok(NULL, "\n\t");
                indeks_kolumny++;
            }
        }
        indeks_wiersza++;
        indeks_kolumny = 0;
    }

    fclose(plik_wejsciowy);
    
    //////////////////////////////////////////////////////////////////////////////// już mamy wczytana tablice wartosci




    return 0;
}