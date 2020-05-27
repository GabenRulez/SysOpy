#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <string.h>

int main(int argc, char** argv){
    sigset_t nowa_maska;
    if( strcmp(argv[1], "pending") != 0 ){
        raise(SIGUSR1);
    }
    if( strcmp(argv[1], "mask") == 0 || strcmp(argv[1], "pending") == 0){
        sigpending(&nowa_maska);
        printf("Oczekuje na sygnal: %d\n", sigismember(&nowa_maska, SIGUSR1));
    }
    return 0;
}