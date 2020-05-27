#include <signal.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

#define fps 5
double sleep_time_seconds = 1.0 / fps;
int flaga_spania = 0;

void odbior_SIGTSTP(){
    if(flaga_spania == 0){
        printf("   - Oczekuje na CTRL+Z - kontynuacja albo CTRL+C - zakonczenie programu. \n");
        flaga_spania = 1;
/*
        sigset_t sigs;
        sigemptyset(&sigs);
        sigaddset(&sigs, SIGINT);
        sigsuspend(&sigs);*/
    }
    else{
        printf("   - Odebrano sygnal SIGTSTP (CTRL+Z) - kontynuacja. \n");
        flaga_spania = 0;
    }
};

void odbior_SIGINT(){
    printf("   - Odebrano sygnal SIGINT\n");
    exit(0);
}

int main(int argc, char**argv){
    struct sigaction sig_tstp;
    sig_tstp.sa_handler = odbior_SIGTSTP;
    sig_tstp.sa_flags = 0;
    sigemptyset(&sig_tstp.sa_mask);     // przygotowanie "akcji"

    sigaction(SIGTSTP, &sig_tstp, NULL);

    signal(SIGINT, odbior_SIGINT);

    usleep(sleep_time_seconds );
    while(1){
        if(flaga_spania == 0) system("ls");
        usleep(sleep_time_seconds * 1000000 );
    }

return 0;
}