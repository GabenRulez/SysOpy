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
char* plik_wyjsciowy;
int szerokosc;
int wysokosc;
int liczba_watkow = -1;
unsigned int ilosc_wystapien[256] = {0};

void ukaz_wyniki(){
    printf("\n");
    wypisz_wysrodkowane("--- Tworze histogram ---");

    unsigned int ilosc_pikseli = 0;
    for(int i=0; i<256; i++) ilosc_pikseli+= ilosc_wystapien[i];
    if( ilosc_pikseli != szerokosc*wysokosc ) wyjscie_z_bledem("Nastapil blad jednostajnego dostepu do pamieci wspolnej.");

    FILE * odpowiedni_plik_wyjsciowy = fopen(plik_wyjsciowy, "w+");
    for(int i=0; i<256; i++) fprintf(odpowiedni_plik_wyjsciowy, "%d %d\n", i, ilosc_wystapien[i]);
    fclose(odpowiedni_plik_wyjsciowy);
}

void* histogram_czesc_sign(void* arg){
    struct timeval czas_startu;
    gettimeofday(&czas_startu, NULL);


    int i = *((int*) arg);
    int start = sufit(i * 256, liczba_watkow);
    int koniec = sufit((i+1) * 256, liczba_watkow) - 1;
    for(int mierzona_wartosc = start; mierzona_wartosc <= koniec; mierzona_wartosc++){
        for(int j=0; j<szerokosc*wysokosc; j++){
            int temp = tablica_pliku_wejsciowego[j];
            if( temp == mierzona_wartosc ){
                ilosc_wystapien[mierzona_wartosc]++;
            }
        }
    }



    struct timeval czas_konca;
    gettimeofday(&czas_konca, NULL);
    long* wynik_w_mikrosekundach = malloc(sizeof(long));
    *(wynik_w_mikrosekundach) = (czas_konca.tv_sec - czas_startu.tv_sec) * 1000000 + czas_konca.tv_usec - czas_startu.tv_usec;
    pthread_exit(wynik_w_mikrosekundach);
}

int stworz_histogram(int tryb){
    switch(tryb){
        case 1:
        {
            pthread_t* watki = (pthread_t*)calloc(liczba_watkow, sizeof(pthread_t));
            for(int i=0; i<256; i++) ilosc_wystapien[i] = 0;
            for(int i=0; i<liczba_watkow; i++){

                int* i_temp = malloc(sizeof(int));
                *(i_temp) = i;

                pthread_create(&watki[i], NULL, histogram_czesc_sign, i_temp);
                usleep(1);
            };
            for(int i=0; i<liczba_watkow; i++){

                void* temp;
                if( pthread_join(watki[i], &temp) > 1 ) wyjscie_z_bledem("Nie moge skonczyc watku.");
                long czas_w_mikrosekundach = *((long*) temp);
                printf("\nWatek %lu zakonczyl prace w %ld mikrosekund.", watki[i], czas_w_mikrosekundach);
                usleep(1);
            }
            ukaz_wyniki();
            break;
        }

        case 2:
        {
            printf("\nhuh2\n");
            break;
        }

        case 3:
        {
            printf("\nhuh3\n");
            break;
        }

        default:
        {
            wyjscie_z_bledem("Nie ma takiego trybu! Nie powinienes sie tu znalezc.");
            break;
        }
    }
    return 0;
}

int main(int argc, char** argv){
    if (argc<5){
        printf("Zbyt malo argumentow. Podaj <liczba_watkow> <sign / block / interleaved> <plik wejsciowy> <plik wyjsciowy>\n");
        return 1;
    }
    liczba_watkow = (int) strtol(argv[1], (char**)NULL, 10);
    int tryb;
    if(strcmp(argv[2], "sign") == 0) tryb = 1;                  // sign = 1, block = 2, interleaved = 3
    else if(strcmp(argv[2], "block") == 0) tryb = 2;
    else if(strcmp(argv[2], "interleaved") == 0) tryb = 3;
    else wyjscie_z_bledem("Nie istnieje taki tryb.");

    ////////////////////////////////////////////////////////////////////////////////

    FILE* plik_wejsciowy = fopen(argv[3], "r");
    if(plik_wejsciowy == NULL) wyjscie_z_bledem("Nie moge otworzyc pliku wejsciowego.");
    
    char* linia;
    size_t len = 0;
    char* temp;
    int indeks_wiersza = 0;
    int indeks_piksela = 0;

    while( getline(&linia, &len, plik_wejsciowy) != -1 ){
        if(indeks_wiersza == 1){        // Wczytanie headera z pliku
            temp = strtok(linia, " ");
            szerokosc = (int) strtol(temp, (char**)NULL, 10);
            temp = strtok(NULL, "\n");
            wysokosc = (int) strtol(temp, (char**)NULL, 10);
            
            tablica_pliku_wejsciowego = (unsigned short int*)calloc(szerokosc * wysokosc, sizeof(unsigned short int));
        }
        else if(indeks_wiersza > 2){    //wczytanie zawartości z pliku
            temp = strtok(linia, " \n\t");
            while(temp != NULL){
                unsigned short int wartosc_piksela = (int) strtol(temp, (char**)NULL, 10);
                if(wartosc_piksela>255) wartosc_piksela = 255;  //dla pewnosci
                tablica_pliku_wejsciowego[indeks_piksela] = (unsigned short int) wartosc_piksela;
                temp = strtok(NULL, " \n\t");
                indeks_piksela++;
            }
        }
        indeks_wiersza++;
    }
    fclose(plik_wejsciowy);
    
    //////////////////////////////////////////////////////////////////////////////// już mamy wczytana tablice wartosci
    plik_wyjsciowy = argv[4];

    stworz_histogram(tryb);

    free(tablica_pliku_wejsciowego);
    return 0;
}