#include <stdio.h>
#include <stdlib.h>
#include <sys/msg.h>
#include <sys/ipc.h>
#include <errno.h>
#include <stdbool.h>
#include <string.h>
#include <signal.h>

#include "config.h"

key_t kolejki_klientow[MAX_KLIENTOW];
bool dostepni_klienci[MAX_KLIENTOW];
int id_kolejki_serwera;

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
    msg_buf* komunikat = (msg_buf*)malloc(sizeof(msg_buf));
    komunikat->m_type = STOP;

    for(int i = 0; i < MAX_KLIENTOW; i++) {
        key_t kolejka = kolejki_klientow[i];
        if(kolejka != -1) {
            int id_kolejki_klienta = msgget(kolejka, 0);
            if( msgsnd(id_kolejki_klienta, komunikat, ROZMIAR_BUFORA, 0) < 0) wyjscie_z_bledem("Nie moge wyslac komunikatu.");
            if( msgrcv(id_kolejki_serwera, komunikat, ROZMIAR_BUFORA, STOP, 0) < 0) wyjscie_z_bledem("Nie moge otrzymac komunikatu.");
        }
    }
    msgctl(id_kolejki_serwera, IPC_RMID, NULL); // usuń kolejkę
    wypisz_linie_gwiazdek();
    exit(EXIT_SUCCESS);
}

int tworz_id_klienta() {
    int i = 0;
    while(i < MAX_KLIENTOW && kolejki_klientow[i] != -1) i++;
    if (i < MAX_KLIENTOW) return i+1;       // podaj pierwsze wolne miejsce w statycznej tablicy
    else return -1;
}



