#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/file.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/times.h>
#include <unistd.h>
#include <sys/sem.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <time.h>
#include <stdbool.h>

#include "pomocnicze.h"

union semun{
    int val;
    struct semid_ds *buf;
    unsigned short *array;
    struct seminfo *__buf;
};

void wyjdz_z_bledem(char* tekst){
    wypisz_wysrodkowane("--- ERROR ---");
    printf("%s\n\n", tekst);
    exit(1);
}

key_t klucz_zbioru_semaforow;
int identyfikator_zbioru_semaforow;

key_t klucz_segmentu_pamieci_wspolnej;
int identyfikator_segmentu_pamieci_wspolnej;



void wlasny_exit(int signum){
    usleep(100000);
    printf("[%d %s (pracownik_1)] Zamykaja sklep. Koncze prace.\n", getpid(), obecny_timestamp());

    semctl(identyfikator_zbioru_semaforow, 0, IPC_RMID, NULL);
    shmctl(identyfikator_segmentu_pamieci_wspolnej, IPC_RMID, NULL);
    exit(EXIT_SUCCESS);
}

void dodaj_zamowienie() {
    struct sembuf *sops_start = (struct sembuf *) calloc(1, sizeof(struct sembuf));
    sops_start[0].sem_num = 0;
    sops_start[0].sem_op = -1;
    semop(identyfikator_zbioru_semaforow, sops_start, 1); // ustaw że ty modyfikujesz teraz tablice



    tablica_zamowien *tablica = shmat(identyfikator_segmentu_pamieci_wspolnej, NULL, 0);
    int indeks = (semctl(identyfikator_zbioru_semaforow, 1, GETVAL, NULL) + 1) % MAX_ZAMOWIEN;
    int wielkosc_zamowienia = losowy_int(1, 1000);
    tablica->wartosci[indeks] = wielkosc_zamowienia;
    shmdt(tablica);


    struct sembuf *sops_koniec = (struct sembuf *) calloc(3, sizeof(struct sembuf));
    sops_koniec[0].sem_num = 0;     // ustaw, że już nie korzystasz
    sops_koniec[0].sem_op = 1;

    sops_koniec[1].sem_num = 1;     // ustaw ostatni zapisany indeks
    if (indeks == 0) {
        sops_koniec[1].sem_op = -MAX_ZAMOWIEN+1;
    } else {
        sops_koniec[1].sem_op = 1;
    }

    sops_koniec[2].sem_num = 3;     //zwieksz ilosc zamowien do przygotowania
    sops_koniec[2].sem_op = 1;
    semop(identyfikator_zbioru_semaforow, sops_koniec, 3);

    printf("[%d %s (pracownik_1)] Dodalem liczbe: %d. Liczba zamowien do przygotowania: %d. Liczba zamowien do wyslania: %d.\n", getpid(), obecny_timestamp(), wielkosc_zamowienia, semctl(identyfikator_zbioru_semaforow, 3, GETVAL, NULL), semctl(identyfikator_zbioru_semaforow, 5, GETVAL, NULL));
    usleep(1000 * wielkosc_zamowienia);
}

int main() {
    signal(SIGINT, wlasny_exit);
    srand(time(NULL) + getpid());

    // stworz tutaj semafory
    klucz_zbioru_semaforow = ftok(getenv("HOME"), 1);
    identyfikator_zbioru_semaforow = semget(klucz_zbioru_semaforow, 0, 0);
    if(identyfikator_zbioru_semaforow < 0) wyjdz_z_bledem("Nie moge otworzyc zbioru semaforow.");
    /*
     * Semafory
     * 0 - czy tablica jest w tym momencie modyfikowana (wartość 0 dla modyfikowana, 1 dla wolna)
     * 1 - ostatni zapisany indeks w tablicy
     * 2 - pierwszy indeks zamowienia do przygotowania
     * 3 - ilosc zamowien do przygotowania
     * 4 - pierwszy indeks zamowienia do wyslania
     * 5 - ilosc zamowien do wyslania
     * */


    klucz_segmentu_pamieci_wspolnej = ftok(getenv("HOME"), 2);
    identyfikator_segmentu_pamieci_wspolnej = shmget(klucz_segmentu_pamieci_wspolnej, 0, 0);
    if(identyfikator_segmentu_pamieci_wspolnej < 0) wyjdz_z_bledem("Nie moge otworzyc segmentu pamieci wspolnej.");

    bool flaga = false;
    while(1){
        usleep(losowy_int(10000, 1000000));
        if( semctl(identyfikator_zbioru_semaforow, 3, GETVAL, NULL) + semctl(identyfikator_zbioru_semaforow, 5, GETVAL, NULL) < MAX_ZAMOWIEN ){
            dodaj_zamowienie();
            flaga = false;
        }
        else{
            if(flaga == false){
                printf("[%d %s (pracownik_1)] Nie moge przyjac wiecej zamowien.\n", getpid(), obecny_timestamp());
                usleep(100000);
                flaga = true;
            }
        }
    }



    semctl(identyfikator_zbioru_semaforow, 0, IPC_RMID, NULL);
    shmctl(identyfikator_segmentu_pamieci_wspolnej, IPC_RMID, NULL);
    return 0;
}