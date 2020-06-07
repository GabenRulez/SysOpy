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

sem_t* semafory[6];
int deskryptor_pamieci_wspolnej;


int wartosc_semafora(int indeks){
    int wartosc;
    sem_getvalue(semafory[indeks], &wartosc);
    return wartosc;
}

void wlasny_exit(int signum){
    usleep(100000);
    printf("[%d %s (pracownik_1)] Zamykaja sklep. Koncze prace.\n", getpid(), obecny_timestamp());

    for(int i=0; i<6; i++){
        int temp = sem_close(semafory[i]);
        if(temp == -1) wyjdz_z_bledem("Nie udalo sie zamknac semafora.");
    }
    exit(EXIT_SUCCESS);
}

void dodaj_zamowienie() {
    sem_wait(semafory[0]); // ustaw że ty modyfikujesz teraz tablice



    tablica_zamowien *tablica = mmap(NULL, sizeof(tablica_zamowien), PROT_READ | PROT_WRITE, MAP_SHARED, deskryptor_pamieci_wspolnej, 0);
    if(tablica == (void*)-1) wyjdz_z_bledem("Nie moge zmapowac pamieci wspolnej.");

    int indeks = (wartosc_semafora(1) + 1) % MAX_ZAMOWIEN;
    int wielkosc_zamowienia = losowy_int(1, 1000);
    tablica->wartosci[indeks] = wielkosc_zamowienia;

    if( munmap(tablica, sizeof(tablica_zamowien)) == -1 ) wyjdz_z_bledem("Nie moge odlaczyc pamieci wspolnej.");



    if( indeks == MAX_ZAMOWIEN - 1 ){   // ustaw ostatni zapisany indeks
        for(int i=0; i<MAX_ZAMOWIEN - 1; i++){
            sem_trywait(semafory[1]);
        }
    }
    else{
        sem_post(semafory[1]);
    }
    sem_post(semafory[3]);          //zwieksz ilosc zamowien do przygotowania
    sem_post(semafory[0]);          // ustaw, że już nie korzystasz


    printf("[%d %s (pracownik_1)] Dodalem liczbe: %d. Liczba zamowien do przygotowania: %d. Liczba zamowien do wyslania: %d.\n", getpid(), obecny_timestamp(), wielkosc_zamowienia, wartosc_semafora(3), wartosc_semafora(5));
    usleep(1000 * wielkosc_zamowienia);
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
        if( wartosc_semafora(3) + wartosc_semafora(5) < MAX_ZAMOWIEN ){
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


    for(int i=0; i<6; i++){
        int temp = sem_close(semafory[i]);
        if(temp == -1) wyjdz_z_bledem("Nie udalo sie zamknac semafora.");
    }
    return 0;
}