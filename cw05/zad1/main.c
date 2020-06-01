#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>

#define max_dlugosc_linijki 10000
#define max_komend_w_linii 100

int main(int argc, char** argv){
    if (argc < 2) {
        printf("Podaj:  main <plik z lista polecen>\n");
        return 1;
    }

    FILE* plik = fopen(argv[1], "r");
    char* zawartosc_linijki = (char*)calloc(max_dlugosc_linijki, sizeof(char));

    while( fgets(zawartosc_linijki, max_dlugosc_linijki, plik) != NULL ){   // dla każdej linijki z pliku

        char*** wszystkie_komendy = (char***)calloc(max_komend_w_linii, sizeof(char**));  // wskaźnik na tablice tablic argumentów
        
        char* pointer = strtok(zawartosc_linijki, "|");
        int i = 0;
        while(pointer != NULL){
            wszystkie_komendy[i] = (char**)calloc(15, sizeof(char*));
            char* argument = strtok(pointer, " ");
            
            int j = 0;
            while(argument != NULL){
                wszystkie_komendy[i][j] = (char*)calloc(25, sizeof(char));
                strcpy(wszystkie_komendy[i][j], argument);
                argument = strtok(NULL, " ");
                j++;
            }
            i++;
            pointer = strtok(NULL, "|");
        };
        
        pid_t* podprocesy = (pid_t*)calloc(max_komend_w_linii, sizeof(pid_t));
        
        int fd[2];
        pipe(fd);


        pid_t pierwsze_dziecko = fork();
        if(pierwsze_dziecko == 0) {
            dup2(fd[1], STDOUT_FILENO);
            close(fd[0]);
            close(fd[1]);
            execvp(wszystkie_komendy[0][0], wszystkie_komendy[0]);  // argument nr0 to nazwa funkcji
        }
        podprocesy[0] = pierwsze_dziecko;

        int fd_przed[2];
        fd_przed[0] = fd[0];
        fd_przed[1] = fd[1];
        
        int k = 1;
        while(wszystkie_komendy[k] != NULL) {
            fd_przed[0] = fd[0]; 
            fd_przed[1] = fd[1];
            pipe(fd);
            pid_t dziecko = fork();

            if(dziecko == 0) {
                dup2(fd_przed[0], STDIN_FILENO);
                dup2(fd[1], STDOUT_FILENO);
                
                close(fd_przed[0]); // muszę wszystkie zamykać, bo w tym miejscu są stworzone dwa potoki
                close(fd_przed[1]);
                close(fd[0]);
                close(fd[1]);
                
                execvp(wszystkie_komendy[k][0], wszystkie_komendy[k]);
            }
            close(fd_przed[0]);
            close(fd_przed[1]);
            podprocesy[k] = dziecko;
            k++;
        }


        for(int l = 0; l < k; l++){
            waitpid(podprocesy[l], NULL, 0);    // poczekaj na wszystkie dzieci jak w poleceniu
        }

        close(fd[0]);
        close(fd[1]);   // zamknij te potoki w procesie matki

        free(podprocesy);
        free(wszystkie_komendy);
    }
    free(zawartosc_linijki);
    fclose(plik);
    return 0;
}