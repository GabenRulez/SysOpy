#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/times.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <time.h>

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
    read(   plik, temp1,   wielkosc_elementu);

    lseek(  plik, indeks2 * wielkosc_elementu, SEEK_SET);
    read(   plik, temp2,   wielkosc_elementu);


    lseek(  plik, indeks1 * wielkosc_elementu, SEEK_SET);
    write(  plik, temp2,    wielkosc_elementu);

    lseek(  plik, indeks2 * wielkosc_elementu, SEEK_SET);
    write(  plik, temp1,    wielkosc_elementu);

    free(temp1);
    free(temp2);
}

int quicksort_systemowo(int plik, int l, int r, int wielkosc_elementu){
    if (l<r){
        int punkt_podzialu = l;     // tak powiedziane w poleceniu
        char* wartosc_punktu_podzialu = (char*) malloc(wielkosc_elementu * sizeof(char));

        lseek(  plik, punkt_podzialu * wielkosc_elementu, SEEK_SET);
        read(   plik, wartosc_punktu_podzialu, wielkosc_elementu);

        zamien_miejscami_systemowo(plik, punkt_podzialu, r, wielkosc_elementu);


        int aktualna_pozycja = l;

        for (int i=l; i < r; i++){
            char* wartosc_porownywana = (char*) malloc(wielkosc_elementu * sizeof(char));
            lseek(  plik, i * wielkosc_elementu, SEEK_SET);
            read(   plik, wartosc_porownywana, wielkosc_elementu);

            if( strcmp(wartosc_porownywana, wartosc_punktu_podzialu) < 0 ){ // czytaj: jeśli w porządku leksykograficznym tekst1 jest przed tekst2
                zamien_miejscami_systemowo(plik, i, aktualna_pozycja, wielkosc_elementu);
                aktualna_pozycja++;
            }
            free(wartosc_porownywana);
        }
        free(wartosc_punktu_podzialu);

        zamien_miejscami_systemowo(plik, aktualna_pozycja, r, wielkosc_elementu);

        quicksort_systemowo(plik, l, aktualna_pozycja-1, wielkosc_elementu);
        quicksort_systemowo(plik, aktualna_pozycja+1, r, wielkosc_elementu);
    }
    return 0;
}


void zamien_miejscami_bibliotecznie(FILE* plik, int indeks1, int indeks2, int wielkosc_elementu){
    char* temp1 = (char*) malloc(wielkosc_elementu * sizeof(char));
    char* temp2 = (char*) malloc(wielkosc_elementu * sizeof(char));

    fseek(  plik, indeks1 * wielkosc_elementu, 0);
    fread(  temp1, sizeof(char), wielkosc_elementu, plik);

    fseek(  plik, indeks2 * wielkosc_elementu, 0);
    fread(  temp2, sizeof(char), wielkosc_elementu, plik);


    fseek(  plik, indeks1 * wielkosc_elementu, 0);
    fwrite( temp2, sizeof(char), wielkosc_elementu, plik);

    fseek(  plik, indeks2 * wielkosc_elementu, 0);
    fwrite( temp1, sizeof(char), wielkosc_elementu, plik);

    free(temp1);
    free(temp2);
}

int quicksort_bibliotecznie(FILE* plik, int l, int r, int wielkosc_elementu){
    if (l<r){
        int punkt_podzialu = l;     // tak powiedziane w poleceniu
        char* wartosc_punktu_podzialu = (char*) malloc(wielkosc_elementu * sizeof(char));

        fseek(  plik, punkt_podzialu * wielkosc_elementu, 0);
        fread(  wartosc_punktu_podzialu, sizeof(char), wielkosc_elementu, plik);

        zamien_miejscami_bibliotecznie(plik, punkt_podzialu, r, wielkosc_elementu);


        int aktualna_pozycja = l;

        for (int i=l; i < r; i++){
            char* wartosc_porownywana = (char*) malloc(wielkosc_elementu * sizeof(char));
            fseek(  plik, i * wielkosc_elementu, 0);
            fread(  wartosc_porownywana, sizeof(char), wielkosc_elementu, plik);

            if( strcmp(wartosc_porownywana, wartosc_punktu_podzialu) < 0 ){ // czytaj: jeśli w porządku leksykograficznym tekst1 jest przed tekst2
                zamien_miejscami_bibliotecznie(plik, i, aktualna_pozycja, wielkosc_elementu);
                aktualna_pozycja++;
            }
            free(wartosc_porownywana);
        }
        free(wartosc_punktu_podzialu);

        zamien_miejscami_bibliotecznie(plik, aktualna_pozycja, r, wielkosc_elementu);

        quicksort_bibliotecznie(plik, l, aktualna_pozycja-1, wielkosc_elementu);
        quicksort_bibliotecznie(plik, aktualna_pozycja+1, r, wielkosc_elementu);
    }
    return 0;
}





