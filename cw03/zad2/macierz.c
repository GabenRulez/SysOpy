#include <stdio.h>
#include <stdlib.h>

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


    if( tryb == 0 ){    //oddzielne pliki

    }
    else if (tryb == 1){    // wszystkie do tego samego pliku

    }
    else{
        printf("Nie ma takiego trybu.");
        return 1;
    }


    free(sciezka_lista_bezwzgledna);
    return 0;
}