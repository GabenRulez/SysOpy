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


int numer_portu;
char* sciezka_gniazda_UNIX;

struct sockaddr_un adres_gniazda_unix;
int deskryptor_gniazda_unix;
struct sockaddr_in adres_gniazda_inet;
int deskryptor_gniazda_inet;

pthread_t watek_obslugujacy_siec;
pthread_t watek_pingujacy;

klient* klienci[MAX_KLIENTOW];
pthread_mutex_t mutex_tablicy_klientow = PTHREAD_MUTEX_INITIALIZER;
int oczekujacy_klient = -1;

gra* gry[MAX_KLIENTOW / 2];

const int SPRAWDZANE_LINIE[8][3] = { {0, 3, 6}, {1, 4, 7}, {2, 5, 8}, {0, 1, 2}, {3, 4, 5}, {6, 7, 8}, {0, 4, 8}, {2, 4, 6} };

void inicjalizuj_serwer(){
    printf("\n");
    wypisz_wysrodkowane("_____ Uruchamianie serwera _____");

    adres_gniazda_unix.sun_family = AF_UNIX;
    strcpy(adres_gniazda_unix.sun_path, sciezka_gniazda_UNIX);

    deskryptor_gniazda_unix = socket(AF_UNIX, SOCK_STREAM, 0);
    if(deskryptor_gniazda_unix < 0) wyjscie_z_bledem("Nieudane utworzenie gniazda (Unix).");

    if( bind(deskryptor_gniazda_unix, (struct sockaddr*) &adres_gniazda_unix, sizeof(adres_gniazda_unix)) < 0 ) wyjscie_z_bledem("Nieudane związanie gniazda z jego adresem (Unix).");
    if( listen(deskryptor_gniazda_unix, MAX_KLIENTOW) < 0 ) wyjscie_z_bledem("Nieudane rozpoczecie akceptowania polaczen od klientow (Unix).");

    printf("Gniazdo UNIX nasluchuje na %s\n", sciezka_gniazda_UNIX);

    /*************************************************/

    struct in_addr temp = *(struct in_addr*) gethostbyname("localhost")->h_addr;
    adres_gniazda_inet.sin_family = AF_INET;
    adres_gniazda_inet.sin_port = htons(numer_portu);
    adres_gniazda_inet.sin_addr.s_addr = temp.s_addr;

    deskryptor_gniazda_inet = socket(AF_INET, SOCK_STREAM, 0);
    if(deskryptor_gniazda_inet < 0) wyjscie_z_bledem("Nieudane utworzenie gniazda (Inet).");

    if( bind(deskryptor_gniazda_inet, (struct sockaddr*) &adres_gniazda_inet, sizeof(adres_gniazda_inet)) < 0 ) wyjscie_z_bledem("Nieudane związanie gniazda z jego adresem (Inet).");
    if( listen(deskryptor_gniazda_inet, MAX_KLIENTOW) < 0 ) wyjscie_z_bledem("Nieudane rozpoczecie akceptowania polaczen od klientow (Inet).");

    printf("Gniazdo INET nasluchuje na %s:%d\n", inet_ntoa(temp), numer_portu);
}


void wylacz_serwer(){
    printf("\n");
    wypisz_wysrodkowane("_____ Wylaczanie serwera _____");

    if( pthread_cancel(watek_obslugujacy_siec) != 0 ) wyjscie_z_bledem("Nie udalo sie wyslac sygnalu CANCEL do watku obslugujacego siec.");
    if( pthread_cancel(watek_pingujacy) != 0 ) wyjscie_z_bledem("Nie udalo sie wyslac sygnalu CANCEL do watku obslugujacego pingowanie");

    if( shutdown(deskryptor_gniazda_unix, SHUT_RDWR) < 0 ) wyjscie_z_bledem("Nie udalo sie wylaczyc gniazda (Unix).");
    if( close(deskryptor_gniazda_unix) < 0 ) wyjscie_z_bledem("Nie udalo sie zamknac gniazda (Unix).");
    if( unlink(sciezka_gniazda_UNIX) < 0 ) wyjscie_z_bledem("Nieudane usuwanie pliku reprezentujacego gniazdo (Unix).");

    if( shutdown(deskryptor_gniazda_inet, SHUT_RDWR) < 0 ) wyjscie_z_bledem("Nie udalo sie wylaczyc gniazda (Inet).");
    if( close(deskryptor_gniazda_inet) < 0 ) wyjscie_z_bledem("Nie udalo sie zamknac gniazda (Inet).");

    printf("Poprawnie wylaczono serwer.\n");

    exit(0);
}

