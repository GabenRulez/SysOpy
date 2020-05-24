#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <sys/stat.h>
#include <dirent.h>
#define __USE_XOPEN_EXTENDED 1
#include <ftw.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>

#define biggest_int 32767
#define longest_path 1024

int mtime;      // ilosc dni od ostatniej modyfikacji
int mtime_znak; // znak, jaka operacje porownania wykonujemy
int atime;      //ilosc dni od ostatniego dostepu
int atime_znak;
int maxdepth_global;    // to dla podfunkcji nftw
char* sciezka_bezwzgledna_global;           // to do wyswieltania wzglednej sciezki przy ls


char *strremove(char *str, const char *sub){
    char *p, *q, *r;
    if ((q = r = strstr(str, sub)) != NULL){
        size_t len = strlen(sub);
        while ((r = strstr(p = r + len, sub)) != NULL){
            while (p < r){
                *q++ = *p++;
            }
        }
        while ((*q++ = *p++) != '\0')
            continue;
    }
    return str;
}

int czy_pasuje_czasowo(const struct stat* statystyka) {
    int correct_values = 0;

    if(mtime >= 0) {
        int diff = difftime(time(NULL), (statystyka->st_mtime)) / 86400;
        if(mtime_znak == -1  &&  diff <  mtime) correct_values++;
        if(mtime_znak == 0   &&  diff == mtime) correct_values++;
        if(mtime_znak == 1   &&  diff >  mtime) correct_values++;
    }

    if(atime >= 0) {
        int diff = difftime(time(NULL), (statystyka->st_atime)) / 86400;
        if(atime_znak == -1 && diff <  atime) correct_values++;
        if(atime_znak == 0  && diff == atime) correct_values++;
        if(atime_znak == 1  && diff >  atime) correct_values++;
    }
    if((mtime>=0 && atime>=0 && correct_values==2) || (mtime>=0 && atime<0 && correct_values==1) || (mtime<0 && atime>=0 && correct_values==1) || (mtime<0 && atime<0 && correct_values==0)){
        return 0;       // wszystkie przypadki, gdzie spelnione sa wszystkie podane warunki czasowe
    };
    return -1;
}


void wypisz_info_o_pliku(const char* sciezka_pliku, const struct stat* statystyka) {
    const int wielkosc_bufora = 20;
    char bufor[wielkosc_bufora];

    printf("\nPlik: %s", sciezka_pliku);
    printf("\nLiczba dowiazan: %lu", statystyka->st_nlink);

    char* temp;
    if(S_ISREG(statystyka->st_mode))
        temp = "plik";
    else if(S_ISDIR(statystyka->st_mode))
        temp = "folder";
    else temp = "inny";
    printf("\nTyp: %s", temp);

    printf("\nRozmiar: %ldB", statystyka->st_size);

    strftime(bufor, wielkosc_bufora, "%Y_%m_%d-%H:%M", localtime(&statystyka->st_atime));
    printf("\nOstatni dostep: %s", bufor);
    strftime(bufor, wielkosc_bufora, "%Y_%m_%d-%H:%M", localtime(&statystyka->st_mtime));
    printf("\nOstatnia modyfikacja: %s\n", bufor);
}

void find_stat(char* sciezka, int maxdepth) {   // w tym znaczeniu maxdepth rozumiane jest na ile poziomow ta instancja findera moze zejsc w dol
    DIR* folder = opendir(sciezka);
    if(folder == NULL) exit(2);

    struct dirent* plik_w_strumieniu = readdir(folder);

    while(plik_w_strumieniu != NULL) {
        char* nazwa_pliku = plik_w_strumieniu->d_name;

        if( strcmp(nazwa_pliku, ".." ) != 0 && strcmp(nazwa_pliku, ".") !=0 ) {  // nie chcemy pliku nadrzednego, gdyz nie znajduje sie on w poddrzewie tego pliku
            char* sciezka_pliku = (char*)calloc(strlen(sciezka) + strlen(nazwa_pliku) + 2, sizeof(char));
            strcpy(sciezka_pliku, sciezka);
            strcat(sciezka_pliku, "/");
            strcat(sciezka_pliku, nazwa_pliku);

            struct stat* statystyka = (struct stat*)malloc(sizeof(struct stat));
            lstat(sciezka_pliku, statystyka);

            if(czy_pasuje_czasowo(statystyka) == 0) {
                wypisz_info_o_pliku(sciezka_pliku, statystyka);
            }

            if(S_ISDIR(statystyka->st_mode) && maxdepth > 0 ) {
                pid_t parent_pid = getpid();
                printf("\n--------------------\n");
                printf("Proces dziecka\n");
                char temp_string[longest_path];
                strcpy(temp_string, sciezka_pliku);
                printf("Sciezka: %s\n", strremove(temp_string, sciezka_bezwzgledna_global));
                printf("Proces wywolujacy: %d\n", parent_pid);
                printf("       \n");

                pid_t child_pid = fork();
                if(child_pid!=0){
                    waitpid(child_pid, NULL, 0);
                    wait(child_pid);
                    find_stat(sciezka_pliku, maxdepth-1);
                }
                else{
                    execlp("ls", "ls", "-l", sciezka_pliku);
                }




            }

            free(statystyka);
            free(sciezka_pliku);
        }

        plik_w_strumieniu = readdir(folder);
    }

    free(plik_w_strumieniu);
    closedir(folder);
}

