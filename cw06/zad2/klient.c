#include <stdio.h>
#include <stdlib.h>
#include <sys/msg.h>
#include <sys/ipc.h>
#include <errno.h>
#include <stdbool.h>
#include <string.h>
#include <time.h>
#include <signal.h>
#include <sys/stat.h>
#include <mqueue.h>

#include "config.h"

char nazwa_kolejki[NAME_LEN];
mqd_t deskryptor_kolejki;
mqd_t deskryptor_kolejki_serwera;
unsigned int id_klienta;

void wypisz_wysrodkowane(char* wiadomosc){
    int dlugosc_napisu = (int) strlen(wiadomosc);
    if (dlugosc_napisu < SZEROKOSC_KONSOLI){
        int spacje_po_lewej = (int)(SZEROKOSC_KONSOLI - dlugosc_napisu) / 2;
        int spacje_po_prawej = SZEROKOSC_KONSOLI - dlugosc_napisu - spacje_po_lewej;
        char nowa_wiadomosc[SZEROKOSC_KONSOLI] = "";

        int i = spacje_po_lewej;
        while(i>0){
            if(i>=16){
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
            if(i>=16){
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



void wyjscie_z_bledem(char* komunikat) {
    wypisz_linie_gwiazdek();
    printf("\nError: %s\n", komunikat);
    printf("Errno: %d\n\n", errno);
    wypisz_linie_gwiazdek();
    exit(EXIT_FAILURE);
}

void komenda_STOP() {
    char* komunikat = (char*)calloc(MAX_DLUGOSC_KOMUNIKATU, sizeof(char));
    komunikat[0] = id_klienta;

    if(mq_send(deskryptor_kolejki_serwera, komunikat, MAX_DLUGOSC_KOMUNIKATU, STOP) < 0) wyjscie_z_bledem("Nie udalo sie wyslac komunikatu.");
    if( mq_close(deskryptor_kolejki_serwera) < 0 ) wyjscie_z_bledem("Nie mozna zamknac kolejki.");
    if(mq_unlink(nazwa_kolejki) < 0 ) wyjscie_z_bledem("Nie moge usunac kolejki.");

    wypisz_wysrodkowane("--- Koncze dzialanie ---");
    exit(EXIT_SUCCESS);
}


void polacz_z_serwerem() {
    wypisz_wysrodkowane("Laczenie");

    char* komunikat = (char*)calloc(MAX_DLUGOSC_KOMUNIKATU, sizeof(char));
    strcpy(komunikat, nazwa_kolejki);
    if(mq_send(deskryptor_kolejki_serwera, komunikat, MAX_DLUGOSC_KOMUNIKATU, INIT) < 0 ) wyjscie_z_bledem("Nie udalo sie wyslac komunikatu.");

    char* komunikat_odebrany = (char*)calloc(MAX_DLUGOSC_KOMUNIKATU + 1, sizeof(char));
    if(mq_receive(deskryptor_kolejki, komunikat_odebrany, MAX_DLUGOSC_KOMUNIKATU + 1, &id_klienta) < 0 ) wyjscie_z_bledem("Nie moge otrzymac zadnych komunikatow.");

    wypisz_wysrodkowane("Polaczono");
}


void tryb_chatu(int id_drugiego_klienta, mqd_t deskryptor_drugiego_klienta) {
    char* komenda = NULL;
    size_t len = 0;
    ssize_t odczyt = 0;
    char* komunikat = (char*)calloc(MAX_DLUGOSC_KOMUNIKATU, sizeof(char));
    while(1) {
        printf("Podaj tekst lub Q: ");
        odczyt = getline(&komenda, &len, stdin);
        komenda[odczyt - 1] = '\0';

        struct timespec* tspec = (struct timespec*)malloc(sizeof(struct timespec));
        unsigned int temp_priorytet;
        bool disconnect = false;

        int flag_przed = false;
        int flag_po = false;
        while(mq_timedreceive(deskryptor_kolejki, komunikat, MAX_DLUGOSC_KOMUNIKATU, &temp_priorytet, tspec) >= 0){
            if(temp_priorytet == STOP){
                wypisz_wysrodkowane("--- Odebrano polecenie STOP (z serwera) ---");
                komenda_STOP();
            }
            else if(temp_priorytet == DISCONNECT){
                wypisz_wysrodkowane("--- Odebrano polecenie DISCONNECT (z serwera) ---");
                disconnect = true;
                break;
            }
            else{
                if (!flag_przed) {
                    printf("\n");
                    flag_przed = true;
                }
                printf("Klient %d: %s\n", id_drugiego_klienta, komunikat);
                flag_po = true;
            }
        }
        if (flag_po) printf("\n");
        if(disconnect) break;

        if(strcmp(komenda, "Q") == 0) {
            komunikat[0] = id_klienta;
            komunikat[1] = id_drugiego_klienta;
            if(mq_send(deskryptor_kolejki_serwera, komunikat, MAX_DLUGOSC_KOMUNIKATU, DISCONNECT) < 0 ) wyjscie_z_bledem("Nie udalo sie wyslac komunikatu.");
            break;

        } else if(strcmp(komenda, "") != 0) {
            strcpy(komunikat, komenda);
            if(mq_send(deskryptor_drugiego_klienta, komunikat, MAX_DLUGOSC_KOMUNIKATU, CONNECT) < 0) wyjscie_z_bledem("Nie udalo sie wyslac komunikatu.");
        }
    }
}


void komenda_CONNECT(int id) {
    char* komunikat = (char*)calloc(MAX_DLUGOSC_KOMUNIKATU, sizeof(char));

    komunikat[0] = id_klienta;
    komunikat[1] = id;
    if(mq_send(deskryptor_kolejki_serwera, komunikat, MAX_DLUGOSC_KOMUNIKATU, CONNECT) < 0) wyjscie_z_bledem("Nie udalo sie wyslac komunikatu.");

    if(mq_receive(deskryptor_kolejki, komunikat, MAX_DLUGOSC_KOMUNIKATU, NULL) < 0) wyjscie_z_bledem("Nie moge otrzymac zadnych komunikatow.");

    char* nazwa_drugiej_kolejki = (char*)calloc(NAME_LEN, sizeof(char));//komunikat_otrzymany->klucz_kolejki;
    strncpy(nazwa_drugiej_kolejki, komunikat + 1, strlen(komunikat) - 1);
    printf("Polaczono z klientem %s.\n\n", nazwa_drugiej_kolejki);

    mqd_t deskryptor_kolejki_drugiego_klienta = mq_open(nazwa_drugiej_kolejki, O_RDWR);
    if(deskryptor_kolejki_drugiego_klienta < 0) wyjscie_z_bledem("Nie mozna otworzyc kolejki klienta.");

    tryb_chatu(id, deskryptor_kolejki_drugiego_klienta);
}

void komenda_LIST() {
    char* komunikat = (char*)calloc(MAX_DLUGOSC_KOMUNIKATU, sizeof(char));
    komunikat[0] = id_klienta;

    if(mq_send(deskryptor_kolejki_serwera, komunikat, MAX_DLUGOSC_KOMUNIKATU, LIST) < 0 ) wyjscie_z_bledem("Nie udalo sie wyslac komunikatu.");

    if(mq_receive(deskryptor_kolejki, komunikat, MAX_DLUGOSC_KOMUNIKATU, NULL) < 0 ) wyjscie_z_bledem("Nie moge otrzymac zadnych komunikatow.");
    printf("%s\n", komunikat);
}

void funkcja_wyjscia(int signum) {
    printf("\n");
    komenda_STOP();
}

void odbierz_komunikat_z_serwera() {
    char* komunikat = (char*)calloc(MAX_DLUGOSC_KOMUNIKATU, sizeof(char));
    bool flaga = false;

    struct timespec* tspec = (struct timespec*)malloc(sizeof(struct timespec));
    unsigned int temp_priorytet;

    if(mq_timedreceive(deskryptor_kolejki, komunikat, MAX_DLUGOSC_KOMUNIKATU, &temp_priorytet, tspec) >= 0 ) {
        if(temp_priorytet == STOP) {
            wypisz_wysrodkowane("--- Odebrano polecenie STOP (z serwera) ---");
            flaga = true;
            komenda_STOP();

        } else if(temp_priorytet == CONNECT) {
            wypisz_wysrodkowane("Laczenie z klientem ...");

            char* nazwa_drugiej_kolejki = (char*)calloc(NAME_LEN, sizeof(char));
            strncpy(nazwa_drugiej_kolejki, komunikat + 1, strlen(komunikat) - 1);
            printf("Polaczono z klientem %s.\n\n", nazwa_drugiej_kolejki);

            mqd_t deskryptor_kolejki_drugiego_klienta = mq_open(nazwa_drugiej_kolejki, O_RDWR);
            if(deskryptor_kolejki_drugiego_klienta < 0) wyjscie_z_bledem("Nie mozna otworzyc kolejki drugiego klienta.");

            tryb_chatu((int)komunikat[0], deskryptor_kolejki_drugiego_klienta);
            flaga = true;
        }
    }
    if (flaga) odbierz_komunikat_z_serwera();
}


int main() {
    srand(time(NULL));

    nazwa_kolejki[0] = '/';
    for(int i=1; i<NAME_LEN; i++) nazwa_kolejki[i] = "ABCDEFGHIJKLMNOPRSTUWXYZabcdefghijklmnoprstuwxyz"[rand() % 48];

    struct mq_attr attr;
    attr.mq_maxmsg = 10;
    attr.mq_msgsize = MAX_DLUGOSC_KOMUNIKATU;

    deskryptor_kolejki = mq_open(nazwa_kolejki, O_RDWR | O_CREAT, 0666, &attr);
    if(deskryptor_kolejki < 0) wyjscie_z_bledem("Nie mozna otworzyc kolejki.");

    wypisz_linie_gwiazdek();
    wypisz_wysrodkowane("Uruchamiam klienta.");

    printf("\nNazwa kolejki klienta: %s\n", nazwa_kolejki);
    printf("Deskryptor kolejki klienta: %d\n", deskryptor_kolejki);


    deskryptor_kolejki_serwera = mq_open(NAZWA_KOLEJKI_SERWERA, O_RDWR);
    if(deskryptor_kolejki_serwera < 0) wyjscie_z_bledem("Brak dostepu do kolejki serwera.");
    printf("Deskryptor kolejki serwera: %d\n\n", deskryptor_kolejki_serwera);


    polacz_z_serwerem();
    printf("Otrzymane ID: %d\n", id_klienta);
    wypisz_wysrodkowane("Dostepne komendy: LIST, CONNECT, STOP");

    signal(SIGINT, funkcja_wyjscia);

    char* komenda = NULL;
    size_t len = 0;
    ssize_t odczyt = 0;
    while(1) {
        printf("Podaj polecenie: ");
        odczyt = getline(&komenda, &len, stdin);
        komenda[odczyt - 1] = '\0';

        odbierz_komunikat_z_serwera();

        if(strcmp(komenda, "") == 0) continue;
        printf("\n");
        char* temp_slowo = strtok(komenda, " ");
        if(strcmp(temp_slowo, "LIST") == 0) {
            wypisz_wysrodkowane("--- Polecenie LIST ---");
            komenda_LIST();

        } else if(strcmp(temp_slowo, "CONNECT") == 0) {
            wypisz_wysrodkowane("--- Polecenie CONNECT ---");
            temp_slowo = strtok(NULL, " ");
            int id = (int) strtol(temp_slowo, (char **) NULL, 10);
            komenda_CONNECT(id);

        } else if(strcmp(temp_slowo, "STOP") == 0) {
            wypisz_wysrodkowane("--- Polecenie STOP ---");
            komenda_STOP();

        } else {
            printf("Nie ma takiej komendy: %s\n", komenda);
            printf("Dostepne komendy: LIST, CONNECT, STOP");
        }
    }

}