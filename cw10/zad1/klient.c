#include "pomocnicze.h"
#include "funkcje_tekstowe.h"
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


char* nazwa;
char* sciezka_gniazda_unix;
char* sciezka_gniazda_inet;
int numer_portu;
int deskryptor_gniazda_serwera; // wystarczy jeden, bo klient laczy sie na jeden sposob

pthread_t watek_pingujacy;
int gra_rozpoczeta = 0;
int odczytano_start = 0;
ZNAK znak = BRAK;
char znak_char = ' ';

char input[2];


void wyswietl_GUI(char* tresc){
    wypisz_linie_gwiazdek();
    printf("\n");
    char* temp_string = (char*)calloc(SZEROKOSC_KONSOLI, sizeof(char));
    sprintf(temp_string, "     #     #     ");
    wypisz_wysrodkowane(temp_string);
    sprintf(temp_string, "  %c  #  %c  #  %c  ", tresc[0], tresc[1], tresc[2]);
    wypisz_wysrodkowane(temp_string);
    sprintf(temp_string, "     #     #     ");
    wypisz_wysrodkowane(temp_string);

    sprintf(temp_string, "#################");
    wypisz_wysrodkowane(temp_string);

    sprintf(temp_string, "     #     #     ");
    wypisz_wysrodkowane(temp_string);
    sprintf(temp_string, "  %c  #  %c  #  %c  ", tresc[3], tresc[4], tresc[5]);
    wypisz_wysrodkowane(temp_string);
    sprintf(temp_string, "     #     #     ");
    wypisz_wysrodkowane(temp_string);

    sprintf(temp_string, "#################");
    wypisz_wysrodkowane(temp_string);

    sprintf(temp_string, "     #     #     ");
    wypisz_wysrodkowane(temp_string);
    sprintf(temp_string, "  %c  #  %c  #  %c  ", tresc[6], tresc[7], tresc[8]);
    wypisz_wysrodkowane(temp_string);
    sprintf(temp_string, "     #     #     ");
    wypisz_wysrodkowane(temp_string);

    printf("\n");

    sprintf(temp_string, "Twoj znak %c\n\n", znak_char);
    wypisz_linie_gwiazdek();
    printf("%s", temp_string);

}


void rozlacz() {
    printf("\n");
    wypisz_wysrodkowane("_____ Koncze prace _____");

    wyslij_wiadomosc(deskryptor_gniazda_serwera, LOGOUT, NULL);
    sleep(1);   // aby serwer mial czas zadzialac
    pthread_cancel(watek_pingujacy);
    shutdown(deskryptor_gniazda_serwera, SHUT_RDWR);
    if( close(deskryptor_gniazda_serwera) < 0 ) wyjscie_z_bledem("Nie udalo sie zamknac gniazda serwera.");
}



void read_input() {
    input[0] = '.';
    scanf("%s", input);
}
void* obsluga_odczytu(){
    read_input();
    wyslij_wiadomosc(deskryptor_gniazda_serwera, GAME_MOVE, input);
    return NULL;
}

void obsluga_wiadomosci(wiadomosc* temp_wiadomosc){

    if( gra_rozpoczeta == 0 && temp_wiadomosc->typ == GAME_WAIT) {
        printf("\n");
        wypisz_wysrodkowane("_____  Oczekiwanie na rozpoczecie gry... _____");

    } else if( temp_wiadomosc->typ == GAME_FOUND) {
        printf("\n");
        wypisz_wysrodkowane("_____ Znaleziono gre! _____");
        gra_rozpoczeta = 1;

        if (temp_wiadomosc->tresc[0] == 'X') {
            znak = KRZYZYK;
            znak_char = 'X';
        } else if (temp_wiadomosc->tresc[0] == 'O') {
            znak = KOLKO;
            znak_char = 'O';
        } else wyjscie_z_bledem("Otrzymano nieodpowiedni znak z serwera.");

        if (odczytano_start == 0 && znak == KRZYZYK) {
            wyslij_wiadomosc(deskryptor_gniazda_serwera, GAME_MOVE, "10");
            odczytano_start = 1;
        }
    } else if( temp_wiadomosc->typ == GAME_MOVE ) {
        if (odczytano_start == 0) {
            wyslij_wiadomosc(deskryptor_gniazda_serwera, GAME_MOVE, "10");
            odczytano_start = 1;
        } else {
            wyswietl_GUI(temp_wiadomosc->tresc);

            pthread_t temp_watek_odczytujacy;
            pthread_create(&temp_watek_odczytujacy, NULL, obsluga_odczytu, NULL);

            wiadomosc* temp_temp_wiadomosc = wczytaj_wiadomosc_nieblokujaco(deskryptor_gniazda_serwera);
            while(input[0] == '.'){
                if( temp_temp_wiadomosc == NULL ){
                    usleep(10000 * CZAS_SPANIA);
                    continue;
                }
                else{
                    if( temp_temp_wiadomosc->typ == PING ){
                        wyslij_wiadomosc(deskryptor_gniazda_serwera, PING, NULL);
                    }
                }
            }


        }
    }else if( temp_wiadomosc->typ == GAME_FINISHED ){
        printf("\n");
        wypisz_wysrodkowane("_____ Koniec gry _____");
        printf("Wynik: %s", temp_wiadomosc->tresc);
        wyslij_wiadomosc(deskryptor_gniazda_serwera, LOGOUT, NULL);
        printf("\n");
        wypisz_wysrodkowane("_____ Wylogowywanie z serwera... _____");
    }
}

