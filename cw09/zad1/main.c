#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t condition = PTHREAD_COND_INITIALIZER;

struct salon{
    pthread_t* golibroda;
    pthread_t* klient_na_krzesle;
    pthread_t** krzesla_w_poczekalni;
    int ilosc_krzesel_w_poczekalni;
    int oczekujacy_klienci;
};

int golibroda_spi = 0;


int losowy_int(int min, int max){
    return min + rand() % (max - min);
}


struct salon* salon_fryzjerski;

void* golibroda(){
    pthread_mutex_lock(&mutex);

    while(salon_fryzjerski->oczekujacy_klienci == 0){
        printf("Golibroda: Idę spać. \n");
        golibroda_spi = 1;
        pthread_cond_wait(&condition, &mutex);
    }

    golibroda_spi = 0;  // skoro tu przeszedłem, to znaczy ze ktos mnie obudzil
    pthread_t* temp_klient;

    for(int i=0; i < salon_fryzjerski->ilosc_krzesel_w_poczekalni; i++){
        if( salon_fryzjerski->krzesla_w_poczekalni[i] == NULL ) continue;

        temp_klient = salon_fryzjerski->krzesla_w_poczekalni[i];
        printf("Biore klienta %ld na krzeslo. \n", temp_klient);
        salon_fryzjerski->krzesla_w_poczekalni[i] = NULL;
        salon_fryzjerski->oczekujacy_klienci--;
        break;
    }

    salon_fryzjerski->klient_na_krzesle = temp_klient;

    pthread_mutex_unlock(&mutex);

    sleep(losowy_int(10,30));
    printf("Obsluzylem klienta %ld.\n", temp_klient);

    pthread_mutex_lock(&mutex);
    if(salon_fryzjerski->oczekujacy_klienci > 0){
        pthread_mutex_unlock(&mutex);
        golibroda();
    }
    else{
        pthread_mutex_unlock(&mutex);
    }
    pthread_exit(0);
};

void* klient(){
    pthread_t ID = pthread_self();
    pthread_mutex_lock(&mutex);
    int flag = 0;
    for(int i=0; i<salon_fryzjerski->ilosc_krzesel_w_poczekalni; i++){  //zajęcie krzesła
        if( salon_fryzjerski->krzesla_w_poczekalni[i] == NULL ){
            salon_fryzjerski->krzesla_w_poczekalni[i] = &ID;
            flag = 1;
            break;
        }
    }
    if( flag == 0 ){
        pthread_mutex_unlock(&mutex);
        printf("Klient %d: Nie ma miejsc. Jeszcze tu wroce.\n", ID);
        sleep(losowy_int(20,30));
        klient();
    }
    // tu przeszedł już klient, który usiadł na krzesle

    if(golibroda_spi == 0){     // budzimy
        printf("Klient %d: Ach, ten golibroda zasnal. Ide go obudzic.\n", ID);
        pthread_cond_broadcast(&condition);
        pthread_mutex_unlock(&mutex);
    }
    else{
        int temp2 = 0;
        for(int i=0; i<salon_fryzjerski->ilosc_krzesel_w_poczekalni; i++){
            if(  salon_fryzjerski->krzesla_w_poczekalni[i] == NULL ) temp2++;
        }
        printf("Klient %d: Jestem w poczekalni, a jest %d wolnych miejsc.\n", ID, temp2);
        pthread_mutex_unlock(&mutex);
    }

    pthread_exit(0);
};


int main(int argc, char** argv){
    if (argc<3){
        printf("Zbyt malo argumentow. Podaj <liczba_krzesel> <liczba_klientow>\n");
        return 1;
    }
    int liczba_krzesel = (int) strtol(argv[1], (char**)NULL, 10);
    int liczba_klientow = (int) strtol(argv[2], (char**)NULL, 10);

    salon_fryzjerski = (struct salon*) malloc(sizeof(struct salon));      // tworze salon, ktory przechowuje informacje o golibrodzie, kliencie na krzesle, o krzeslach w poczekalni


    salon_fryzjerski->golibroda = (pthread_t*) malloc(sizeof(pthread_t));
    salon_fryzjerski->klient_na_krzesle = NULL;
    salon_fryzjerski->ilosc_krzesel_w_poczekalni = liczba_krzesel;
    salon_fryzjerski->krzesla_w_poczekalni = (pthread_t**) calloc(liczba_krzesel, sizeof(pthread_t*));
    for(int i=0; i<salon_fryzjerski->ilosc_krzesel_w_poczekalni; i++) salon_fryzjerski->krzesla_w_poczekalni[i] = NULL;
    salon_fryzjerski->oczekujacy_klienci = 0;

    printf("Jestem golibroda.\n");
    pthread_create(salon_fryzjerski->golibroda, NULL, golibroda(), NULL);

    printf("Jestem tutaj");

    pthread_t* wszyscy_klienci = (pthread_t*) calloc(liczba_klientow, sizeof(pthread_t));
    for(int i=0; i<liczba_klientow; i++){
        printf("Jestem klientem.\n");
        pthread_create(&wszyscy_klienci[i], NULL, klient(), NULL);
        //usleep(,);
    }

    void* wartosci_zwracane;
    pthread_join(salon_fryzjerski->golibroda[0], &wartosci_zwracane);
    for(int i = 0; i< liczba_klientow; i++){
        pthread_join(wszyscy_klienci[i], &wartosci_zwracane);
    }

    pthread_mutex_destroy(&mutex);
    pthread_cond_destroy(&condition);

}