#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char** argv) {
    if (argc < 4) {
        printf("Podaj argumenty: <sciezka potoku> <sciezka do pliku do zapisu> <N>\n");
        exit(1);
    }

    char* sciezka_do_potoku_nazwanego = argv[1];
    char* sciezka_do_pliku_tekstowego = argv[2];
    int N = (int) strtol(argv[3], (char**) NULL, 10);

    FILE* potok_nazwany = fopen(sciezka_do_potoku_nazwanego, "r");
    FILE* plik = fopen(sciezka_do_pliku_tekstowego, "w+");
    if(potok_nazwany == NULL || plik == NULL) exit(2);
    //fwrite("yo", sizeof(char), 2, plik);

    char* bufor = (char*)calloc(N + 4, sizeof(char));       // nie wiem czemu to +4 , może usuń

    while( fread(bufor, sizeof(char), N, potok_nazwany) > 0 ){   // dopóki coś czytamy z potoku
        fwrite(bufor, sizeof(char), N, plik);       //wstaw bufor do pliku
    }
    fclose(potok_nazwany);
    fclose(plik);
    free(bufor);
    return 0;
}
