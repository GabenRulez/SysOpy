#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/times.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>


void wypisz_czasy(double rzeczywisty, double uzytkownika, double systemowy){
    printf("Rzeczywisty: %f Uzytkownika: %f Systemowy: %f\n", rzeczywisty, uzytkownika, systemowy);
}

double roznica_czasow(clock_t start, clock_t stop){
    return ((double)(stop - start) / sysconf(_SC_CLK_TCK));
}



void zamien_miejscami_systemowo(int plik, int indeks1, int indeks2, int wielkosc_elementu){
    char* temp1 = (char*) malloc(wielkosc_elementu * sizeof(char));
    char* temp2 = (char*) malloc(wielkosc_elementu * sizeof(char));

    lseek(  plik, indeks1 * wielkosc_elementu, SEEK_SET);
    read(   plik, &temp1,   wielkosc_elementu);

    lseek(  plik, indeks2 * wielkosc_elementu, SEEK_SET);
    read(   plik, &temp2,   wielkosc_elementu);


    lseek(  plik, indeks1 * wielkosc_elementu, SEEK_SET);
    write(  plik, &temp2,    wielkosc_elementu);

    lseek(  plik, indeks2 * wielkosc_elementu, SEEK_SET);
    write(  plik, &temp1,    wielkosc_elementu);
}

void quicksort_systemowo(int plik, int l, int r, int wielkosc_elementu){
    if (l<r){
        int punkt_podzialu = l;     // tak powiedziane w poleceniu
        char* wartosc_punktu_podzialu = malloc(wielkosc_elementu * sizeof(char));

        lseek(  plik, punkt_podzialu * wielkosc_elementu, SEEK_SET);
        read(   plik, &wartosc_punktu_podzialu, wielkosc_elementu);

        zamien_miejscami_systemowo(plik, punkt_podzialu, r, wielkosc_elementu);
/*

        int aktualna_pozycja = l;

        for (int i=1; i < r-1; i++){
            char* wartosc_porownywana = malloc(wielkosc_elementu * sizeof(char));
            lseek(  plik, i * wielkosc_elementu, SEEK_SET);
            read(   plik, &wartosc_porownywana, wielkosc_elementu);

            if( strcmp(wartosc_porownywana, wartosc_punktu_podzialu) < 0 ){ // czytaj: jeśli w porządku leksykograficznym tekst1 jest przed tekst2
                zamien_miejscami_systemowo(plik, i, aktualna_pozycja, wielkosc_elementu);
                aktualna_pozycja++;
            }
            //free(wartosc_porownywana);
        }
        //free(wartosc_punktu_podzialu);

        zamien_miejscami_systemowo(plik, aktualna_pozycja, r, wielkosc_elementu);

        quicksort_systemowo(plik, l, aktualna_pozycja-1, wielkosc_elementu);
        quicksort_systemowo(plik, aktualna_pozycja+1, r, wielkosc_elementu);*/
    }
}

int main(int argc, char** argv){
    if (argc < 3){
        printf("Podaj:  komendę   <plik1> <plik2>\n");
        return 1;
    }

    struct tms* start_bufor = calloc(1, sizeof(struct tms));
    struct tms* stop_bufor =  calloc(1, sizeof(struct tms));

    clock_t czas_na_start = 0;
    clock_t czas_na_stop = 0;

    czas_na_start = times(start_bufor);

    char* komenda = argv[1];
    if(komenda == NULL) return 2;

    if( strcmp(komenda, "generate") == 0 ){
        if (argc != 5){
            printf("Do funkcji 'generate' podaj:  generate   <nazwa_pliku>  <ilosc rekordow>    <ilosc_znakow>\n");
            return 1;
        }
        char* sciezka = argv[2];
        int ilosc_elementow = (int) strtol(argv[3], (char**)NULL, 10);
        int wielkosc_elementu = (int) strtol(argv[4], (char**)NULL, 10);

        char* bufor = (char*)malloc(wielkosc_elementu * sizeof(char));  // tworze bufor o wielkosci jednego elementu

        FILE* plik = fopen(sciezka, "w");
        for (int i=0; i<ilosc_elementow; i++){
            for (int i=0; i<wielkosc_elementu; i++){    // wypelniam bufor losowymi znakami
                bufor[i] = "ABCDEFGHIJKLMNOPRSTUWYZabcdefghijklmnoprstuwyz"[random() % 46];
            }
            fwrite(bufor, wielkosc_elementu, 1, plik);  // zapis jednego elementu do pliku
        }
    }

    else if( strcmp(komenda, "sort") == 0 ){
        if (argc != 6){
            printf("Do funkcji 'sort' podaj:  sort   <nazwa_pliku>  <ilosc rekordow>    <ilosc_znakow>  <sys/lib>\n");
            return 1;
        }
        char* sciezka = argv[2];
        int ilosc_elementow = (int) strtol(argv[3], (char**)NULL, 10);
        int wielkosc_elementu = (int) strtol(argv[4], (char**)NULL, 10);

        if( strcmp(argv[5], "sys") == 0 ){
            int plik = open(sciezka, O_RDWR);
            if( plik<0 ){
                printf("Blad przy czytaniu pliku ze sciezki %s.\nUpewnij sie, ze istnieje taka sciezka", sciezka);
                return 4;   // brak pliku
            }



            //quicksort_systemowo(plik, 1, ilosc_elementow-1, wielkosc_elementu);
            zamien_miejscami_systemowo(plik, 0, 1, wielkosc_elementu);



            close(plik);
        }
        else if( strcmp(argv[5], "lib") == 0 ){

        }
        else{
            printf("Nie istnieje tryb: %s", argv[5]);
            return 3;
        }
        /*

        for(int j=1; j<=ilosc_par_plikow; j++){
            char* plik_A = strtok(argv[i+j], ":");
            char* plik_B = strtok(NULL, ":");

            FILE* plik_temp = porownaj_pliki(plik_A, plik_B);
            utworz_blok_operacji_z_pliku(plik_temp, tablica);
        }
        i = i + ilosc_par_plikow + 1;   // przeskocz przez wszystkie pary plików do nastepnej komendy*/
    }
    /*else if( strcmp(komenda, "copy") == 0 ){
        int numer_bloku = (int) strtol(argv[i+1], (char**)NULL, 10);
        usun_blok(numer_bloku, tablica);
        i = i + 2;
    }
    else{
        printf("Error - nie istnieje komenda: '");
        printf("%s", komenda);
        printf("'\nSprawdz, czy poprawnie wprowadziles argumenty.\n");
        return 3;
    }
*/


    czas_na_stop = times(stop_bufor);
    wypisz_czasy( roznica_czasow(czas_na_start, czas_na_stop), roznica_czasow(start_bufor->tms_cutime, stop_bufor->tms_cutime), roznica_czasow(start_bufor->tms_cstime, stop_bufor->tms_cstime) );
}