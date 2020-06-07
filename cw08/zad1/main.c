#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char** argv){
    if (argc<5){
        printf("Zbyt malo argumentow. Podaj <liczba_watkow> <sign / block / interleaved> <plik wejsciowy> <plik wyjsciowy>\n");
        return 1;
    }
    int liczba_watkow = (int) strtol(argv[1], (char**)NULL, 10);

    return 0;
}