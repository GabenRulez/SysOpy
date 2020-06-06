#include <stdio.h>
#include <stdlib.h>
#include <sys/msg.h>
#include <sys/ipc.h>
#include <errno.h>
#include <stdbool.h>
#include <string.h>
#include <time.h>
#include <signal.h>

#include "config.h"

key_t klucz_kolejki;
int id_kolejki;
int id_kolejki_serwera;
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
    msg_buf* komunikat = (msg_buf*)malloc(sizeof(msg_buf));
    komunikat->m_type = STOP;
    komunikat->ID_klienta = id_klienta;

    if(msgsnd(id_kolejki_serwera, komunikat, ROZMIAR_BUFORA, 0) < 0) wyjscie_z_bledem("Nie udalo sie wyslac komunikatu.");
    msgctl(id_kolejki, IPC_RMID, NULL);

    wypisz_wysrodkowane("--- Koncze dzialanie ---");
    exit(EXIT_SUCCESS);
}



int polacz_z_serwerem() {
    wypisz_wysrodkowane("Laczenie");

    msg_buf* komunikat = (msg_buf*)malloc(sizeof(msg_buf));
    komunikat->m_type = INIT;
    komunikat->klucz_kolejki = klucz_kolejki;
    if( msgsnd(id_kolejki_serwera, komunikat, ROZMIAR_BUFORA, 0) < 0 ) wyjscie_z_bledem("Nie udalo sie wyslac komunikatu.");

    msg_buf* komunikat_otrzymany = (msg_buf*)malloc(sizeof(msg_buf));
    if( msgrcv(id_kolejki, komunikat_otrzymany, ROZMIAR_BUFORA, 0, 0) < 0 ) wyjscie_z_bledem("Nie moge otrzymac zadnych komunikatow.");

    int id_klienta = komunikat_otrzymany->m_type;
    wypisz_wysrodkowane("Polaczono");
    return id_klienta;
}



void tryb_chatu(int id_drugiego_klienta, int id_kolejki_drugiego_klienta) {
    char* komenda = NULL;
    size_t len = 0;
    ssize_t odczyt = 0;
    msg_buf* komunikat = (msg_buf*)malloc(sizeof(msg_buf));
    while(1) {
        printf("Podaj tekst lub Q: ");
        odczyt = getline(&komenda, &len, stdin);
        komenda[odczyt - 1] = '\0';

        if(msgrcv(id_kolejki, komunikat, ROZMIAR_BUFORA, STOP, IPC_NOWAIT) >= 0) {
            wypisz_wysrodkowane("--- Odebrano polecenie STOP (z serwera) ---");
            komenda_STOP();
        }

        if(msgrcv(id_kolejki, komunikat, ROZMIAR_BUFORA, DISCONNECT, IPC_NOWAIT) >= 0) {
            wypisz_wysrodkowane("--- Odebrano polecenie DISCONNECT (z serwera) ---");
            break;
        }

        int flag_przed = false;
        int flag_po = false;
        while(msgrcv(id_kolejki, komunikat, ROZMIAR_BUFORA, 0, IPC_NOWAIT) >= 0) {
            if (!flag_przed) {
                printf("\n");
                flag_przed = true;
            }
            printf("Klient %d: %s\n", id_drugiego_klienta, komunikat->m_text);
            flag_po = true;
        }
        if (flag_po) printf("\n");

        if(strcmp(komenda, "Q") == 0) {
            komunikat->m_type = DISCONNECT;
            komunikat->ID_klienta = id_klienta;
            komunikat->ID_polaczonego_klienta = id_drugiego_klienta;
            if( msgsnd(id_kolejki_serwera, komunikat, ROZMIAR_BUFORA, 0) < 0 ) wyjscie_z_bledem("Nie udalo sie wyslac komunikatu.");
            break;

        } else if(strcmp(komenda, "") != 0) {
            komunikat->m_type = CONNECT;
            strcpy(komunikat->m_text, komenda);
            if(msgsnd(id_kolejki_drugiego_klienta, komunikat, ROZMIAR_BUFORA, 0) < 0) wyjscie_z_bledem("Nie udalo sie wyslac komunikatu.");
        }
    }
}


