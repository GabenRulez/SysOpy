#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>

void sighandler(int sig_no, siginfo_t * info, void* kontekst){
    printf("Dziecko: otrzymalem od rodzica %d\n\n", info->si_value.sival_int);
}


int main(int argc, char* argv[]) {

    if(argc != 3){
        printf("Not a suitable number of program parameters\n");
        return 1;
    }

    struct sigaction action;
    action.sa_sigaction = &sighandler;

    //..........
    sigset_t maska;
    sigfillset(&maska);
    sigdelset(&maska, SIGUSR1);
    action.sa_flags = SA_SIGINFO;
    action.sa_mask = maska;


    int child = fork();
    if(child == 0) {
        //zablokuj wszystkie sygnaly za wyjatkiem SIGUSR1
        //zdefiniuj obsluge SIGUSR1 w taki sposob zeby proces potomny wydrukowal
        //na konsole przekazana przez rodzica wraz z sygnalem SIGUSR1 wartosc
        sigprocmask(SIG_BLOCK, &maska, NULL);

        sigaction(SIGUSR1, &action, NULL);
        pause();
        return 0;

    }
    else {
        //wyslij do procesu potomnego sygnal przekazany jako argv[2]
        //wraz z wartoscia przekazana jako argv[1]

        sleep(1);   // poczekaj chwilkę, aby wszystko było poprawne (tj. aby dziecko zdążyło zainicjalizować maske)

        int sygnal_int = (int) strtol(argv[1], (char**) NULL, 10);
        int sygnal =  (int) strtol(argv[2], (char**) NULL, 10);

        __sigval_t wartosc;
        wartosc.sival_int = sygnal_int;

        sigqueue(child, sygnal, wartosc);
        printf("Rodzic: Wyslalem sygnal.\n");
    }

    return 0;
}
