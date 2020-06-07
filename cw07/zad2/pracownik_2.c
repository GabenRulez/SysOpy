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

#include <semaphore.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/mman.h>

#include "pomocnicze.h"

void wyjdz_z_bledem(char* tekst){
    wypisz_wysrodkowane("--- ERROR ---");
    printf("%s\n\n", tekst);
    exit(1);
}

sem_t* semafory[6];
int deskryptor_pamieci_wspolnej;


int wartosc_semafora(int indeks){
    int wartosc;
    sem_getvalue(semafory[indeks], &wartosc);
    return wartosc;
}

void wlasny_exit(int signum){
    usleep(100000);
    printf("[%d %s (pracownik_2)] Zamykaja sklep. Koncze prace.\n", getpid(), obecny_timestamp());

    for(int i=0; i<6; i++){
        int temp = sem_close(semafory[i]);
        if(temp == -1) wyjdz_z_bledem("Nie udalo sie zamknac semafora.");
    }
    exit(EXIT_SUCCESS);
}

void pakuj_zamowienie() {
    sem_wait(semafory[0]);          // ustaw że ty modyfikujesz teraz tablice
    sem_wait(semafory[3]);          //zmniejsz ilosc zamowien do przygotowania


    tablica_zamowien *tablica = mmap(NULL, sizeof(tablica_zamowien), PROT_READ | PROT_WRITE, MAP_SHARED, deskryptor_pamieci_wspolnej, 0);
    if(tablica == (void*)-1) wyjdz_z_bledem("Nie moge zmapowac pamieci wspolnej.");

    int indeks = wartosc_semafora(2);
    int wielkosc = tablica->wartosci[indeks] * 2;
    tablica->wartosci[indeks] = wielkosc;


    if( munmap(tablica, sizeof(tablica_zamowien)) == -1 ) wyjdz_z_bledem("Nie moge odlaczyc pamieci wspolnej.");



    if( indeks == MAX_ZAMOWIEN - 1 ){   // ustaw ostatni zapisany indeks
        for(int i=0; i<MAX_ZAMOWIEN - 1; i++){
            sem_trywait(semafory[2]);
        }
    }
    else{
        sem_post(semafory[2]);
    }

    sem_post(semafory[5]);          //zwieksz ilosc zamowien do wyslania
    sem_post(semafory[0]);          // ustaw, że już nie korzystasz


    printf("[%d %s (pracownik_2)] Przygotowalem zamowienie o wielkosci: %d. Liczba zamowien do przygotowania: %d. Liczba zamowien do wyslania: %d.\n", getpid(), obecny_timestamp(), wielkosc, wartosc_semafora(3), wartosc_semafora(5));
    usleep(1000 * wielkosc);
}

int main() {
    signal(SIGINT, wlasny_exit);
    srand(time(NULL) + getpid());

    // stworz tutaj semafory
    for(int i=0; i<6; i++){
        semafory[i] = sem_open(SEMAFORY[i], O_RDWR);
        if(semafory[i] == SEM_FAILED) wyjdz_z_bledem("Nie mozna otworzyc semafora.");
    }

    /*
     * Semafory
     * 0 - czy tablica jest w tym momencie modyfikowana (wartość 0 dla modyfikowana, 1 dla wolna)
     * 1 - ostatni zapisany indeks w tablicy
     * 2 - pierwszy indeks zamowienia do przygotowania
     * 3 - ilosc zamowien do przygotowania
     * 4 - pierwszy indeks zamowienia do wyslania
     * 5 - ilosc zamowien do wyslania
     * */


    deskryptor_pamieci_wspolnej = shm_open(PAMIEC_WSPOLNA, O_RDWR | O_CREAT, S_IRWXU | S_IRWXG | S_IRWXO);
    if(deskryptor_pamieci_wspolnej == -1) wyjdz_z_bledem("Nie udalo sie otworzyc pamieci wspolnej");


    bool flaga = false;
    while(1){
        usleep(losowy_int(10000, 1000000));
        if( wartosc_semafora(3) > 0 && wartosc_semafora(0) == 1 ){
            pakuj_zamowienie();
            flaga = false;
        }
        else{
            if(flaga == false){
                printf("[%d %s (pracownik_2)] Nie ma zamowien do spakowania.\n", getpid(), obecny_timestamp());
                usleep(100000);
                flaga = true;
            }
        }
    }


    for(int i=0; i<6; i++){
        int temp = sem_close(semafory[i]);
        if(temp == -1) wyjdz_z_bledem("Nie udalo sie zamknac semafora.");
    }
    return 0;
}