void komenda_CONNECT(int id) {
    msg_buf* komunikat = (msg_buf*)malloc(sizeof(msg_buf));
    komunikat->m_type = CONNECT;
    komunikat->ID_klienta = id_klienta;
    komunikat->ID_polaczonego_klienta = id;
    if(msgsnd(id_kolejki_serwera, komunikat, ROZMIAR_BUFORA, 0) < 0) wyjscie_z_bledem("Nie udalo sie wyslac komunikatu.");

    msg_buf* komunikat_otrzymany = (msg_buf*)malloc(sizeof(msg_buf));
    if(msgrcv(id_kolejki, komunikat_otrzymany, ROZMIAR_BUFORA, 0, 0) < 0) wyjscie_z_bledem("Nie moge otrzymac zadnych komunikatow.");

    key_t klucz_drugiego_klienta = komunikat_otrzymany->klucz_kolejki;
    int id_kolejki_drugiego_klienta = msgget(klucz_drugiego_klienta, 0);
    if(id_kolejki_drugiego_klienta < 0) wyjscie_z_bledem("Nie mozna otworzyc kolejki klienta.");

    tryb_chatu(id, id_kolejki_drugiego_klienta);
}

void komenda_LIST() {
    msg_buf* komunikat = (msg_buf*)malloc(sizeof(msg_buf));
    komunikat->m_type = LIST;
    komunikat->ID_klienta = id_klienta;
    if( msgsnd(id_kolejki_serwera, komunikat, ROZMIAR_BUFORA, 0) < 0 ) wyjscie_z_bledem("Nie udalo sie wyslac komunikatu.");

    msg_buf* komunikat_otrzymany = (msg_buf*)malloc(sizeof(msg_buf));
    if( msgrcv(id_kolejki, komunikat_otrzymany, ROZMIAR_BUFORA, 0, 0) < 0 ) wyjscie_z_bledem("Nie moge otrzymac zadnych komunikatow.");
    printf("%s\n", komunikat_otrzymany->m_text);
}

void funkcja_wyjscia(int signum) {
    printf("\n");
    komenda_STOP();
}

void odbierz_komunikat_z_serwera() {
    msg_buf* komunikat = (msg_buf*)malloc(sizeof(msg_buf));
    bool flaga = false;

    if( msgrcv(id_kolejki, komunikat, ROZMIAR_BUFORA, 0, IPC_NOWAIT) >= 0 ) {
        if(komunikat->m_type == STOP) {
            wypisz_wysrodkowane("--- Odebrano polecenie STOP (z serwera) ---");
            flaga = true;
            komenda_STOP();

        } else if(komunikat->m_type == CONNECT) {
            printf("Laczenie z klientem %d...\n", komunikat->ID_klienta);
            int id_drugiego_klienta = msgget(komunikat->klucz_kolejki, 0);
            if(id_drugiego_klienta < 1) wyjscie_z_bledem("Nie mozna otworzyc kolejki drugiego klienta.");
            tryb_chatu(komunikat->ID_klienta, id_drugiego_klienta);
            flaga = true;
        }
    }
    if (flaga) odbierz_komunikat_z_serwera();
}


int main() {
    srand(time(NULL));


    klucz_kolejki = ftok(getenv("HOME"), rand() % 253 + ID_SERWERA + 1);
    id_kolejki = msgget(klucz_kolejki, IPC_CREAT | 0666);

    wypisz_linie_gwiazdek();
    wypisz_wysrodkowane("Uruchamiam klienta.");

    printf("\nKlucz kolejki klienta: %d\n", klucz_kolejki);
    printf("ID kolejki klienta: %d\n", id_kolejki);


    key_t klucz_kolejki_serwera = ftok(getenv("HOME"), ID_SERWERA);
    id_kolejki_serwera = msgget(klucz_kolejki_serwera, 0);

    if(id_kolejki_serwera < 0) wyjscie_z_bledem("Brak dostepu do kolejki serwera.");
    printf("ID kolejki serwera: %d\n\n", id_kolejki_serwera);


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