#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char** argv) {
    if (argc < 4) {
        printf("Podaj argumenty: <sciezka potoku> <sciezka do pliku do odczytu> <N>\n");
        return 1;
    }

    FILE* plik = fopen(argv[1], "r");   // otwórz plik z argumentu

    char* bufor = (char*)calloc(1000, sizeof(char)); // stwórz bufor
    FILE* wejscie_sortowania = popen("sort", "w");  // stwórz potok

    while( fgets(bufor, 1000, plik) != NULL ){   // dopóki coś czytamy z pliku z argumentu

        fputs(bufor, wejscie_sortowania);       //wstaw bufor do potoku

    }
    pclose(wejscie_sortowania);     // zamknij potok -> niech rozpocznie pracować
    free(bufor);
    return 0;
}
