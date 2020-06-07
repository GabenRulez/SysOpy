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
    printf("[%d - Pracownik_2] Zamykaja sklep. Koncze prace.\n", getpid());

    semctl(identyfikator_zbioru_semaforow, 0, IPC_RMID, NULL);
    shmctl(identyfikator_segmentu_pamieci_wspolnej, IPC_RMID, NULL);
    exit(EXIT_SUCCESS);
}

void pakuj_zamowienie() {
    struct sembuf *sops_start = (struct sembuf *) calloc(2, sizeof(struct sembuf));
    sops_start[0].sem_num = 0;
    sops_start[0].sem_op = -1;

    sops_start[1].sem_num = 3;     //zmniejsz ilosc zamowien do przygotowania
    sops_start[1].sem_op = -1;
    semop(identyfikator_zbioru_semaforow, sops_start, 2); // ustaw że ty modyfikujesz teraz tablice


    tablica_zamowien *tablica = shmat(identyfikator_segmentu_pamieci_wspolnej, NULL, 0);
    int indeks = semctl(identyfikator_zbioru_semaforow, 2, GETVAL, NULL);
    int wielkosc = tablica->wartosci[indeks] * 2;
    tablica->wartosci[indeks] = wielkosc;
    shmdt(tablica);


    struct sembuf *sops_koniec = (struct sembuf *) calloc(3, sizeof(struct sembuf));
    sops_koniec[0].sem_num = 0;     // ustaw, że już nie korzystasz
    sops_koniec[0].sem_op = 1;

    sops_koniec[1].sem_num = 2;     // ustaw ostatni zapisany indeks
    if (indeks == MAX_ZAMOWIEN-1) {
        sops_koniec[1].sem_op = -indeks;
    } else {
        sops_koniec[1].sem_op = 1;
    }

    sops_koniec[2].sem_num = 5;     //zwieksz ilosc zamowien do wyslania
    sops_koniec[2].sem_op = 1;
    semop(identyfikator_zbioru_semaforow, sops_koniec, 3);

    printf("[%d %ld] Przygotowalem zamowienie o wielkosci: %d. Liczba zamowien do przygotowania: %d. Liczba zamowien do wyslania: %d.\n", getpid(), time(NULL), wielkosc, semctl(identyfikator_zbioru_semaforow, 3, GETVAL, NULL), semctl(identyfikator_zbioru_semaforow, 5, GETVAL, NULL));
    usleep(1000 * wielkosc);
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
        if( semctl(identyfikator_zbioru_semaforow, 3, GETVAL, NULL) > 0 && semctl(identyfikator_zbioru_semaforow, 0, GETVAL, NULL) == 1 ){
            pakuj_zamowienie();
            flaga = false;
        }
        else{
            if(flaga == false){
                printf("[%d - Pracownik_2] Nie ma zamowien do spakowania.\n", getpid());
                usleep(100000);
                flaga = true;
            }
        }
    }

    /*
        printf("\n%d\n", semctl(identyfikator_zbioru_semaforow, 0, GETVAL, NULL));
    printf("%d\n", semctl(identyfikator_zbioru_semaforow, 1, GETVAL, NULL));
    printf("%d\n", semctl(identyfikator_zbioru_semaforow, 2, GETVAL, NULL));
    printf("%d\n", semctl(identyfikator_zbioru_semaforow, 3, GETVAL, NULL));
    printf("%d\n", semctl(identyfikator_zbioru_semaforow, 4, GETVAL, NULL));
    printf("%d\n", semctl(identyfikator_zbioru_semaforow, 5, GETVAL, NULL));
     */


    semctl(identyfikator_zbioru_semaforow, 0, IPC_RMID, NULL);
    shmctl(identyfikator_segmentu_pamieci_wspolnej, IPC_RMID, NULL);
    return 0;
}