#include <stdio.h>
#include <stdlib.h>
#include <sys/msg.h>
#include <sys/ipc.h>
#include <errno.h>
#include <stdbool.h>
#include <string.h>
#include <signal.h>
#include <sys/stat.h>
#include <mqueue.h>
#include <fcntl.h>

#include "config.h"

char* kolejki_klientow[MAX_KLIENTOW];
bool dostepni_klienci[MAX_KLIENTOW];
mqd_t deskryptor_kolejki_serwera;

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

void funkcja_wyjscia(int signum) {
    printf("\n");
    wypisz_linie_gwiazdek();
    wypisz_wysrodkowane("Rozpoczeto wylaczanie systemu.");
    char* komunikat = (char*)calloc(MAX_DLUGOSC_KOMUNIKATU, sizeof(char));


    for(int i = 0; i < MAX_KLIENTOW; i++) {

        if(kolejki_klientow[i] != NULL) {
            mqd_t deskryptor_kolejki_klienta = mq_open(kolejki_klientow[i], O_RDWR);
            if( mq_send(deskryptor_kolejki_klienta, komunikat, MAX_DLUGOSC_KOMUNIKATU, STOP) < 0) wyjscie_z_bledem("Nie moge wyslac komunikatu.");
            if( mq_receive(deskryptor_kolejki_serwera, komunikat, MAX_DLUGOSC_KOMUNIKATU, NULL) < 0) wyjscie_z_bledem("Nie moge otrzymac komunikatu.");
            if( mq_close(deskryptor_kolejki_klienta) < 0 ) wyjscie_z_bledem("Nie moge zamknac kolejki.");
        }
    }
    if( mq_close(deskryptor_kolejki_serwera) < 0 ) wyjscie_z_bledem("Nie moge zamknac kolejki.");
    if( mq_unlink(NAZWA_KOLEJKI_SERWERA) < 0 ) wyjscie_z_bledem("Nie moge usunac kolejki.");
    wypisz_linie_gwiazdek();
    exit(EXIT_SUCCESS);
}

int tworz_id_klienta() {
    int i = 0;
    while(i < MAX_KLIENTOW && kolejki_klientow[i] != NULL) i++;
    if (i < MAX_KLIENTOW) return i+1;       // podaj pierwsze wolne miejsce w statycznej tablicy
    else return -1;
}



