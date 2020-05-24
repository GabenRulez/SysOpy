#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct macierz{
    int kolumny;
    int wiersze;
    int szerokosc_komorki;
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
    macierz* temp = init_macierz(kolumny, wiersze, 1024, nazwa_pliku);
    for(int y=0; y<temp->kolumny; y++ ){
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
            return 2;
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

    int *proc_per_m = NULL;
    if( tryb == 0 ){    //oddzielne pliki
        proc_per_m = count_proc_per_m(m_list, proc_count);
        create_empty_files(m_list, proc_per_m);
    }
    
    pid_t* child_pids = calloc(proc_count, sizeof(pid_t));
    for(int i = 0; i<proc_count; i++){
        pid_t child_pid = fork();
        if(child_pid == 0){
            int num_of_fragments;
            multiply(m_list, i, proc_count, max_time, mode, &num_of_fragments);
            exit(num_of_fragments);
        }
        else if (child_pid>0){
            child_pids[i] = child_pid;
        }
        else{
            exit(EXIT_FAILURE);
        }
    }


    int* stat_loc = malloc(sizeof(int));
    for(int i = 0; i<proc_count; i++){
        waitpid(child_pids[i], stat_loc, 0);
        printf("Process of PID %d multiplied %d fragment(s) of matrices\n", child_pids[i], WEXITSTATUS(*stat_loc));
    }

    if(tryb == 0) exec_paste(m_list, proc_per_m, proc_count);



    usun_liste(lista);
    free(sciezka_lista_bezwzgledna);
    return 0;
}