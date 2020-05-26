#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/file.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/times.h>
#include <unistd.h>

struct macierz{
    int kolumny;
    int wiersze;
    int szerokosc_komorki;      // potrzebne tylko przy zapisie     // zakladam ze wartosci sa z int, wiec z -2147483648 do 2147483647
    char* sciezka_do_pliku;
}; typedef struct macierz macierz;

macierz* init_macierz(int kolumny, int wiersze, int szerokosc_komorki, char* sciezka_do_pliku){
    macierz* temp = (macierz*) malloc(sizeof(macierz));
    temp->kolumny = kolumny;
    temp->wiersze = wiersze;
    temp->szerokosc_komorki = szerokosc_komorki;
    temp->sciezka_do_pliku = calloc(1024, sizeof(char));
    strcpy(temp->sciezka_do_pliku, sciezka_do_pliku);
    return temp;
}

struct plik_lista{
    macierz** macierze_A;
    macierz** macierze_B;
    macierz** macierze_C;
    int ile_macierzy;
}; typedef struct plik_lista plik_lista ;

plik_lista* init_plik_lista(int size){
    plik_lista* temp = (plik_lista*) malloc(sizeof(plik_lista));
    temp->ile_macierzy = size;
    temp->macierze_A = (macierz**) calloc(size, sizeof(macierz*));
    temp->macierze_B = (macierz**) calloc(size, sizeof(macierz*));
    temp->macierze_C = (macierz**) calloc(size, sizeof(macierz*));
    return temp;
}

int ile_kolumn(char* plik){
    FILE* temp = fopen(plik, "r");
    char* pojedynczy_wiersz = calloc(1048576, sizeof(char));
    fgets(pojedynczy_wiersz, 1048576, temp);

    strtok(pojedynczy_wiersz, " ");
    int kolumny = 1;    // jako ze da sie cos przeczytac

    while( strtok(NULL, " ") != NULL ) kolumny++;
    free(pojedynczy_wiersz);
    fclose(temp);
    return kolumny;
}

int ile_wierszy(char* plik){
    FILE* temp = fopen(plik, "r");
    char* wiersz = calloc(1048576, sizeof(char));
    
    int wiersze = 0;
    while( fgets(wiersz, 1048576, temp) != NULL ) wiersze++;

    free(wiersz);
    fclose(temp);
    return wiersze;
}


macierz* stworz_macierz(char* nazwa_pliku, int kolumny, int wiersze){
    FILE* plik = fopen(nazwa_pliku, "w+");
    macierz* temp = init_macierz(kolumny, wiersze, 16, nazwa_pliku);
    for(int y=0; y<temp->kolumny * 16; y++ ){
        for(int x=0; x<temp->wiersze; x++){
            fwrite(" ", 1, 1, plik);
        }
        fwrite("\n", 1, 1, plik);
    }
    fclose(plik);
    return temp;
}

void usun_macierz(macierz* macierz){
    free(macierz->sciezka_do_pliku);
    free(macierz);
}


plik_lista* czytaj_plik_lista(char* nazwa_pliku){
    FILE* plik = fopen(nazwa_pliku, "r");

    char* temp = calloc(3 * 1024, sizeof(char));
    int ile_macierzy = 0;
    while( fgets(temp, 3 * 1024, plik) != NULL ){
        ile_macierzy = ile_macierzy + 1;
    }

    plik_lista* temp_lista = init_plik_lista(ile_macierzy);
    rewind(plik);

    int iterator = 0;
    while( fgets(temp, 3 * 1024, plik) != NULL ){
        char* plik_A = strtok(temp, " ");
        char* plik_B = strtok(NULL, " ");
        char* plik_C = strtok(NULL, "\n");

        macierz* macierz_A = malloc(sizeof(macierz));
        macierz_A->sciezka_do_pliku = calloc(1024, sizeof(char));
        strcpy(macierz_A->sciezka_do_pliku, plik_A);
        macierz_A->kolumny = ile_kolumn(plik_A);
        macierz_A->wiersze = ile_wierszy(plik_A);
        temp_lista->macierze_A[iterator] = macierz_A;

        macierz* macierz_B = malloc(sizeof(macierz));
        macierz_B->sciezka_do_pliku = calloc(1024, sizeof(char));
        strcpy(macierz_B->sciezka_do_pliku, plik_B);
        macierz_B->kolumny = ile_kolumn(plik_B);
        macierz_B->wiersze = ile_wierszy(plik_B);
        temp_lista->macierze_B[iterator] = macierz_B;

        if(macierz_A->kolumny != macierz_B->wiersze){
            printf("Macierzy nie da sie pomnozyc.");
            exit(2);
        }

        macierz* macierz_C = stworz_macierz(plik_C, macierz_B->kolumny, macierz_A->wiersze);
        temp_lista->macierze_C[iterator] = macierz_C;

        iterator = iterator + 1;
    }
    free(temp);
    fclose(plik);
    return temp_lista;
}

