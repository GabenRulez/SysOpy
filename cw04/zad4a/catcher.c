#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <string.h>

int sygnal_1;
int sygnal_2;

int otrzymane_sygnaly = 0;

int czy_to_queue = 0;

void akcja_sygnalu(int sygnal, siginfo_t* info, void* kontekst){
    if(sygnal == sygnal_1){
        otrzymane_sygnaly++;
    }
    else if(sygnal == sygnal_2){
        if(czy_to_queue == 1){
            union sigval sigval;
            sigval.sival_int = otrzymane_sygnaly;
            for(int i=0; i<otrzymane_sygnaly; i++){
                sigqueue(info->si_pid, sygnal_1, sigval);
            }
            sigqueue(info->si_pid, sygnal_2, sigval);
        }
        else{
            for(int i=0; i<otrzymane_sygnaly; i++){
                kill(info->si_pid, sygnal_1);
            }
            kill(info->si_pid, sygnal_2);
        }

        printf("Koncze prace. Otrzymalem i wyslalem %d sygnalow.\n", otrzymane_sygnaly);
        exit(0);
    }
}

int main(int argc, char** argv){    // kill/sigqueue/sigrt
    if(argc != 2){
        printf("Zbyt malo argumentow.\n");
        return 1;
    }


    if(strcmp(argv[1], "sigrt") == 0){
        sygnal_1 = SIGRTMIN;
        sygnal_2 = SIGRTMIN+1;
    }
    else{
        sygnal_1 = SIGUSR1;
        sygnal_2 = SIGUSR2;
    }

    if(strcmp(argv[1], "queue") == 0){
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

    printf("\nUruchomienie catchera o PID %d.\n", getpid());

    while(1){
        usleep(1);
    }
    return 0;
}