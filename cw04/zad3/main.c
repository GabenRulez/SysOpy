#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <string.h>
#include <sys/wait.h>

void segfault(int sygnal, siginfo_t* info, void* kontekst){
    printf("\nNaruszenie ochrony pamieci. Sygnal: %d", info->si_signo);
    printf("\nProces: %d.", info->si_pid);
    printf("\nAdres: %p", info->si_addr);
    printf("\nKod: %d", info->si_code);
    printf("\n\n");
    exit(1);
}

void sigint(int sygnal, siginfo_t* info, void* kontekst){
    printf("\nSIGINT. Sygnal: %d", info->si_signo);
    printf("\nProces: %d.", info->si_pid);
    printf("\nKod: %d", info->si_code);
    printf("\nBand event: %ld", info->si_band);
    printf("\nUzytkownik: %d", info->si_uid);
    printf("\n\n");
    exit(1);
}

void queue(int sygnal, siginfo_t* info, void* kontekst){
    printf("\nQueue. Sygnal: %d", info->si_signo);
    printf("\nProces: %d.", info->si_pid);
    printf("\nWartosc: %d", info->si_int);
    printf("\nBand event: %ld", info->si_band);
    printf("\nUzytkownik: %d", info->si_uid);
    printf("\n\n");
    exit(1);
}


int main(int argc, char** argv){
    if (argc<2){
        printf("Zbyt malo argumentow. \n");
        return 1;
    }

    struct sigaction action;
    action.sa_flags = SA_SIGINFO;
    sigemptyset(&action.sa_mask);

    if( strcmp(argv[1], "segfault") == 0 ){
        action.sa_sigaction = segfault;
        sigaction(SIGSEGV, &action, NULL);

        int* macierz = NULL;
        macierz[23414] = 420;
    }
    else if( strcmp(argv[1], "sigint") == 0 ){
        action.sa_sigaction = sigint;
        sigaction(SIGINT, &action, NULL);

        union sigval sigval;
        sigqueue(getpid(), SIGINT, sigval);
    }
    else if( strcmp(argv[1], "queue") == 0 ){
        action.sa_sigaction = queue;
        sigaction(SIGUSR1, &action, NULL);

        union sigval sigval;
        sigval.sival_int = 69;
        sigqueue(getpid(), SIGUSR1, sigval);
    }

}