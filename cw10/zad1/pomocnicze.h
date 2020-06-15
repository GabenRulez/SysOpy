#ifndef POMOCNICZE_H
#define POMOCNICZE_H

#include <sys/time.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <errno.h>
#include <endian.h>
#include <sys/types.h>
#include <string.h>
#include <unistd.h>
#include <sys/un.h>
#include <signal.h>
#include <pthread.h>
#include <poll.h>
#include <time.h>
#include <netdb.h>
#include "funkcje_tekstowe.h"

void wyjscie_z_bledem(char* komunikat) {
    wypisz_linie_gwiazdek();
    printf("\nError: %s\n", komunikat);
    printf("Errno: %d\n\n", errno);
    wypisz_linie_gwiazdek();
    exit(EXIT_FAILURE);
}



#define MAX_KLIENTOW 10
#define MAX_DLUGOSC_TEKSTU 32
#define CZAS_SPANIA 3

typedef enum ZNAK{
    KOLKO = 0,
    KRZYZYK = 1,
    BRAK = 7
} ZNAK;

typedef struct klient {
    int deskryptor;
    char nazwa[MAX_DLUGOSC_TEKSTU];
    int ticks;
    ZNAK znak;
} klient;

typedef enum TYP_WIADOMOSCI{
    PING,
    LOGIN_REQUEST,
    LOGIN_APPROVED,
    LOGIN_REJECTED,
    GAME_WAIT,
    GAME_MOVE,
    GAME_FOUND,
    GAME_FINISHED,
    LOGOUT
} TYP_WIADOMOSCI;

typedef struct wiadomosc{
    char tresc[MAX_DLUGOSC_TEKSTU - 1];
    TYP_WIADOMOSCI typ;
} wiadomosc;

wiadomosc* wczytaj_wiadomosc(int deskryptor_gniazda){
    wiadomosc* temp = (wiadomosc*) malloc(sizeof(wiadomosc));
    char* temp_string = (char*) calloc(MAX_DLUGOSC_TEKSTU, sizeof(char));

    if( read(deskryptor_gniazda, (void*) temp_string, MAX_DLUGOSC_TEKSTU) < 0) wyjscie_z_bledem("Nieudane czytanie z gniazda.");

    sscanf(temp_string, "%d:%[^:]", (int*) &temp->typ, (char*) &temp->tresc);   // wczytaj typ i tresc wiadomosci, z ":" pomiedzy nimi

    free(temp_string);
    return temp;
}

void wyslij_wiadomosc(int deskryptor_gniazda, TYP_WIADOMOSCI typ, char* tresc){
    char* temp_string = (char*)calloc(MAX_DLUGOSC_TEKSTU, sizeof(char));
    sprintf(temp_string, "%d:%s", (int) typ, tresc);
    if( write(deskryptor_gniazda, (void*) temp_string, MAX_DLUGOSC_TEKSTU) < 0 ) wyjscie_z_bledem("Nieudany zapis do gniazda.");
    free(temp_string);
}


typedef struct gra{
    int gracz_1;
    int gracz_2;
    ZNAK plansza[9];
} gra;


wiadomosc* wczytaj_wiadomosc_nieblokujaco(int deskryptor_gniazda){
    wiadomosc* temp = (wiadomosc*) malloc(sizeof(wiadomosc));
    char* temp_string = (char*) calloc(MAX_DLUGOSC_TEKSTU, sizeof(char));

    if( recv(deskryptor_gniazda, (void*) temp_string, MAX_DLUGOSC_TEKSTU, MSG_DONTWAIT) < 0) return NULL;

    sscanf(temp_string, "%d:%[^:]", (int*) &temp->typ, (char*) &temp->tresc);   // wczytaj typ i tresc wiadomosci, z ":" pomiedzy nimi

    free(temp_string);
    return temp;
}




int losowy_int(int min, int max){
    return min + rand() % (max - min + 1);
}

char* obecny_timestamp(){
    struct timeval tv;
    gettimeofday(&tv, NULL);

    struct tm *ts = gmtime(&tv.tv_sec);
    char* godzinka = (char*)calloc(80, sizeof(char));
    char* temp = (char*)calloc(4, sizeof(char));
    if(ts->tm_hour >= 10){
        sprintf(temp, "%d:", ts->tm_hour);
        strcat(godzinka, temp);
    }
    else{
        sprintf(temp, "0%d:", ts->tm_hour);
        strcat(godzinka, temp);
    }

    if(ts->tm_min >= 10){
        sprintf(temp, "%d:", ts->tm_min);
        strcat(godzinka, temp);
    }
    else{
        sprintf(temp, "0%d:", ts->tm_min);
        strcat(godzinka, temp);
    }

    if(ts->tm_sec >= 10){
        sprintf(temp, "%d.", ts->tm_sec);
        strcat(godzinka, temp);
    }
    else{
        sprintf(temp, "0%d:", ts->tm_sec);
        strcat(godzinka, temp);
    }

    if(tv.tv_usec/1000 >= 100){
        sprintf(temp, "%ld", tv.tv_usec/1000);
        strcat(godzinka, temp);
    }
    else if(tv.tv_usec/1000 >= 10){
        sprintf(temp, "0%ld", tv.tv_usec/1000);
        strcat(godzinka, temp);
    }
    else{
        sprintf(temp, "00%ld", tv.tv_usec/1000);
        strcat(godzinka, temp);
    }

    free(temp);
    return godzinka;
}

int sufit(int licznik, int mianownik){
    return licznik/mianownik + (licznik % mianownik != 0);
}



#endif //POMOCNICZE_H



