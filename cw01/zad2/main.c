#include"myLibrary.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/times.h>
#include <unistd.h>

void wypisz_czasy(double rzeczywisty, double uzytkownika, double systemowy){
    printf("Rzeczywisty: %f Uzytkownika: %f Systemowy: %f", rzeczywisty, uzytkownika, systemowy);
}

double roznica_czasow(clock_t start, clock_t stop){
    return ((double)(stop - start) / sysconf(_SC_CLK_TCK));
}


int main(int argc, char** argv){
    if (argc < 3){
        printf("Podaj:   liczbę par   komendę   <plik1> <plik2>\n");
        return 1;
    }



    struct tms* start_bufor = calloc(1, sizeof(struct tms));
    struct tms* stop_bufor =  calloc(1, sizeof(struct tms));

    clock_t czas_na_start = 0;
    clock_t czas_na_stop = 0;

    czas_na_start = times(start_bufor);



    int ilosc_par_plikow = (int) strtol(argv[1], (char**)NULL, 10);

    struct tablica_glowna* tablica;

    int i = 2;  // indeks pierwszej komendy
    while(i < argc){
        char* komenda = argv[i];
        if(komenda == NULL) return 2;

        if( strcmp(komenda, "create_table") == 0 ){
            int rozmiar = (int) strtol(argv[i+1], (char**)NULL, 10);
            tablica = stworz_tablice_glowna(rozmiar);
            i = i + 2;
        }
        else if( strcmp(komenda, "compare_pairs") == 0 ){

            for(int j=1; j<=ilosc_par_plikow; j++){
                char* para = argv[i+j];
                char* plik_A = strtok(para, ":");
                char* plik_B = strtok(NULL, ":");

                FILE* plik_temp = porownaj_pliki(plik_A, plik_B);
                utworz_blok_operacji_z_pliku(plik_temp, tablica);
            }
            i = i + ilosc_par_plikow + 1;   // przeskocz przez wszystkie pary plików do nastepnej komendy
        }
        else if( strcmp(komenda, "remove_block") == 0 ){
            int numer_bloku = (int) strtol(argv[i+1], (char**)NULL, 10);
            usun_blok(numer_bloku, tablica);
            i = i + 2;
        }
        else if( strcmp(komenda, "remove_operation") == 0 ){
            int numer_bloku     = (int) strtol(argv[i+1], (char**)NULL, 10);
            int numer_operacji  = (int) strtol(argv[i+2], (char**)NULL, 10);

            struct blok_operacji_edycyjnych* blok = tablica->bloki[numer_bloku];
            usun_operacje_z_bloku(numer_operacji, blok);
            i = i + 3;
        }
        else{
            printf("Error - nie istnieje komenda: '");
            printf("%s", komenda);
            printf("'\nSprawdz, czy poprawnie wprowadziles argumenty.\n");
            return 3;
        }
    }



    czas_na_stop = times(stop_bufor);
    wypisz_czasy( roznica_czasow(czas_na_start, czas_na_stop), roznica_czasow(start_bufor->tms_cutime, stop_bufor->tms_cutime), roznica_czasow(start_bufor->tms_cstime, stop_bufor->tms_cstime) );
}