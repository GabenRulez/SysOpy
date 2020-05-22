#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

struct salon{
    pthread_t* golibroda;
    pthread_t** krzesla_w_poczekalni;
    int ilosc_krzesel_w_poczekalni;
};

void* utworz_golibrode(){
    printf("Jestem golibroda.");

};

void* utworz_klienta(){
    printf("Jestem klientem.");
};


int main(int argc, char** argv){
    if (argc<3){
        printf("Zbyt malo argumentow. Podaj <liczba_krzesel> <liczba_klientow>");
        return 1;
    }
    int liczba_krzesel = (int) strtol(argv[1], (char**)NULL, 10);
    int liczba_klientow = (int) strtol(argv[2], (char**)NULL, 10);

    struct salon* salon_fryzjerski = (struct salon*) malloc(sizeof(struct salon));

    salon_fryzjerski->golibroda = (pthread_t*) malloc(sizeof(pthread_t));
    pthread_create(salon_fryzjerski->golibroda, NULL, utworz_golibrode(), NULL);

    salon_fryzjerski->ilosc_krzesel_w_poczekalni = liczba_krzesel;
    salon_fryzjerski->krzesla_w_poczekalni = (pthread_t**) calloc(liczba_krzesel, sizeof(pthread_t*));

    pthread_t* wszyscy_klienci = (pthread_t*) calloc(liczba_klientow, sizeof(pthread_t));
    for(int i=0; i<liczba_klientow; i++){
        pthread_create(wszyscy_klienci[i], NULL, utworz_klienta(), NULL);
        //usleep(,);
    }






}