void przerwij_polaczenie(int deskryptor){
    if( shutdown(deskryptor, SHUT_RDWR) < 0 ) wyjscie_z_bledem("Nie udalo sie wylaczyc gniazda (przerwac polaczenia).");
    if( close(deskryptor) < 0 ) wyjscie_z_bledem("Nie udalo sie zamknac gniazda (przerwac polaczenia).");
}

void usun_klienta(int deskryptor){
    for (int i=0; i<MAX_KLIENTOW; i++){
        if( klienci[i] != NULL && klienci[i]->deskryptor == deskryptor ){
            free(klienci[i]);
            klienci[i] = NULL;
            if( oczekujacy_klient == i ) oczekujacy_klient = -1;
            return;
        }
    }
}

int znajdz_gre_klienta(int indeks_klienta){
    for(int i=0; i<MAX_KLIENTOW/2; i++){
        if(gry[i]->gracz_1 == indeks_klienta || gry[i]->gracz_2 == indeks_klienta) return i;
    }
    return -1;
}

void stworz_mecz(int indeks) {
    if(oczekujacy_klient < 0) {

        printf("Klient '%s' [indeks: %d] czeka na przeciwnika.\n", klienci[indeks]->nazwa, indeks);
        wyslij_wiadomosc(klienci[indeks]->deskryptor, GAME_WAIT, NULL);
        oczekujacy_klient = indeks;

    } else {

        printf("\n");
        wypisz_wysrodkowane("_____ Tworzenie nowej gry _____");
        printf("Tworze mecz miedzy klientem '%s' [indeks: %d], a klientem '%s' [indeks: %d].\n", klienci[indeks]->nazwa, indeks, klienci[oczekujacy_klient]->nazwa, oczekujacy_klient);

        int indeks_gry = -1;
        for(int i=0; i<MAX_KLIENTOW/2; i++){        // znajdz wolny slot na gre
            if(gry[i]==NULL){
                indeks_gry = i;
                break;
            }
        }       // nie powinna byc mozliwa sytuacja, ze nie ma wolnego miejsca na gre, ale i tak sprawdze
        if(indeks_gry == -1) wyjscie_z_bledem("Nastapila sytuacja, gdzie zabraklo miejsca na gry (nie powinno to miec miejsca)!");

        gra* temp_gra = (gra*)malloc(sizeof(gra));
        temp_gra->gracz_1 = indeks;
        temp_gra->gracz_2 = oczekujacy_klient;
        for(int j=0; j<9; j++) temp_gra->plansza[j] = BRAK;

        gry[indeks_gry] = temp_gra;

        if(losowy_int(0, 1) == 0) {
            wyslij_wiadomosc(klienci[indeks]->deskryptor, GAME_FOUND, "X");
            wyslij_wiadomosc(klienci[oczekujacy_klient]->deskryptor, GAME_FOUND, "O");
            klienci[indeks]->znak = KRZYZYK;
            klienci[oczekujacy_klient]->znak = KOLKO;
        } else {
            wyslij_wiadomosc(klienci[indeks]->deskryptor, GAME_FOUND, "O");
            wyslij_wiadomosc(klienci[oczekujacy_klient]->deskryptor, GAME_FOUND, "X");
            klienci[indeks]->znak = KOLKO;
            klienci[oczekujacy_klient]->znak = KRZYZYK;
        }
        oczekujacy_klient = -1;
    }
}

