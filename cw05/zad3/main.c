#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <wait.h>

int main(int argc, char** argv) {

    char* nazwa_potoku = "temp_potok";
    mkfifo(nazwa_potoku, 0666);

    char* pliki[] = {"pliki_wejsciowe/a.txt", "pliki_wejsciowe/b.txt", "pliki_wejsciowe/c.txt"};
    const int ilosc_plikow = 3;     // ilość plików na górze
    pid_t podprocesy[ilosc_plikow + 1];

    for(int i=0; i<ilosc_plikow; i++){
        podprocesy[i] = fork();
        if( podprocesy[i] == 0 ){
            execl("./producent", "./producent", nazwa_potoku, pliki[i], "10", NULL);    // po 10 znaków na raz
        }
    }

    podprocesy[ilosc_plikow] = fork();
    if( podprocesy[ilosc_plikow] == 0 ){
        execl("./konsument", "./konsument", nazwa_potoku, "wynik.txt", "10", NULL );
    }

    for(int i = 0; i<=ilosc_plikow; i++){
        waitpid(podprocesy[i], NULL, 0);
    }

    return 0;
}
