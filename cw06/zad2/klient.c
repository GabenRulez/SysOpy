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

char queue_name[NAME_LEN];
mqd_t deskryptor_kolejki;
mqd_t deskryptor_kolejki_serwera;
int id_klienta;

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
    char* komunikat = (char*)calloc(MAX_MSG_LEN, sizeof(char));
    komunikat[0] = id_klienta;

    if(mq_send(deskryptor_kolejki_serwera, komunikat, MAX_MSG_LEN, STOP) < 0) wyjscie_z_bledem("Nie udalo sie wyslac komunikatu.");
    if( mq_close(deskryptor_kolejki_serwera) < 0 ) wyjscie_z_bledem("Nie mozna zamknac kolejki.");

    wypisz_wysrodkowane("--- Koncze dzialanie ---");
    exit(EXIT_SUCCESS);
}



int polacz_z_serwerem() {
    wypisz_wysrodkowane("Laczenie");

    char* komunikat = (char*)calloc(MAX_MSG_LEN, sizeof(char));
    strcpy(komunikat, queue_name);
    if(mq_send(deskryptor_kolejki_serwera, komunikat, MAX_MSG_LEN, INIT) < 0 ) wyjscie_z_bledem("Nie udalo sie wyslac komunikatu.");

    unsigned int id_klienta;
    if(mq_receive(deskryptor_kolejki, komunikat, MAX_MSG_LEN, &id_klienta) < 0 ) wyjscie_z_bledem("Nie moge otrzymac zadnych komunikatow.");
    printf("ID received %d\n", id_klienta);

    wypisz_wysrodkowane("Polaczono");
    return id_klienta;
}