int main(int argc, char** argv){
    if (argc < 3){
        printf("Podaj:  komendę   <plik1> <plik2>\n");
        return 1;
    }

    struct tms* start_bufor = malloc(sizeof(struct tms));
    struct tms* stop_bufor =  malloc(sizeof(struct tms));

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
        srand(time(0));
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

            if( quicksort_systemowo(plik, 0, ilosc_elementow-1, wielkosc_elementu) != 0){
                return 7; // error w quicksort
            };

            close(plik);
        }

        else if( strcmp(argv[5], "lib") == 0 ){
            FILE* plik = fopen(sciezka, "r+");
            if( plik == NULL ){
                printf("Blad przy czytaniu pliku ze sciezki %s.\nUpewnij sie, ze istnieje taka sciezka", sciezka);
                return 4;   // brak pliku
            }

            if( quicksort_bibliotecznie(plik, 0, ilosc_elementow-1, wielkosc_elementu) != 0){
                return 7; // error w quicksort
            }

            fclose(plik);
        }
        else{
            printf("Nie istnieje tryb: %s", argv[5]);
            return 3;
        }
    }



    else if( strcmp(komenda, "copy") == 0 ){
        if (argc != 7){
            printf("Do funkcji 'copy' podaj:  copy   <nazwa_pliku1> <nazwa_pliku2> <ilosc rekordow>    <ilosc_znakow>  <sys/lib>\n");
            return 1;
        }
        char* sciezka1 = argv[2];
        char* sciezka2 = argv[3];
        int ilosc_elementow = (int) strtol(argv[4], (char**)NULL, 10);
        int wielkosc_elementu = (int) strtol(argv[5], (char**)NULL, 10);

        if( strcmp(argv[6], "sys") == 0 ){
            int plik1 = open(sciezka1, O_RDONLY);
            if( plik1<0 ){
                printf("Blad przy czytaniu pliku ze sciezki %s.\nUpewnij sie, ze istnieje taka sciezka", sciezka1);
                return 4;   // brak pliku
            }
            lseek(plik1, 0, SEEK_SET);

            int plik2 = open(sciezka2, O_RDWR|O_CREAT|O_TRUNC, 0644);
            if( plik2<0 ){
                printf("Blad przy otwieraniu pliku ze sciezki %s.\nUpewnij sie, ze istnieje taka sciezka", sciezka2);
                return 4;
            }
            lseek(plik2, 0, SEEK_SET);

            char* bufor = (char*) malloc(wielkosc_elementu * sizeof(char));
            for(int i=0; i<ilosc_elementow; i++){
                if( read(plik1, bufor, wielkosc_elementu) != wielkosc_elementu ){
                    printf("W: Blad przy czytaniu wartosci z pliku %s. Prawdopodobnie podales bledne dane wejsciowe.", sciezka1);
                    //return 6;
                }
                if( write(plik2, bufor, wielkosc_elementu) != wielkosc_elementu ){
                    printf("W: Blad przy zapisywaniu wartosci do pliku %s.", sciezka2);
                }
            }
            free(bufor);
            close(plik1);
            close(plik2);
        }


        else if( strcmp(argv[6], "lib") == 0 ){
            FILE* plik1 = fopen(sciezka1, "r");
            if( plik1 == NULL ){
                printf("Blad przy czytaniu pliku ze sciezki %s.\nUpewnij sie, ze istnieje taka sciezka", sciezka1);
                return 4;   // brak pliku
            }

            FILE* plik2 = fopen(sciezka2, "w");
            if( plik2 == NULL ){
                printf("Blad przy otwieraniu pliku ze sciezki %s.\nUpewnij sie, ze istnieje taka sciezka", sciezka2);
                return 4;
            }

            char* bufor = (char*) malloc(wielkosc_elementu * sizeof(char));

            for(int i=0; i<ilosc_elementow; i++){
                if( fread(bufor, sizeof(char), wielkosc_elementu, plik1) != wielkosc_elementu ){
                    printf("W: Blad przy czytaniu wartosci z pliku %s. Prawdopodobnie podales bledne dane wejsciowe.", sciezka1);
                    //return 6;
                }
                if( fwrite(bufor, sizeof(char), wielkosc_elementu, plik2) != wielkosc_elementu ){
                    printf("W: Blad przy zapisywaniu wartosci do pliku %s.", sciezka2);
                }
            }
            free(bufor);
            fclose(plik1);
            fclose(plik2);
        }



        else{
            printf("Nie istnieje tryb: %s", argv[5]);
            return 3;
        }

    }
    else{
        printf("Error - nie istnieje komenda: '");
        printf("%s", komenda);
        printf("'\nSprawdz, czy poprawnie wprowadziles argumenty.\n");
        return 3;
    }

    czas_na_stop = times(stop_bufor);
    wypisz_czasy( roznica_czasow(czas_na_start, czas_na_stop), roznica_czasow(start_bufor->tms_cutime, stop_bufor->tms_cutime), roznica_czasow(start_bufor->tms_cstime, stop_bufor->tms_cstime) );
}