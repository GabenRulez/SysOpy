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

int ile_pracownikow_typu_1;
int ile_pracownikow_typu_2;
int ile_pracownikow_typu_3;

pid_t* pracownicy_typu_1;
pid_t* pracownicy_typu_2;
pid_t* pracownicy_typu_3;

sem_t* semafory[6];
int deskryptor_pamieci_wspolnej;


void pelny_exit(int signum){
    printf("\n\n");
    sleep(1);

    for(int i=0; i<ile_pracownikow_typu_1; i++) kill(pracownicy_typu_1[i], SIGINT);
    for(int i=0; i<ile_pracownikow_typu_2; i++) kill(pracownicy_typu_2[i], SIGINT);
    for(int i=0; i<ile_pracownikow_typu_3; i++) kill(pracownicy_typu_3[i], SIGINT);
    usleep(100000);

    for(int i=0; i < ile_pracownikow_typu_1 + ile_pracownikow_typu_2 + ile_pracownikow_typu_3; i++) wait(NULL);

    for(int i=0; i<6; i++){
        int temp = sem_unlink(SEMAFORY[i]);
        if(temp == -1) wyjdz_z_bledem("Nie udalo sie usunacc semafora.");
    }

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
    for(int i=0; i<6; i++){
        semafory[i] = sem_open(SEMAFORY[i], O_RDWR | O_CREAT, S_IRWXU | S_IRWXG | S_IRWXO, (i+1)%2);        // akurat semafory 0, 2 i 4 mają mieć 1 na start
        if(semafory[i] == SEM_FAILED) wyjdz_z_bledem("Nie mozna stworzyc semafora.");
    }

    for(int i=0; i<6; i++){
        int temp = sem_close(semafory[i]);
        if(temp == -1) wyjdz_z_bledem("Nie udalo sie zamknac semafora.");
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
    if(deskryptor_pamieci_wspolnej == -1) wyjdz_z_bledem("Nie udalo sie utworzyc pamieci wspolnej");
    if( ftruncate(deskryptor_pamieci_wspolnej, sizeof(tablica_zamowien)) == -1 ) wyjdz_z_bledem("Nie udalo sie ustawic rozmiaru pamieci wspolnej");


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
    for(int i=0; i<6; i++){
        int temp = sem_unlink(SEMAFORY[i]);
        if(temp == -1) wyjdz_z_bledem("Nie udalo sie usunac semafora.");
    }

    return 0;
}