void tryb_chatu(int id_drugiego_klienta, mqd_t deskryptor_drugiego_klienta) {
    char* komenda = NULL;
    size_t len = 0;
    ssize_t odczyt = 0;
    char* komunikat = (char*)calloc(MAX_MSG_LEN, sizeof(char));
    while(1) {
        printf("Podaj tekst lub Q: ");
        odczyt = getline(&komenda, &len, stdin);
        komenda[odczyt - 1] = '\0';

        struct timespec* tspec = (struct timespec*)malloc(sizeof(struct timespec));
        unsigned int type;
        bool disconnect = false;

        int flag_przed = false;
        int flag_po = false;
        while(mq_timedreceive(deskryptor_kolejki, komunikat, MAX_MSG_LEN, &type, tspec) >= 0){
            if(type==STOP){
                wypisz_wysrodkowane("--- Odebrano polecenie STOP (z serwera) ---");
                komenda_STOP();
            }
            else if(type == DISCONNECT){
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
            if(mq_send(deskryptor_kolejki_serwera, komunikat, MAX_MSG_LEN, DISCONNECT) < 0 ) wyjscie_z_bledem("Nie udalo sie wyslac komunikatu.");
            break;

        } else if(strcmp(komenda, "") != 0) {
            strcpy(komunikat, komenda);
            if(mq_send(deskryptor_drugiego_klienta, komunikat, MAX_MSG_LEN, CONNECT) < 0) wyjscie_z_bledem("Nie udalo sie wyslac komunikatu.");
        }
    }
}


void komenda_CONNECT(int id) {
    char* komunikat = (char*)calloc(MAX_MSG_LEN, sizeof(char));

    komunikat[0] = id_klienta;
    komunikat[1] = id;
    if(mq_send(deskryptor_kolejki_serwera, komunikat, MAX_MSG_LEN, CONNECT) < 0) wyjscie_z_bledem("Nie udalo sie wyslac komunikatu.");

    if(mq_receive(deskryptor_kolejki, komunikat, MAX_MSG_LEN, NULL) < 0) wyjscie_z_bledem("Nie moge otrzymac zadnych komunikatow.");

    char* klucz_drugiego_klienta = (char*)calloc(NAME_LEN, sizeof(char));//komunikat_otrzymany->klucz_kolejki;
    strncpy(klucz_drugiego_klienta, komunikat + 1, strlen(komunikat)-1);
    printf("other name %s,\n", klucz_drugiego_klienta);

    mqd_t deskryptor_kolejki_drugiego_klienta = mq_open(klucz_drugiego_klienta, O_RDWR);
    if(deskryptor_kolejki_drugiego_klienta < 0) wyjscie_z_bledem("Nie mozna otworzyc kolejki klienta.");

    tryb_chatu(id, deskryptor_kolejki_drugiego_klienta);
}

void komenda_LIST() {
    char* komunikat = (char*)calloc(MAX_MSG_LEN, sizeof(char));
    komunikat[0] = id_klienta;

    if(mq_send(deskryptor_kolejki_serwera, komunikat, MAX_MSG_LEN, LIST) < 0 ) wyjscie_z_bledem("Nie udalo sie wyslac komunikatu.");

    if(mq_receive(deskryptor_kolejki, komunikat, MAX_MSG_LEN, NULL) < 0 ) wyjscie_z_bledem("Nie moge otrzymac zadnych komunikatow.");
    printf("%s\n", komunikat);
}

void funkcja_wyjscia(int signum) {
    printf("\n");
    komenda_STOP();
}

void odbierz_komunikat_z_serwera() {
    char* komunikat = (char*)calloc(MAX_MSG_LEN, sizeof(char));
    bool flaga = false;

    struct timespec* tspec = (struct timespec*)malloc(sizeof(struct timespec));
    unsigned int type;

    if(mq_timedreceive(deskryptor_kolejki, komunikat, MAX_MSG_LEN, &type, tspec) >= 0 ) {
        if(type == STOP) {
            wypisz_wysrodkowane("--- Odebrano polecenie STOP (z serwera) ---");
            flaga = true;
            komenda_STOP();

        } else if(type == CONNECT) {
            printf("Laczenie z klientem ...\n");

            char* other_queue_name = (char*)calloc(NAME_LEN, sizeof(char));
            strncpy(other_queue_name, komunikat+1, strlen(komunikat)-1);
            printf("other name %s",other_queue_name);
            mqd_t deskryptor_kolejki_drugiego_klienta = mq_open(other_queue_name, O_RDWR);
            if(deskryptor_kolejki_drugiego_klienta < 0) wyjscie_z_bledem("Nie mozna otworzyc kolejki drugiego klienta.");
            tryb_chatu((int)komunikat[0], deskryptor_kolejki_drugiego_klienta);
            flaga = true;
        }
    }
    if (flaga) odbierz_komunikat_z_serwera();
}


int main() {
    srand(time(NULL));

    queue_name[0] = '/';
    for(int i=1; i<NAME_LEN; i++) queue_name[i] = ( rand() % ('Z' - 'A' + 1) + 'A' );
    deskryptor_kolejki = mq_open(queue_name, O_RDWR | O_CREAT, S_IRWXU | S_IRWXG | S_IRWXO, NULL);

    wypisz_linie_gwiazdek();
    wypisz_wysrodkowane("Uruchamiam klienta.");

    printf("\nNazwa kolejki klienta: %d\n", queue_name);
    printf("Deskryptor kolejki klienta: %d\n", deskryptor_kolejki);


    deskryptor_kolejki_serwera = mq_open(SERVER_QUEUE_NAME, O_RDWR);
    if(deskryptor_kolejki_serwera < 0) wyjscie_z_bledem("Brak dostepu do kolejki serwera.");
    printf("Deskryptor kolejki serwera: %d\n\n", deskryptor_kolejki_serwera);


    id_klienta = polacz_z_serwerem();
    printf("Otrzymane ID: %d\n", id_klienta);
    wypisz_wysrodkowane("Dostepne komendy: LIST, CONNECT, STOP");

    signal(SIGINT, funkcja_wyjscia);



    char* komenda = NULL;
    size_t len = 0;
    ssize_t read = 0;
    while(1) {
        printf("Podaj polecenie: ");
        read = getline(&komenda, &len, stdin);      // można się pokusić na użycie fork() - podproces zajmuje sie IO, a glowny sprawdza, czy cos nie nadeszlo
        komenda[read - 1] = '\0';

        odbierz_komunikat_z_serwera();

        if(strcmp(komenda, "") == 0) continue;

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

    return 0;
}