void* obsluga_sieci(){
    struct pollfd deskryptory[MAX_KLIENTOW + 2];

    deskryptory[MAX_KLIENTOW].fd = deskryptor_gniazda_unix;
    deskryptory[MAX_KLIENTOW].events = POLLIN;

    deskryptory[MAX_KLIENTOW + 1].fd = deskryptor_gniazda_inet;
    deskryptory[MAX_KLIENTOW + 1].events = POLLIN;

    oczekujacy_klient = -1; // nikt nie czeka

    while(1){
        pthread_mutex_lock(&mutex_tablicy_klientow);
        for(int i=0; i<MAX_KLIENTOW; i++){
            if( klienci[i] != NULL){
                deskryptory[i].fd = klienci[i]->deskryptor;
                deskryptory[i].events = POLLIN;     /* requested events */
                deskryptory[i].revents = 0;         /* returned events  */
            }
            else{
                deskryptory[i].fd = -1;
            }
        }
        deskryptory[MAX_KLIENTOW].revents = 0;
        deskryptory[MAX_KLIENTOW+1].revents = 0;
        pthread_mutex_unlock(&mutex_tablicy_klientow);

        poll(deskryptory, MAX_KLIENTOW +2, -1);

        pthread_mutex_lock(&mutex_tablicy_klientow);
        for(int i=0; i<MAX_KLIENTOW+2; i++){
            if(i < MAX_KLIENTOW && klienci[i] == NULL) continue;    // pomijamy pustych klientów

            if( deskryptory[i].revents == POLLIN ){ // możliwy odczyt

                if( deskryptory[i].fd == deskryptor_gniazda_unix || deskryptory[i].fd == deskryptor_gniazda_inet){      // login

                    printf("\n");
                    wypisz_wysrodkowane("_____ Odebrano prośbę o login _____");
                    int deskryptor_gniazda_klienta = accept(deskryptory[i].fd, NULL, NULL);
                    if(deskryptor_gniazda_klienta < 0) wyjscie_z_bledem("Nieudane polaczenie z gniazdem klienta.");

                    wiadomosc* temp_wiadomosc = wczytaj_wiadomosc(deskryptor_gniazda_klienta);

                    int flag = 0;
                    for(int j=0; j<MAX_KLIENTOW; j++){      // sprawdzenie, czy ktos o tej nazwie nie jest juz w systemie
                        if(klienci[j] != NULL && strcmp(klienci[j]->nazwa, temp_wiadomosc->tresc) == 0){
                            printf("Nazwa '%s' figuruje juz w systemie.\n", temp_wiadomosc->tresc);
                            wyslij_wiadomosc(deskryptor_gniazda_klienta, LOGIN_REJECTED, "Wybrana nazwa jest juz w uzyciu.");
                            przerwij_polaczenie(deskryptor_gniazda_klienta);
                            flag = 1;
                            break;
                        }
                    }
                    if(flag==1) continue;


                    int temp_indeks = -1;
                    for(int j=0; j<MAX_KLIENTOW; j++){      // ustawienie indeksu na pierwszy wolny
                        if(klienci[j] == NULL){
                            temp_indeks = j;
                            break;
                        }
                    }
                    if( temp_indeks == -1 ){                // gdy nie ma wolnych miejsc
                        printf("Brak wolnych miejsc na uzytkownika '%s'.\n", temp_wiadomosc->tresc);
                        wyslij_wiadomosc(deskryptor_gniazda_klienta, LOGIN_REJECTED, "Brak wolnych miejsc na serwerze. Sprobuj pozniej.");
                        przerwij_polaczenie(deskryptor_gniazda_klienta);
                        continue;
                    }


                    klient* temp_klient = (klient*)malloc(sizeof(klient));
                    temp_klient->deskryptor = deskryptor_gniazda_klienta;
                    temp_klient->ticks = 5;
                    strcpy(temp_klient->nazwa, temp_wiadomosc->tresc);

                    klienci[temp_indeks] = temp_klient;     // zapamietanie klienta
                    wyslij_wiadomosc(deskryptor_gniazda_klienta, LOGIN_APPROVED, NULL);
                    printf("Poprawnie zalogowano uzytkownika '%s' [indeks: %d].\n", temp_wiadomosc->tresc, temp_indeks);

                    stworz_mecz(temp_indeks);

                }
                else{
                    wiadomosc* temp_wiadomosc = wczytaj_wiadomosc(deskryptory[i].fd);
                    if(temp_wiadomosc->typ == PING){
                        klienci[i]->ticks = 5;
                    }
                    else if(temp_wiadomosc->typ == LOGOUT){
                        printf("Klient '%s' [indeks: %d] wylogowuje sie.\n", klienci[i]->nazwa, i);
                        przerwij_polaczenie(deskryptory[i].fd);
                        usun_klienta(deskryptory[i].fd);
                    }

                    else if(temp_wiadomosc->typ == GAME_MOVE){
                        gra* temp_gra = gry[znajdz_gre_klienta(i)];
                        int wybrane_pole = (int) strtol(temp_wiadomosc->tresc, (char**)NULL, 10);
                        ZNAK temp_znak = klienci[i]->znak;

                        temp_gra->plansza[wybrane_pole] = temp_znak;    // zaakceptuj ruch




                        int nastepny_klient;        // znajdz potencjalnego nastepnego klienta (tego, ktorego jest kolejka)
                        if(temp_gra->gracz_1 == i){
                            nastepny_klient = temp_gra->gracz_2;
                        }
                        else{
                            nastepny_klient = temp_gra->gracz_1;
                        }



                        int temp_suma = 0;
                        for(int j=0; j<8; j++){
                            temp_suma = 0;
                            for(int k=0; k<3; k++){
                                ZNAK znak_sumowany = temp_gra->plansza[SPRAWDZANE_LINIE[j][k]];
                                if(znak_sumowany == KOLKO) temp_suma += 1;
                                else if(znak_sumowany == KRZYZYK) temp_suma += 3;
                                else temp_suma -= 999;
                            }
                            if(temp_suma == 3 || temp_suma == 9) break;     // jesli ktorys wygral to nie sprawdzaj dalej
                        }



                        switch (temp_suma) {
                            case 3:{
                                int temp_j = znajdz_gre_klienta(i);
                                free(gry[temp_j]);
                                gry[temp_j] = NULL;

                                if( klienci[i]->znak == KOLKO ){
                                    wyslij_wiadomosc(deskryptory[i].fd, GAME_FINISHED, "Wygrales gre!");
                                    wyslij_wiadomosc(deskryptory[nastepny_klient].fd, GAME_FINISHED, "Przegrales gre!");
                                    printf("\n");
                                    wypisz_wysrodkowane("_____ Koniec gry _____");
                                    printf("Klient '%s' [indeks: %d] wygral z klientem '%s' [indeks: %d].\n", klienci[i]->nazwa, i, klienci[nastepny_klient]->nazwa, nastepny_klient);
                                }
                                else{
                                    wyslij_wiadomosc(deskryptory[i].fd, GAME_FINISHED, "Przegrales gre!");
                                    wyslij_wiadomosc(deskryptory[nastepny_klient].fd, GAME_FINISHED, "Wygrales gre!");
                                    printf("\n");
                                    wypisz_wysrodkowane("_____ Koniec gry _____");
                                    printf("Klient '%s' [indeks: %d] wygral z klientem '%s' [indeks: %d].\n", klienci[nastepny_klient]->nazwa, nastepny_klient, klienci[i]->nazwa, i);
                                }
                                break;
                            }


                            case 9:{
                                int temp_j = znajdz_gre_klienta(i);
                                free(gry[temp_j]);
                                gry[temp_j] = NULL;

                                if( klienci[i]->znak == KRZYZYK ){
                                    wyslij_wiadomosc(deskryptory[i].fd, GAME_FINISHED, "Wygrales gre!");
                                    wyslij_wiadomosc(deskryptory[nastepny_klient].fd, GAME_FINISHED, "Przegrales gre!");
                                    printf("\n");
                                    wypisz_wysrodkowane("_____ Koniec gry _____");
                                    printf("Klient '%s' [indeks: %d] wygral z klientem '%s' [indeks: %d].\n", klienci[i]->nazwa, i, klienci[nastepny_klient]->nazwa, nastepny_klient);
                                }
                                else{
                                    wyslij_wiadomosc(deskryptory[i].fd, GAME_FINISHED, "Przegrales gre!");
                                    wyslij_wiadomosc(deskryptory[nastepny_klient].fd, GAME_FINISHED, "Wygrales gre!");
                                    printf("\n");
                                    wypisz_wysrodkowane("_____ Koniec gry _____");
                                    printf("Klient '%s' [indeks: %d] wygral z klientem '%s' [indeks: %d].\n", klienci[nastepny_klient]->nazwa, nastepny_klient, klienci[i]->nazwa, i);
                                }
                                break;
                            }


                            default:
                                if(temp_suma < 0){  // zostaly jeszcze wolne miejsca, wiec gra toczy sie dalej
                                    char* temp_string = (char*)calloc(9, sizeof(char));
                                    for(int k=0; k<9; k++){
                                        switch(temp_gra->plansza[k]){
                                            case KRZYZYK:
                                                temp_string[k] = 'X';
                                                break;
                                            case KOLKO:
                                                temp_string[k] = 'O';
                                                break;
                                            default:
                                                temp_string[k] = '0' + (char)(k);
                                                break;
                                        }
                                    }

                                    wyslij_wiadomosc(deskryptory[nastepny_klient].fd, GAME_MOVE, temp_string);
                                }
                                else{   // koniec gry, remis
                                    int temp_j = znajdz_gre_klienta(i);
                                    free(gry[temp_j]);
                                    gry[temp_j] = NULL;

                                    wyslij_wiadomosc(deskryptory[i].fd, GAME_FINISHED, "Remis!");
                                    wyslij_wiadomosc(deskryptory[nastepny_klient].fd, GAME_FINISHED, "Remis!");
                                    printf("\n");
                                    wypisz_wysrodkowane("_____ Koniec gry _____");
                                    printf("Klient '%s' [indeks: %d] zremisowal z klientem '%s' [indeks: %d].\n", klienci[nastepny_klient]->nazwa, nastepny_klient, klienci[i]->nazwa, i);
                                }
                                break;
                        }
                    }   // koniec GAME_MOVE
                }
            }
            else if( deskryptory[i].revents == POLLHUP || deskryptory[i].revents == POLLERR ){   // rozłączenie lub błąd
                przerwij_polaczenie(deskryptory[i].fd);
                usun_klienta(deskryptory[i].fd);
            }
        }
        pthread_mutex_unlock(&mutex_tablicy_klientow);
    }
    return 0;
}

