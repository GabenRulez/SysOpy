#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/times.h>
#include <unistd.h>
#include <dlfcn.h>
#include "myLibrary.h"

void wypisz_czasy(double rzeczywisty, double uzytkownika, double systemowy){
    printf("Rzeczywisty: %f Uzytkownika: %f Systemowy: %f\n", rzeczywisty, uzytkownika, systemowy);
}

double roznica_czasow(clock_t start, clock_t stop){
    return ((double)(stop - start) / sysconf(_SC_CLK_TCK));
}


int main(int argc, char** argv){
    if (argc < 3){
        printf("Podaj:   liczbę par   komendę   <plik1> <plik2>\n");
        return 1;
    }
    
    
    void* dl_handle = dlopen(".myLibrary.so", RTLD_LAZY);

    struct tablica_glowna* (*dl_stworz_tablice_glowna)(int);
    FILE* (*dl_porownaj_pliki)(char*, char*);
    int (*dl_utworz_blok_operacji_z_pliku)(FILE*, struct tablica_glowna*);
    void (*dl_usun_blok)(int, struct tablica_glowna*);
    void (*dl_usun_operacje_z_bloku)(int, struct blok_operacji_edycyjnych*);
    
    dl_stworz_tablice_glowna = (struct tablica_glowna* (*)(int))dlsym(dl_handle, "stworz_tablice_glowna");
    dl_porownaj_pliki = (FILE* (*)(char*, char*))dlsym(dl_handle, "porownaj_pliki");
    dl_utworz_blok_operacji_z_pliku = (int (*)(FILE*, struct tablica_glowna*))dlsym(dl_handle, "utworz_blok_operacji_z_pliku");
    dl_usun_blok = (void (*)(int, struct tablica_glowna*))dlsym(dl_handle, "usun_blok");
    dl_usun_operacje_z_bloku = (void (*)(int, struct blok_operacji_edycyjnych*))dlsym(dl_handle, "usun_operacje_z_bloku");
    



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
            tablica = dl_stworz_tablice_glowna(rozmiar);
            i = i + 2;
        }
        else if( strcmp(komenda, "compare_pairs") == 0 ){
            for(int j=1; j<=ilosc_par_plikow; j++){
                char* plik_A = strtok(argv[i+j], ":");
                char* plik_B = strtok(NULL, ":");

                FILE* plik_temp = dl_porownaj_pliki(plik_A, plik_B);
                dl_utworz_blok_operacji_z_pliku(plik_temp, tablica);
            }
            i = i + ilosc_par_plikow + 1;   // przeskocz przez wszystkie pary plików do nastepnej komendy
        }
        else if( strcmp(komenda, "remove_block") == 0 ){
            int numer_bloku = (int) strtol(argv[i+1], (char**)NULL, 10);
            dl_usun_blok(numer_bloku, tablica);
            i = i + 2;
        }
        else if( strcmp(komenda, "remove_operation") == 0 ){
            int numer_bloku     = (int) strtol(argv[i+1], (char**)NULL, 10);
            int numer_operacji  = (int) strtol(argv[i+2], (char**)NULL, 10);

            struct blok_operacji_edycyjnych* blok = tablica->bloki[numer_bloku];
            dl_usun_operacje_z_bloku(numer_operacji, blok);
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
    dlclose(dl_handle);
}