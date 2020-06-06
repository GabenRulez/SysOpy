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

#include "pomocnicze.h"

#define ILOSC_SEMAFOROW 10
//int tablica_zamowien[ILOSC_SEMAFOROW]= {0};
int* tablica_zamowien;

void uruchom_pracownika_1(){
    while(1){
        int wielkosc_zamowienia = losowy_int(1,6);
        printf("buuu");
        exit(1);
    }
}

void uruchom_pracownika_2(){
    while(1){
        printf("buuu");
        exit(1);
    }
}

void uruchom_pracownika_3(){
    while(1){
        printf("buuu");
        exit(1);
    }
}

void wyjdz_z_bledem(char* tekst){
    wypisz_wysrodkowane("--- ERROR ---");
    printf("%s\n\n", tekst);
    exit(1);
}

void wyjdz_poprawnie(char* tekst, pid_t* pid){
    char* temp_tekst = calloc(SZEROKOSC_KONSOLI, sizeof(char));
    sprintf(temp_tekst, "Proces %ls konczy prace.", pid);
    wypisz_wysrodkowane(temp_tekst);
    free(temp_tekst);
}

int main(int argc, char** argv) {
    if (argc < 4) wyjdz_z_bledem("Podaj:  <ile_pracownikow_typu_1> <ile_pracownikow_typu_2> <ile_pracownikow_typu_3>\n");

    int ile_pracownikow_typu_1 = (int) strtol(argv[1], (char **) NULL, 10);
    int ile_pracownikow_typu_2 = (int) strtol(argv[2], (char **) NULL, 10);
    int ile_pracownikow_typu_3 = (int) strtol(argv[3], (char **) NULL, 10);

    if(ile_pracownikow_typu_1 < 1 || ile_pracownikow_typu_2 < 1 || ile_pracownikow_typu_3 < 1) wyjdz_z_bledem("Musi istniec przynajmniej po 1 pracowniku kazdego rodzaju.");

    pid_t* pracownicy_typu_1 = calloc(ile_pracownikow_typu_1, sizeof(pid_t));
    pid_t* pracownicy_typu_2 = calloc(ile_pracownikow_typu_2, sizeof(pid_t));
    pid_t* pracownicy_typu_3 = calloc(ile_pracownikow_typu_3, sizeof(pid_t));

    tablica_zamowien = (int*)calloc(ILOSC_SEMAFOROW, sizeof(int));

    // stworz tutaj semafory
    key_t klucz_zbioru_semaforow = ftok("test_absdjalkfal", 1);
    int identyfikator_zbioru_semaforow = semget(klucz_zbioru_semaforow, ILOSC_SEMAFOROW, IPC_CREAT | 0666);
    for(int i=0; i<ILOSC_SEMAFOROW; i++) semctl(identyfikator_zbioru_semaforow, i, SETVAL, 0);



    for(int i=0; i<ile_pracownikow_typu_1; i++){
        pid_t dziecko = fork();
        if(dziecko == 0){
            uruchom_pracownika_1();
        }
        else{
            pracownicy_typu_1[i] = dziecko;
        }
        usleep(losowy_int(1000, 100000));
    }

    for(int i=0; i<ile_pracownikow_typu_2; i++){
        pid_t dziecko = fork();
        if(dziecko == 0){
            uruchom_pracownika_2();
        }
        else{
            pracownicy_typu_2[i] = dziecko;
        }
        usleep(losowy_int(1000, 100000));
    }

    for(int i=0; i<ile_pracownikow_typu_3; i++){
        pid_t dziecko = fork();
        if(dziecko == 0){
            uruchom_pracownika_3();
        }
        else{
            pracownicy_typu_3[i] = dziecko;
        }
        usleep(losowy_int(1000, 100000));
    }


    free(pracownicy_typu_1);
    free(pracownicy_typu_2);
    free(pracownicy_typu_3);
    free(tablica_zamowien);
    return 0;
}