#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <string.h>

int sygnal_1;
int sygnal_2;

int ilosc_sygnalow = 0; // wyslane sygnaly
int otrzymane_sygnaly = 0;

int czy_to_queue = 0;

void akcja_sygnalu(int sygnal, siginfo_t* info, void* kontekst){
    if(sygnal == sygnal_1){
        otrzymane_sygnaly++;
        if(czy_to_queue == 1){
            printf("%d\n", info->si_value.sival_int);
        }
    }
    else if(sygnal == sygnal_2){
        printf("Otrzymalem %d sygnalow, a powinienm byl otrzymac %d.\n", otrzymane_sygnaly, ilosc_sygnalow);
        exit(0);
    }
}

int main(int argc, char** argv){    // pid_catchera ilosc_sygnalow kill/sigqueue/sigrt
    if(argc != 4){
        printf("Zbyt malo argumentow.\n");
        return 1;
    }

    pid_t catcher = (int) strtol(argv[1], (char**) NULL, 10);
    ilosc_sygnalow = (int) strtol(argv[2], (char**) NULL, 10);


    if(strcmp(argv[3], "sigrt") == 0){
        sygnal_1 = SIGRTMIN;
        sygnal_2 = SIGRTMIN+1;
    }
    else{
        sygnal_1 = SIGUSR1;
        sygnal_2 = SIGUSR2;
    }

    if(strcmp(argv[3], "queue") == 0){
        czy_to_queue = 1;
    }

    sigset_t maska;
    sigfillset(&maska);
    sigdelset(&maska, sygnal_1);
    sigdelset(&maska, sygnal_2);            // mają być zablokowane wszystkie sygnaly oprocz tych dwoch
    sigprocmask(SIG_BLOCK, &maska, NULL);

    struct sigaction action;
    action.sa_flags = SA_SIGINFO;

    sigemptyset(&action.sa_mask);
    sigaddset(&action.sa_mask, sygnal_1);
    sigaddset(&action.sa_mask, sygnal_2);

    action.sa_sigaction = akcja_sygnalu;

    sigaction(sygnal_1, &action, NULL);
    sigaction(sygnal_2, &action, NULL);

    printf("\nUruchomienie sendera.\n");

    if( strcmp(argv[3], "sigrt") == 0 || strcmp(argv[3], "kill") == 0 ){
        for(int i = 0; i<ilosc_sygnalow; i++){
            kill(catcher, sygnal_1);
        }
        kill(catcher, sygnal_2);
    }
    else if( strcmp(argv[3], "queue") == 0 ){
        union sigval sigval;
        sigval.sival_int = 0;
        for(int i = 0; i<ilosc_sygnalow; i++){
            sigqueue(catcher, sygnal_1, sigval);
        }
        sigqueue(catcher, sygnal_2, sigval);
    }
    else{
        printf("Nie ma takiego trybu.\n");
        return 1;
    }

    printf("Wyslalem sygnaly. Czekam...\n");
    while(1){
        usleep(1);
    }
    return 0;
}