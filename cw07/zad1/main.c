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

int ile_pracownikow_typu_1;
int ile_pracownikow_typu_2;
int ile_pracownikow_typu_3;

pid_t* pracownicy_typu_1;
pid_t* pracownicy_typu_2;
pid_t* pracownicy_typu_3;

key_t klucz_zbioru_semaforow;
int identyfikator_zbioru_semaforow;

key_t klucz_segmentu_pamieci_wspolnej;
int identyfikator_segmentu_pamieci_wspolnej;



void pelny_exit(int signum){
    printf("\n\n");
    sleep(1);

    for(int i=0; i<ile_pracownikow_typu_1; i++) kill(pracownicy_typu_1[i], SIGINT);
    for(int i=0; i<ile_pracownikow_typu_2; i++) kill(pracownicy_typu_2[i], SIGINT);
    for(int i=0; i<ile_pracownikow_typu_3; i++) kill(pracownicy_typu_3[i], SIGINT);
    usleep(100000);

    semctl(identyfikator_zbioru_semaforow, 0, IPC_RMID, NULL);
    shmctl(identyfikator_segmentu_pamieci_wspolnej, IPC_RMID, NULL);

    wypisz_wysrodkowane("--- Sklep konczy prace. ---\n");
    exit(EXIT_SUCCESS);
}

int main(int argc, char** argv) {
    if (argc < 4) wyjdz_z_bledem("Podaj:  <ile_pracownikow_typu_1> <ile_pracownikow_typu_2> <ile_pracownikow_typu_3>\n");

    ile_pracownikow_typu_1 = (int) strtol(argv[1], (char **) NULL, 10);
    ile_pracownikow_typu_2 = (int) strtol(argv[2], (char **) NULL, 10);
    ile_pracownikow_typu_3 = (int) strtol(argv[3], (char **) NULL, 10);

    if(ile_pracownikow_typu_1 < 1 || ile_pracownikow_typu_2 < 1 || ile_pracownikow_typu_3 < 1) wyjdz_z_bledem("Musi istniec przynajmniej po 1 pracowniku kazdego rodzaju.");

    pracownicy_typu_1 = calloc(ile_pracownikow_typu_1, sizeof(pid_t));
    pracownicy_typu_2 = calloc(ile_pracownikow_typu_2, sizeof(pid_t));
    pracownicy_typu_3 = calloc(ile_pracownikow_typu_3, sizeof(pid_t));

    signal(SIGINT, pelny_exit);

    // stworz tutaj semafory
    klucz_zbioru_semaforow = ftok(getenv("HOME"), 1);
    identyfikator_zbioru_semaforow = semget(klucz_zbioru_semaforow, 6, IPC_CREAT | 0666);
    /*
     * Semafory
     * 0 - czy tablica jest w tym momencie modyfikowana (wartość 0 dla modyfikowana, 1 dla wolna)
     * 1 - ostatni zapisany indeks w tablicy
     * 2 - pierwszy indeks zamowienia do przygotowania
     * 3 - ilosc zamowien do przygotowania
     * 4 - pierwszy indeks zamowienia do wyslania
     * 5 - ilosc zamowien do wyslania
     * */

    union semun arg;
    arg.val = 0;
    for(int i=0; i<6; i++) semctl(identyfikator_zbioru_semaforow, i, SETVAL, arg);
    arg.val = 1;
    semctl(identyfikator_zbioru_semaforow, 0, SETVAL, arg); // ustawienie semafora 0 na 1
    semctl(identyfikator_zbioru_semaforow, 2, SETVAL, arg);


    klucz_segmentu_pamieci_wspolnej = ftok(getenv("HOME"), 2);
    identyfikator_segmentu_pamieci_wspolnej = shmget(klucz_segmentu_pamieci_wspolnej, sizeof(tablica_zamowien), IPC_CREAT | 0666);

    wypisz_wysrodkowane("--- Otwarty sklep ---");

    for(int i=0; i<ile_pracownikow_typu_1; i++){
        pid_t dziecko = fork();
        if(dziecko == 0){
            execlp("./pracownik_1", "pracownik_1", NULL);
        }
        else{
            pracownicy_typu_1[i] = dziecko;
        }
        usleep(losowy_int(1000, 100000));
    }

    for(int i=0; i<ile_pracownikow_typu_2; i++){
        pid_t dziecko = fork();
        if(dziecko == 0){
            execlp("./pracownik_2", "pracownik_2", NULL);
        }
        else{
            pracownicy_typu_2[i] = dziecko;
        }
        usleep(losowy_int(1000, 100000));
    }

    for(int i=0; i<ile_pracownikow_typu_3; i++){
        pid_t dziecko = fork();
        if(dziecko == 0){
            execlp("./pracownik_3", "pracownik_3", NULL);
        }
        else{
            pracownicy_typu_3[i] = dziecko;
        }
        usleep(losowy_int(1000, 100000));
    }

    for(int i=0; i < ile_pracownikow_typu_1 + ile_pracownikow_typu_2 + ile_pracownikow_typu_3; i++) wait(NULL);


    free(pracownicy_typu_1);
    free(pracownicy_typu_2);
    free(pracownicy_typu_3);

    semctl(identyfikator_zbioru_semaforow, 0, IPC_RMID, NULL);
    shmctl(identyfikator_segmentu_pamieci_wspolnej, IPC_RMID, NULL);
    return 0;
}