void glowna_petla(msg_buf* komunikat) {
    msg_buf* komunikat_odpowiedz = (msg_buf*)malloc(sizeof(msg_buf));
    int id_klienta = komunikat->ID_klienta;
    int id_drugiego_klienta;

    switch(komunikat->m_type) {

        case INIT: ;
            wypisz_wysrodkowane("--- Odebrano zlecenie INIT ---");
            int id = tworz_id_klienta();
            if(id == -1){
                wypisz_wysrodkowane("--- Nie mozna utworzyc wiecej klientow. ---");
                return;
            }

            komunikat_odpowiedz->m_type = id;
            int id_kolejki_klienta = msgget(komunikat->klucz_kolejki, 0);
            if(id_kolejki_klienta < 0) wyjscie_z_bledem("Nie mozna otworzyc kolejki klienta.");

            kolejki_klientow[id - 1] = komunikat->klucz_kolejki;
            dostepni_klienci[id - 1] = true;

            if( msgsnd(id_kolejki_klienta, komunikat_odpowiedz, ROZMIAR_BUFORA, 0) < 0 ) wyjscie_z_bledem("Nie udalo sie wyslac komunikatu.");
            break;


        case LIST:
            wypisz_wysrodkowane("--- Odebrano zlecenie LIST ---");
            strcpy(komunikat_odpowiedz->m_text, "");

            bool flag = false;

            for(int i = 0; i < MAX_KLIENTOW; i++) {
                if(kolejki_klientow[i] != -1 && i+1 != id_klienta) {
                    char temp[SZEROKOSC_KONSOLI];
                    sprintf(temp, "Klient %d - %s\n", i+1, dostepni_klienci[i] ? "dostepny" : "zajety");
                    strcat(komunikat_odpowiedz->m_text, temp);
                    flag = true;
                }
            }
            if(!flag) strcat(komunikat_odpowiedz->m_text, "Brak innych klientow.\n");
            id_kolejki_klienta = msgget(kolejki_klientow[id_klienta - 1], 0);
            if(id_kolejki_klienta < 0) wyjscie_z_bledem("Nie mozna otworzyc kolejki klienta.");

            komunikat_odpowiedz->m_type = id_klienta;
            if( msgsnd(id_kolejki_klienta, komunikat_odpowiedz, ROZMIAR_BUFORA, 0) < 0 ) wyjscie_z_bledem("Nie udalo sie wyslac komunikatu");
            break;


        case CONNECT:
            wypisz_wysrodkowane("--- Odebrano zlecenie CONNECT ---");
            id_drugiego_klienta = komunikat->ID_polaczonego_klienta;

            komunikat_odpowiedz->m_type = CONNECT;
            komunikat_odpowiedz->klucz_kolejki = kolejki_klientow[id_drugiego_klienta - 1];
            id_kolejki_klienta = msgget(kolejki_klientow[id_klienta - 1], 0);
            if(id_kolejki_klienta < 0) wyjscie_z_bledem("Nie mozna otworzyc kolejki klienta.");
            if( msgsnd(id_kolejki_klienta, komunikat_odpowiedz, ROZMIAR_BUFORA, 0) < 0 ) wyjscie_z_bledem("Nie udalo sie wyslac komunikatu");


            komunikat_odpowiedz->ID_klienta = id_klienta;
            komunikat_odpowiedz->m_type = CONNECT;
            komunikat_odpowiedz->klucz_kolejki = kolejki_klientow[id_klienta - 1];
            id_kolejki_klienta = msgget(kolejki_klientow[id_drugiego_klienta - 1], 0);
            if(id_kolejki_klienta < 0) wyjscie_z_bledem("Nie mozna otworzyc kolejki klienta.");
            if( msgsnd(id_kolejki_klienta, komunikat_odpowiedz, ROZMIAR_BUFORA, 0) < 0 ) wyjscie_z_bledem("Nie udalo sie wyslac komunikatu");

            dostepni_klienci[id_klienta - 1] = false;
            dostepni_klienci[id_drugiego_klienta - 1] = false;
            break;


        case DISCONNECT:
            wypisz_wysrodkowane("--- Odebrano zlecenie DISCONNECT ---");
            id_drugiego_klienta = komunikat->ID_polaczonego_klienta;

            komunikat_odpowiedz->m_type = DISCONNECT;
            id_kolejki_klienta = msgget(kolejki_klientow[id_drugiego_klienta - 1], 0);
            if(id_kolejki_klienta < 0) wyjscie_z_bledem("Nie mozna otworzyc kolejki klienta.");
            if( msgsnd(id_kolejki_klienta, komunikat_odpowiedz, ROZMIAR_BUFORA, 0) < 0 ) wyjscie_z_bledem("Nie udalo sie wyslac komunikatu");

            dostepni_klienci[id_klienta - 1] = true;
            dostepni_klienci[id_drugiego_klienta - 1] = true;
            break;


        case STOP:
            wypisz_wysrodkowane("--- Odebrano zlecenie STOP ---");
            kolejki_klientow[id_klienta - 1] = -1;
            dostepni_klienci[id_klienta - 1] = false;
            break;

        default:
            wypisz_wysrodkowane("--- Odebrano nieznane zlecenie ---");
    }
}



int main() {
    for(int i = 0; i < MAX_KLIENTOW; i++){
        kolejki_klientow[i] = -1;
        dostepni_klienci[i] = false;
    }

    key_t klucz_kolejki = ftok(getenv("HOME"), ID_SERWERA);
    id_kolejki_serwera = msgget(klucz_kolejki, IPC_CREAT | 0666);

    wypisz_linie_gwiazdek();
    wypisz_wysrodkowane("Uruchamiam serwer.");

    printf("\nKlucz kolejki serwera: %d\n", klucz_kolejki);
    printf("ID kolejki serwera: %d\n\n", id_kolejki_serwera);

    signal(SIGINT, funkcja_wyjscia);

    msg_buf* komunikat = (msg_buf*)malloc(sizeof(msg_buf));
    while(1) {
        if( msgrcv(id_kolejki_serwera, komunikat, ROZMIAR_BUFORA, -1000, 0) < 0 ) wyjscie_z_bledem("Nie moge otrzymac zadnych komunikatow.");
        glowna_petla(komunikat);
    }
}