void usun_liste(plik_lista* lista){
    for(int i = 0; i< lista->ile_macierzy; i++){
        usun_macierz(lista->macierze_A[i]);
        usun_macierz(lista->macierze_B[i]);
        usun_macierz(lista->macierze_C[i]);
    }

    free(lista->macierze_A);
    free(lista->macierze_B);
    free(lista->macierze_C);
    free(lista);
}

int czytaj_wartosc(macierz* macierz, FILE* plik, int wiersz, int kolumna){
    rewind(plik);
    int i = 0;
    char* temp_linia = calloc(1048576, sizeof(char));
    while( fgets(temp_linia, 1048576, plik) != NULL  &&  i < wiersz  &&  i < macierz->wiersze ) i++;    // wybranie odpowiedniej linijki z pliku
    
    
    int j = 0;
    char* wartosc;
    wartosc = strtok(temp_linia, " ");
    while(wartosc != NULL && j < kolumna && j < macierz->kolumny){
        wartosc = strtok(NULL, " ");            // wybranie odpowiedniego wyrazu z linijki
        j++;
    }


    int wartosc_liczbowa = (int) strtol(wartosc, (char**) NULL, 10);
    free(temp_linia);
    return wartosc_liczbowa;
}

void zapisz_wartosc(macierz* macierz, FILE* plik, int wiersz, int kolumna, int wynik){
    rewind(plik);

    int index = macierz->szerokosc_komorki * macierz->kolumny * wiersz + macierz->szerokosc_komorki * kolumna;

    char* wynik_tekst = (char*) calloc(macierz->szerokosc_komorki, sizeof(char));
    sprintf(wynik_tekst, "%d", wynik);      // zapisanie wartosci liczbowej, jako "tablicy" char'ow

    int j = macierz->szerokosc_komorki-1;
    while(wynik_tekst[j] == 0) wynik_tekst[j--] = ' ';

    fseek(plik, index, 0);      // ustawienie wskaznika na odpowiednie miejsce
    fwrite(wynik_tekst, sizeof(char), macierz->szerokosc_komorki - 1, plik);
    free(wynik_tekst);
};