void* obsluga_pingowania(){
    int tick_serwera = 500;
    sleep(1);
    while(1){
        wiadomosc* temp_wiadomosc = wczytaj_wiadomosc_nieblokujaco(deskryptor_gniazda_serwera);
        tick_serwera -= 1;
        if(temp_wiadomosc == NULL){
            if(tick_serwera > 0) {
                //sleep(CZAS_SPANIA);
                usleep(10000 * CZAS_SPANIA);
                continue;
            }
            else{
                printf("Serwer [deskryptor: %d] nie odpowiada od %d sekund. Rozlaczam...", deskryptor_gniazda_serwera, CZAS_SPANIA * 5);
                rozlacz();
            }
        } else if(temp_wiadomosc->typ == PING) {
            tick_serwera = 500;
            wyslij_wiadomosc(deskryptor_gniazda_serwera, PING, NULL);   // odpinguj serwerowi
            //sleep(CZAS_SPANIA);
            usleep(10000 * CZAS_SPANIA);
        }else{
            obsluga_wiadomosci(temp_wiadomosc);
        }
    }
}

void polacz_z_serwerem(int flaga) {
    if(flaga == 1) {
        struct sockaddr_un adres_gniazda_unix;

        adres_gniazda_unix.sun_family = AF_UNIX;
        strcpy(adres_gniazda_unix.sun_path, sciezka_gniazda_unix);

        deskryptor_gniazda_serwera = socket(AF_UNIX, SOCK_STREAM, 0);
        if(deskryptor_gniazda_serwera < 0) wyjscie_z_bledem("Nieudane otworzenie gniazda (Unix).");
        if( connect(deskryptor_gniazda_serwera, (struct sockaddr*) &adres_gniazda_unix, sizeof(adres_gniazda_unix)) < 0 ) wyjscie_z_bledem("Nieudane polaczenie z serwerem (Unix).");

    } else{
        struct sockaddr_in adres_gniazda_inet;

        adres_gniazda_inet.sin_family = AF_INET;
        adres_gniazda_inet.sin_port = htons(numer_portu);
        adres_gniazda_inet.sin_addr.s_addr = inet_addr(sciezka_gniazda_inet);

        deskryptor_gniazda_serwera = socket(AF_INET, SOCK_STREAM, 0);
        if(deskryptor_gniazda_serwera < 0) wyjscie_z_bledem("Nieudane otworzenie gniazda (Inet).");
        if( connect(deskryptor_gniazda_serwera, (struct sockaddr*) &adres_gniazda_inet, sizeof(adres_gniazda_inet)) < 0 ) wyjscie_z_bledem("Nieudane polaczenie z serwerem (Inet).");
    }

    printf("\n");
    wypisz_wysrodkowane("_____ Proba zalogowania na serwer _____");

    wyslij_wiadomosc(deskryptor_gniazda_serwera, LOGIN_REQUEST, nazwa);
    wiadomosc* temp_wiadomosc = wczytaj_wiadomosc(deskryptor_gniazda_serwera);
    if(temp_wiadomosc->typ == LOGIN_APPROVED) {
        printf("Logowanie zakonczone.\n");
    }
    else if(temp_wiadomosc->typ == LOGIN_REJECTED){
        printf("Logowanie nie powiodlo sie. Powod: %s\n", temp_wiadomosc->tresc);
        exit(1);
    }

    if( pthread_create(&watek_pingujacy, NULL, obsluga_pingowania, NULL) != 0) wyjscie_z_bledem("Nie udalo sie utworzyc watku (do obslugi pingowania).");
}

int main(int argc, char** argv) {
    // ./klient <nazwa> LOCAL <socket Unix>
    // ./klient <nazwa> <IPv4> <PORT>

    if (argc < 4) wyjscie_z_bledem("Podano zbyt mało argumentów.");

    signal(SIGINT, rozlacz);

    nazwa = argv[1];
    if( strcmp(argv[2], "LOCAL") == 0 ){        // polaczenie lokalne
        sciezka_gniazda_unix = argv[3];

        polacz_z_serwerem(1);
    } else{                                     // polaczenie ipv4
        sciezka_gniazda_inet = argv[2];
        numer_portu = (int) strtol(argv[3], (char**)NULL, 10);

        polacz_z_serwerem(0);
    }

    pthread_join(watek_pingujacy, NULL);
    rozlacz();

    return 0;
}