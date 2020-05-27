#include <signal.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>

void obsluga_sygnalu(int sig_no){
    printf("Obsluguje wlasnie sygnal %d.\n", sig_no);
}

int main(int argc, char**argv){
    if (argc<3){
        printf("Zbyt malo argumentow. \n");
        return 1;
    }

    // argv[1] - fork lub exec
    // argv[2] - ignore, handler, mask lub pending

    if( strcmp(argv[1], "exec") == 0 && strcmp(argv[2], "handler") == 0){
        printf("\nOpcja handler jest niedostepna dla exec. \n");
        exit(1);
    }

    if( strcmp(argv[1], "exec") != 0 && strcmp(argv[1], "fork") != 0 ){
        printf("\nBledny argument: %s.\nDostepne opcje to fork i exec.\n", argv[1]);
        exit(1);
    }

    if( strcmp(argv[2], "ignore") != 0 &&  strcmp(argv[2], "handler") != 0 && strcmp(argv[2], "mask") != 0 && strcmp(argv[2], "pending") != 0){
        printf("\nBledny argument: %s.\nDostepne opcje to ignore, handler, mask i pending.\n", argv[2]);
        exit(1);
    }



    if( strcmp(argv[2], "ignore") == 0 ){
        signal(SIGUSR1, SIG_IGN);   // ignorowanie sygnalu
    }

    if( strcmp(argv[2], "handler") == 0 ){
        signal(SIGUSR1, obsluga_sygnalu);   // instalacja handlera
    }

    if( strcmp(argv[2], "mask") == 0 || strcmp(argv[2], "pending") == 0 ){
        sigset_t maska;
        sigemptyset(&maska);
        sigaddset(&maska, SIGUSR1);
        sigprocmask(SIG_SETMASK, &maska, NULL);
    }

    raise(SIGUSR1);

    sigset_t nowa_maska;
    if( strcmp(argv[2], "mask") == 0 || strcmp(argv[2], "pending") == 0){
        sigpending(&nowa_maska);
        printf("Oczekuje na sygnal: %d\n", sigismember(&nowa_maska, SIGUSR1));
    }

    if( strcmp(argv[1], "fork") == 0 ){
        pid_t dziecko = fork();
        if(dziecko == 0){
            if( strcmp(argv[2], "pending") != 0 ){
                raise(SIGUSR1);
            }
            if( strcmp(argv[2], "mask") == 0 || strcmp(argv[2], "pending") == 0){
                sigpending(&nowa_maska);
                printf("Oczekuje na sygnal: %d\n", sigismember(&nowa_maska, SIGUSR1));
            }
        }
    }
    else if( strcmp(argv[1], "exec") == 0 ){
        printf("tu");
        execl("./exec", "./exec", argv[2], NULL );
    }
    else{
        printf("Nie istnieje taki tryb.\n");
        exit(1);
    }
    wait(0);

return 0;
}