void glowna_petla(char* komunikat, int priorytet) {
    char* komunikat_odpowiedz = (char*)calloc(MAX_DLUGOSC_KOMUNIKATU, sizeof(char));
    int id_klienta;
    int id_drugiego_klienta;
    mqd_t deskryptor_kolejki_klienta;

    switch(priorytet) {

        case INIT: ;
            wypisz_wysrodkowane("--- Odebrano zlecenie INIT ---");
            int id = tworz_id_klienta();
            if(id == -1){
                wypisz_wysrodkowane("--- Nie mozna utworzyc wiecej klientow. ---");
                return;
            }

            deskryptor_kolejki_klienta = mq_open(komunikat, O_RDWR);
            if(deskryptor_kolejki_klienta < 0) wyjscie_z_bledem("Nie mozna otworzyc kolejki klienta.");

            kolejki_klientow[id - 1] = (char*)calloc(NAME_LEN, sizeof(char));
            strcpy(kolejki_klientow[id-1], komunikat);
            dostepni_klienci[id - 1] = true;

            if(mq_send(deskryptor_kolejki_klienta, komunikat_odpowiedz, MAX_DLUGOSC_KOMUNIKATU, id) < 0 ) wyjscie_z_bledem("Nie udalo sie wyslac komunikatu.");
            if( mq_close(deskryptor_kolejki_klienta) < 0 ) wyjscie_z_bledem("Nie moge zamknac kolejki");
            break;


        case LIST:
            wypisz_wysrodkowane("--- Odebrano zlecenie LIST ---");

            bool flag = false;
            id_klienta = (int)komunikat[0];
            for(int i = 0; i < MAX_KLIENTOW; i++) {
                if(kolejki_klientow[i] != NULL && i+1 != id_klienta) {
                    char temp[SZEROKOSC_KONSOLI];
                    sprintf(temp, "Klient %d - %s\n", i+1, dostepni_klienci[i] ? "dostepny" : "zajety");
                    strcat(komunikat_odpowiedz, temp);
                    flag = true;
                }
            }


            if(!flag) strcat(komunikat_odpowiedz, "Brak innych klientow.\n");
            deskryptor_kolejki_klienta = mq_open(kolejki_klientow[id_klienta - 1], O_RDWR);
            if(deskryptor_kolejki_klienta < 0) wyjscie_z_bledem("Nie mozna otworzyc kolejki klienta.");

            if(mq_send(deskryptor_kolejki_klienta, komunikat_odpowiedz, MAX_DLUGOSC_KOMUNIKATU, LIST) < 0 ) wyjscie_z_bledem("Nie udalo sie wyslac komunikatu");
            if( mq_close(deskryptor_kolejki_klienta) < 0 ) wyjscie_z_bledem("Nie moge zamknac kolejki");
            break;


        case CONNECT:
            wypisz_wysrodkowane("--- Odebrano zlecenie CONNECT ---");
            id_drugiego_klienta = (int)komunikat[1];

            //komunikat_odpowiedz->m_type = CONNECT;
            //komunikat_odpowiedz->klucz_kolejki = kolejki_klientow[id_drugiego_klienta - 1];
            deskryptor_kolejki_klienta = mq_open(kolejki_klientow[id_klienta - 1], O_RDWR);
            if(deskryptor_kolejki_klienta < 0) wyjscie_z_bledem("Nie mozna otworzyc kolejki klienta.");
            komunikat_odpowiedz[0] = id_drugiego_klienta;
            strcat(komunikat_odpowiedz, kolejki_klientow[id_drugiego_klienta - 1]);

            if(mq_send(deskryptor_kolejki_klienta, komunikat_odpowiedz, MAX_DLUGOSC_KOMUNIKATU, CONNECT) < 0 ) wyjscie_z_bledem("Nie udalo sie wyslac komunikatu");


            memset(komunikat_odpowiedz, 0, strlen(komunikat_odpowiedz));
            deskryptor_kolejki_klienta = mq_open(kolejki_klientow[id_drugiego_klienta - 1], O_RDWR);
            if(deskryptor_kolejki_klienta < 0) wyjscie_z_bledem("Nie mozna otworzyc kolejki klienta.");
            komunikat_odpowiedz[0] = id_klienta;
            strcat(komunikat_odpowiedz, kolejki_klientow[id_klienta - 1]);

            if(mq_send(deskryptor_kolejki_klienta, komunikat_odpowiedz, MAX_DLUGOSC_KOMUNIKATU, CONNECT) < 0 ) wyjscie_z_bledem("Nie udalo sie wyslac komunikatu");
            if( mq_close(deskryptor_kolejki_klienta) < 0 ) wyjscie_z_bledem("Nie moge zamknac kolejki");

            dostepni_klienci[id_klienta - 1] = false;
            dostepni_klienci[id_drugiego_klienta - 1] = false;
            break;


        case DISCONNECT:
            wypisz_wysrodkowane("--- Odebrano zlecenie DISCONNECT ---");
            id_klienta = (int)komunikat[0];
            id_drugiego_klienta = (int)komunikat[1];

            deskryptor_kolejki_klienta = mq_open(kolejki_klientow[id_drugiego_klienta - 1], O_RDWR);
            if(deskryptor_kolejki_klienta < 0) wyjscie_z_bledem("Nie mozna otworzyc kolejki klienta.");
            if(mq_send(deskryptor_kolejki_klienta, komunikat_odpowiedz, MAX_DLUGOSC_KOMUNIKATU, DISCONNECT) < 0 ) wyjscie_z_bledem("Nie udalo sie wyslac komunikatu");
            if( mq_close(deskryptor_kolejki_klienta) < 0 ) wyjscie_z_bledem("Nie moge zamknac kolejki");

            dostepni_klienci[id_klienta - 1] = true;
            dostepni_klienci[id_drugiego_klienta - 1] = true;
            break;


        case STOP:
            wypisz_wysrodkowane("--- Odebrano zlecenie STOP ---");
            id_klienta = (int)komunikat[0];
            kolejki_klientow[id_klienta - 1] = NULL;
            dostepni_klienci[id_klienta - 1] = false;
            break;

        default:
            wypisz_wysrodkowane("--- Odebrano nieznane zlecenie ---");
    }
}



int main() {

    wypisz_linie_gwiazdek();
    wypisz_wysrodkowane("Uruchamiam serwer.\n");

    for(int i = 0; i < MAX_KLIENTOW; i++){
        kolejki_klientow[i] = NULL;
        dostepni_klienci[i] = false;
    }

    struct mq_attr attr;
    attr.mq_maxmsg = 10;
    attr.mq_msgsize = MAX_DLUGOSC_KOMUNIKATU;

    //key_t klucz_kolejki = ftok(getenv("HOME"), ID_SERWERA);
    //id_kolejki_serwera = msgget(klucz_kolejki, IPC_CREAT | 0666);
    deskryptor_kolejki_serwera = mq_open(NAZWA_KOLEJKI_SERWERA, O_RDWR | O_CREAT, 0666, &attr);
    if( deskryptor_kolejki_serwera < 0 )wyjscie_z_bledem("Nie mozna stworzyc kolejki.");

    wypisz_wysrodkowane("Serwer uruchomiony.\n");

    //printf("\nKlucz kolejki serwera: %d\n", klucz_kolejki);
    //printf("ID kolejki serwera: %d\n\n", id_kolejki_serwera);

    signal(SIGINT, funkcja_wyjscia);

    //char* komunikat = (char*)calloc(MAX_MSG_LEN, sizeof(char));
    char komunikat[MAX_DLUGOSC_KOMUNIKATU];
    unsigned int priorytet;
    while(1) {
        if(mq_receive(deskryptor_kolejki_serwera, komunikat, MAX_DLUGOSC_KOMUNIKATU, &priorytet) < 0 ) wyjscie_z_bledem("Nie moge otrzymac zadnych komunikatow.");
        glowna_petla(komunikat, priorytet);
    }
}