int main(int argc, char** argv) {
    if (argc < 5) {
        printf("Podaj:  find <plik z lista> <ilosc podprocesow> <ilosc sekund na podproces> <tryb: 0-oddzielne plik / 1-wszystkie w jednym pliku>\n");
        return 1;
    }

    char *sciezka_lista = argv[1];
    char *sciezka_lista_bezwzgledna = realpath(sciezka_lista, NULL);
    int ilosc_podprocesow = (int) strtol(argv[2], (char **) NULL, 10);
    int limit_czasowy = (int) strtol(argv[3], (char**) NULL, 10);
    int tryb = (int) strtol(argv[4], (char**) NULL, 10);

    plik_lista* lista = czytaj_plik_lista(sciezka_lista);
    
    pid_t* procesy_potomne = calloc(ilosc_podprocesow, sizeof(pid_t));
    
    for(int k=0; k<ilosc_podprocesow; k++){     // zmiana programu -> wczesniej sadzilem, ze program musi wykonac swoja prace
                                                // teraz zakladam, ze skoro program ma stworzyc tylko n procesow, a kazdy proces po k sekundach ma skonczyc prace
                                                // to przy podanej zbyt malej ilosci czasu na wejsciu programy moga nie byc skonczone
        pid_t dziecko = fork();
        
        if(dziecko == 0){
            int wykonane_obliczenia = 0;    // ilosc obliczonych czesci macierzy (czytaj ilosc par plikow na ktorych skonczone roboty)

            struct tms* czas_start = malloc(sizeof(struct tms));
            struct tms* czas_stop = malloc(sizeof(struct tms));
            double t_start=times(czas_start);       // rozpoczete odliczanie


            for(int i=0; i<lista->ile_macierzy; i++){
                macierz* macierz_a = lista->macierze_A[i];
                macierz* macierz_b = lista->macierze_B[i];
                macierz* macierz_c = lista->macierze_C[i];

                int szerokosc = macierz_b->kolumny;
                int wysokosc = macierz_a->wiersze;

                float stosunek_kolumn_do_procesow = (float) ( (float) szerokosc / (float) ilosc_podprocesow );
                
                int startowa_kolumna = (int) ( stosunek_kolumn_do_procesow * (float) k );           // takie rozmieszczenie gwarantuje mi, że mnożenie zostanie podzielone równo, a także, że program będzie działał, jeśli ilość kolumn jest mniejsza od ilości procesów
                int koncowa_kolumna = (int) ( stosunek_kolumn_do_procesow * (float) (k + 1) ) - 1 ;
                if(koncowa_kolumna < startowa_kolumna){     // jeśli jest więcej procesów niż kolumn do policzenia
                    printf("My job here is done. (nie zostaly mi przypisane zadne kolumny)\n");
                    //exit(-222);
                    continue;       // przejdz do nastepnych plikow -> moze tam bedziesz przydatny
                }
                
                FILE* plik_A = fopen(lista->macierze_A[i]->sciezka_do_pliku, "r");
                FILE* plik_B = fopen(lista->macierze_B[i]->sciezka_do_pliku, "r");
                FILE* plik_C = fopen(lista->macierze_C[i]->sciezka_do_pliku, "r+");
                
                for(int kolumna = startowa_kolumna; kolumna <= koncowa_kolumna; kolumna++){

                    FILE* plik_output;

                    if(tryb == 0){
                        char* nazwa_pliku_czesciowego = malloc(1024 * sizeof(char));
                        strcat(nazwa_pliku_czesciowego, "temp__plik_");

                        char* temp = malloc(12 * sizeof(char));
                        sprintf(temp, "%d", i);         // numer pary z pliku
                        strcat(nazwa_pliku_czesciowego, temp);

                        strcat(nazwa_pliku_czesciowego, "__kolumna_");

                        char* temp2 = malloc(12 * sizeof(char));
                        sprintf(temp2, "%d", kolumna);         // numer pary z pliku
                        strcat(nazwa_pliku_czesciowego, temp2);

                        plik_output = fopen(nazwa_pliku_czesciowego, "w");    // stworz plik i go otworz

                        free(nazwa_pliku_czesciowego);
                        free(temp);
                        free(temp2);
                    }

                    for(int wiersz = 0; wiersz < wysokosc; wiersz++){
                        int wynik = 0;
                        
                        for(int iterator=0; iterator<macierz_b->wiersze; iterator++){   // wysokosc macierzy B / szerokosc macierzy A
                            wynik += czytaj_wartosc(macierz_a, plik_A, iterator, kolumna) * czytaj_wartosc(macierz_b, plik_B, wiersz, iterator);
                        }

                        if(tryb == 1){
                            flock(fileno(plik_C), LOCK_EX);
                            zapisz_wartosc(macierz_c, plik_C, wiersz, kolumna, wynik);
                            flock(fileno(plik_C), LOCK_UN);
                        }
                        else if (tryb == 0){
                            flock(fileno(plik_output), LOCK_EX);
                            zapisz_wartosc(macierz_c, plik_output, wiersz, 0, wynik);
                            flock(fileno(plik_output), LOCK_UN);
                        }

                        wykonane_obliczenia++;
                    }

                    if(tryb == 0){
                        fclose(plik_output);
                    }

                }

                fclose(plik_A);
                fclose(plik_B);
                fclose(plik_C);
                
                double t_stop = times(czas_stop);
                double t_miniony= (t_stop - t_start)/sysconf(_SC_CLK_TCK);
                if((int) t_miniony >= limit_czasowy){
                    printf("Gotta go\n");
                    break;
                }
            }

            free(czas_start);
            free(czas_stop);
            exit(wykonane_obliczenia);
        }
        else{   // sekcja rodzica
            procesy_potomne[k] = dziecko;
        }
    }

    int* status = malloc(sizeof(int));
    for(int i = 0; i<ilosc_podprocesow; i++){
        waitpid(procesy_potomne[i], status, 0);
        printf("Proces %d wykonal %d mnozen macierzy\n", procesy_potomne[i], WEXITSTATUS(*status));
    }

    if(tryb == 0){  // sumowanie plikow

        for(int i=0; i<lista->ile_macierzy; i++){
            pid_t dziecko = fork();

            if(dziecko == 0){
                char* argumenty = malloc(1048576 * sizeof(char));

                char* nazwa_pliku_czesciowego = malloc(1024 * sizeof(char));
                strcat(nazwa_pliku_czesciowego, "temp__plik_");

                char* temp = malloc(12 * sizeof(char));
                sprintf(temp, "%d", i);         // numer pary z pliku
                strcat(nazwa_pliku_czesciowego, temp);

                strcat(nazwa_pliku_czesciowego, "__kolumna_");

                for(int kolumna=0; kolumna<lista->macierze_C[i]->kolumny; kolumna++){
                    char* temp2 = malloc(12 * sizeof(char));
                    sprintf(temp2, "%d", kolumna);         // numer pary z pliku

                    char* temp3 = malloc(1024 * sizeof(char));
                    strcpy(temp3, nazwa_pliku_czesciowego);
                    strcat(temp3, temp2);

                    strcat(argumenty, temp3);
                    strcat(argumenty, " ");

                    free(temp2);
                    free(temp3);
                }
                free(nazwa_pliku_czesciowego);
                free(temp);

                execvp("paste", argumenty);
                free(argumenty);
            }
            else {
                wait(0);
            }
        }
    }

    usun_liste(lista);
    free(sciezka_lista_bezwzgledna);
    return 0;
}