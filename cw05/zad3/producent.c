#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

int random_int(int minimum, int maksimum){
    srand(time(NULL));
    return minimum + rand()%(maksimum - minimum);
}

int main(int argc, char** argv) {
    if (argc < 4) {
        printf("Podaj argumenty: <sciezka potoku> <sciezka do pliku do odczytu> <N>\n");
        exit(EXIT_FAILURE);
    }

    char* sciezka_do_potoku_nazwanego = argv[1];
    char* sciezka_do_pliku_tekstowego = argv[2];
    int N = (int) strtol(argv[3], (char**) NULL, 10);

    FILE* potok_nazwany = fopen(sciezka_do_potoku_nazwanego, "w");
    FILE* plik = fopen(sciezka_do_pliku_tekstowego, "r");
    if(potok_nazwany == NULL || plik == NULL) exit(2);


    char* bufor = (char*)calloc(N, sizeof(char));

    char przedrostek[9]; // "#" + liczba z zasięgu[0, 4194303] + "#"
    sprintf(przedrostek, "#%d#", getpid());

    while( fread(bufor, sizeof(char), N, plik) > 0 ){   // dopóki coś czytamy z pliku
        sleep( random_int( 1, 5 ) );
        char* temp_linijka = (char*)calloc(9 + N, sizeof(char));

        strcpy(temp_linijka, przedrostek);
        strcat(temp_linijka, bufor);

        fwrite(temp_linijka, sizeof(char), strlen(temp_linijka), potok_nazwany);       //wstaw linijke do potoku
        free(temp_linijka);
    }
    fclose(potok_nazwany);
    fclose(plik);
    free(bufor);
    return 0;
}
