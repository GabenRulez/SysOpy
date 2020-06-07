#ifndef POMOCNICZE_H
#define POMOCNICZE_H

#include <sys/time.h>
#include <time.h>

#define SZEROKOSC_KONSOLI 80
#define MAX_ZAMOWIEN 5

struct tablica_zamowien{
    int wartosci[MAX_ZAMOWIEN];
}; typedef struct tablica_zamowien tablica_zamowien;


const char* spacja_1 = " ";
const char* spacja_2 = "  ";
const char* spacja_4 = "    ";
const char* spacja_8 = "        ";
const char* spacja_16 = "                ";
const char* spacja_32 = "                                ";
const char* spacja_64 = "                                                                ";
const char* gwiazdka_1 = "*";
const char* gwiazdka_2 = "**";
const char* gwiazdka_4 = "****";
const char* gwiazdka_8 = "********";
const char* gwiazdka_16 = "****************";
const char* gwiazdka_32 = "********************************";
const char* gwiazdka_64 = "****************************************************************";



void wypisz_wysrodkowane(char* wiadomosc){
    int dlugosc_napisu = (int) strlen(wiadomosc);
    if (dlugosc_napisu < SZEROKOSC_KONSOLI){
        int spacje_po_lewej = (int)(SZEROKOSC_KONSOLI - dlugosc_napisu) / 2;
        int spacje_po_prawej = SZEROKOSC_KONSOLI - dlugosc_napisu - spacje_po_lewej;
        char nowa_wiadomosc[SZEROKOSC_KONSOLI] = "";

        int i = spacje_po_lewej;
        while(i>0){
            if(i>=64){
                strcat(nowa_wiadomosc, spacja_64);
                i -= 64;
            }
            else if(i>=32){
                strcat(nowa_wiadomosc, spacja_32);
                i -= 32;
            }
            else if(i>=16){
                strcat(nowa_wiadomosc, spacja_16);
                i -= 16;
            }
            else if(i>=8){
                strcat(nowa_wiadomosc, spacja_8);
                i -= 8;
            }
            else if(i>=4){
                strcat(nowa_wiadomosc, spacja_4);
                i -= 4;
            }
            else if(i>=2){
                strcat(nowa_wiadomosc, spacja_2);
                i -= 2;
            }
            else if(i>=1){
                strcat(nowa_wiadomosc, spacja_1);
                i -= 1;
            }
        }

        strcat(nowa_wiadomosc, wiadomosc);

        i = spacje_po_prawej;
        while(i>0){
            if(i>=64){
                strcat(nowa_wiadomosc, spacja_64);
                i -= 64;
            }
            else if(i>=32){
                strcat(nowa_wiadomosc, spacja_32);
                i -= 32;
            }
            else if(i>=16){
                strcat(nowa_wiadomosc, spacja_16);
                i -= 16;
            }
            else if(i>=8){
                strcat(nowa_wiadomosc, spacja_8);
                i -= 8;
            }
            else if(i>=4){
                strcat(nowa_wiadomosc, spacja_4);
                i -= 4;
            }
            else if(i>=2){
                strcat(nowa_wiadomosc, spacja_2);
                i -= 2;
            }
            else if(i>=1){
                strcat(nowa_wiadomosc, spacja_1);
                i -= 1;
            }
        }

        printf("%s", nowa_wiadomosc);
        printf("\n");
    }
}

void wypisz_linie_gwiazdek(){
    char nowa_wiadomosc[SZEROKOSC_KONSOLI] = "";

    int i = SZEROKOSC_KONSOLI;
    while(i>0){
        if(i>=64){
            strcat(nowa_wiadomosc, gwiazdka_64);
            i -= 64;
        }
        else if(i>=32){
            strcat(nowa_wiadomosc, gwiazdka_32);
            i -= 32;
        }
        else if(i>=16){
            strcat(nowa_wiadomosc, gwiazdka_16);
            i -= 16;
        }
        else if(i>=8){
            strcat(nowa_wiadomosc, gwiazdka_8);
            i -= 8;
        }
        else if(i>=4){
            strcat(nowa_wiadomosc, gwiazdka_4);
            i -= 4;
        }
        else if(i>=2){
            strcat(nowa_wiadomosc, gwiazdka_2);
            i -= 2;
        }
        else if(i>=1){
            strcat(nowa_wiadomosc, gwiazdka_1);
            i -= 1;
        }
    }
    printf("%s", nowa_wiadomosc);
    printf("\n");
}

int losowy_int(int min, int max){
    return min + rand() % (max - min);
}

char* obecny_timestamp(){
    struct timeval tv;
    gettimeofday(&tv, NULL);

    struct tm *ts = gmtime(&tv.tv_sec);
    char* godzinka = (char*)calloc(SZEROKOSC_KONSOLI, sizeof(char));
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

const char* SEMAFORY[6] = {"/SEM0", "/SEM1", "/SEM2", "/SEM3", "/SEM4", "/SEM5"};
const char* PAMIEC_WSPOLNA = "/PAMIECWSPOLNA";


#endif //POMOCNICZE_H