static int podfunkcja_nftw(const char* sciezka_pliku, const struct stat* statystyka, int flaga, struct FTW* bufor_ftw) {
// funkcja, ktora podaje do nftw (bedzie wywolana dla kazdego elementu w drzewie)
    if(czy_pasuje_czasowo(statystyka) == 0 && bufor_ftw->level <= maxdepth_global + 1) {
        wypisz_info_o_pliku(sciezka_pliku, statystyka);
    }
    return 0;
}


int main(int argc, char** argv) {
    int maxdepth = 0;
    maxdepth_global = maxdepth;
    atime = -biggest_int;
    mtime = -biggest_int;
    char* tryb = "stat";

    if (argc < 2){
        printf("Podaj:  find <folder> [-mtime -atime -maxdepth]\n");
        return 1;
    }

    char* sciezka = argv[1];    // na ten moment nie wiemy, czy sciezka jest wzgledna czy nie
    for(int i = 2; i < argc;) {
        if(strcmp(argv[i], "-mtime") == 0) {
            if (argc-1==i){
                printf("Brak podanej wartosci argumentu %s.", argv[i]);
                return 2;
            }

            char znak = argv[i + 1][0];
            if (znak=='+') mtime_znak = 1;
            else if (znak=='-') mtime_znak = -1;
            else mtime_znak = 0;

            mtime = (int) strtol(argv[i+1], (char**)NULL, 10);
            if (mtime<0) mtime = mtime * (-1);
            i += 2;
        }
        else if(strcmp(argv[i], "-atime") == 0) {
            if (argc-1==i){
                printf("Brak podanej wartosci argumentu %s.", argv[i]);
                return 2;
            }

            char znak = argv[i + 1][0];
            if (znak=='+') atime_znak = 1;
            else if (znak=='-') atime_znak = -1;
            else atime_znak = 0;

            atime = (int) strtol(argv[i+1], (char**)NULL, 10);
            if (atime<0) atime = atime * (-1);
            i += 2;
        }
        else if(strcmp(argv[i], "-maxdepth") == 0) {
            if (argc-1==i){
                printf("Brak podanej wartosci argumentu %s.", argv[i]);
                return 2;
            }

            maxdepth = (int) strtol(argv[i+1], (char**)NULL, 10);
            maxdepth_global = maxdepth; // na potrzeby nftw
            i += 2;
        }
        else if(strcmp(argv[i], "-tryb") == 0) {
            if (argc-1==i){
                printf("Brak podanej wartosci argumentu %s.", argv[i]);
                return 2;
            }
            tryb = argv[i+1];
            i = i + 2;
        }

        else {
            printf("Nie istnieje argument %s", argv[i]);
            return 3;
        }
    }

    char* sciezka_bezwzgledna = realpath(sciezka, NULL);
    sciezka_bezwzgledna_global = sciezka_bezwzgledna;

    if ( strcmp( tryb, "stat" ) == 0 ){
        find_stat(sciezka_bezwzgledna, maxdepth );
    }
    else if ( strcmp( tryb, "nftw" ) == 0 ) {
        nftw(sciezka_bezwzgledna, podfunkcja_nftw, 20, FTW_PHYS | FTW_DEPTH );
    }
    else {
        printf( "Nie istnieje tryb %s", tryb );
    }

    free(sciezka_bezwzgledna);
    //free(sciezka_bezwzgledna_global);
    return 0;
}