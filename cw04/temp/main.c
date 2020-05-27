#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <stdlib.h>

void obsluga(int sig_no){
    printf("Obsluga sygnalu %d\n", sig_no);
    exit(0);
}

int main(){
    struct sigaction act;
    act.sa_handler = obsluga;
    sigemptyset(&act.sa_mask);
    act.sa_flags = 0;

    sigaction(SIGINT, &act, NULL);


    while(1){
        printf("Yo\n");
        sleep(1);
    }
}