void* obsluga_pingowania(){
    while(1){
        sleep(CZAS_SPANIA);

        pthread_mutex_lock(&mutex_tablicy_klientow);
        for(int i=0; i<MAX_KLIENTOW; i++){
            if(klienci[i] != NULL){
                klienci[i]->ticks -= 1;
                if(klienci[i]->ticks > 0 ){
                    wyslij_wiadomosc(klienci[i]->deskryptor, PING, NULL);
                }
                else{
                    printf("Klient %s [indeks: %d] nie odpowiada od %d sekund. Rozlaczam...", klienci[i]->nazwa, i, CZAS_SPANIA * 5);
                    przerwij_polaczenie(klienci[i]->deskryptor);
                    usun_klienta(klienci[i]->deskryptor);
                }
            }
        }
        pthread_mutex_unlock(&mutex_tablicy_klientow);
    }
    return 0;
}


int main(int argc, char** argv){

    if (argc < 3) wyjscie_z_bledem("Podano zbyt mało argumentów.");
    numer_portu = (int) strtol(argv[1], (char**)NULL, 10);
    sciezka_gniazda_UNIX = argv[2];

    for(int i=0; i<MAX_KLIENTOW; i++) klienci[i] = NULL;
    for(int i=0; i<MAX_KLIENTOW/2; i++) gry[i] = NULL;

    inicjalizuj_serwer();
    signal(SIGINT, wylacz_serwer);

    if( pthread_create(&watek_obslugujacy_siec, NULL, obsluga_sieci, NULL) != 0) wyjscie_z_bledem("Nie udalo sie utworzyc watku (do obslugi sieci).");
    if( pthread_create(&watek_pingujacy, NULL, obsluga_pingowania, NULL) != 0) wyjscie_z_bledem("Nie udalo sie utworzyc watku (do obslugi pingowania).");

    pthread_join(watek_obslugujacy_siec, NULL);
    pthread_join(watek_pingujacy, NULL);

    wylacz_serwer();
    return 0;
}