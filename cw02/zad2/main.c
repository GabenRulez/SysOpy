/*

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>
#include <time.h>
#include <stdint.h>
#include <fcntl.h>
#include <sys/times.h>
#include <sys/types.h>
#include <limits.h>
#include <ftw.h>

#define max_days 9999

void find_stat(char* sciezka, int mtime, int atime, int maxdepth){
    DIR* folder = opendir(sciezka);
    if(folder == NULL) exit(2);
    system("@echo p");

    struct dirent* plik_w_strumieniu = readdir(folder);
    while(plik_w_strumieniu != NULL){
        char* nazwa_pliku = plik_w_strumieniu->d_name;

        if(strcmp(nazwa_pliku, ".") !=0 && strcmp(nazwa_pliku, "..")!=0){       // być może bezużyteczne

            char* sciezka_pliku = (char*) calloc( strlen(sciezka) + strlen(nazwa_pliku) + 2, sizeof(char) );

            strcpy(sciezka_pliku, sciezka);
            strcat(sciezka_pliku, "/");
            strcat(sciezka_pliku, nazwa_pliku);

            struct stat* statystyka = (struct stat*) malloc(sizeof(stat));
            lstat(sciezka_pliku, statystyka);

            int flag = 1;

            if (flag == 1){
                int dni_od_modyfikacji = (int)( (intmax_t)statystyka->st_mtime / 86400);    // castowanie time_t do int dni
                if (dni_od_modyfikacji > mtime) flag = 0;
            }
            if (flag == 1){
                int dni_od_dostepu = (int)( (intmax_t)statystyka->st_atime / 86400);    // castowanie time_t do int dni
                if (dni_od_dostepu > atime) flag = 0;
            }
            if (flag == 1){
                printf("    Plik %s    ", nazwa_pliku);
                printf("Sciezka: %s", sciezka_pliku);
                printf("Liczba dowiazan: %lu", statystyka->st_nlink);
                printf("Rodzaj pliku: %u", statystyka->st_mode);
                printf("Rozmiar: %ldB", statystyka->st_size);
                printf("Ostatni dostep: %s", asctime(gmtime((const time_t *) statystyka->st_atime)));
                printf("Ostatnia modyfikacja: %s", asctime(gmtime((const time_t *) statystyka->st_mtime)));
                printf(" ");
            }
            if(maxdepth > 0){
                if( S_ISDIR(statystyka->st_mode) ){     // makro z rodziny "stat"
                    find_stat(sciezka_pliku, mtime, atime, maxdepth-1);
                }
            }
            free(statystyka);
            free(sciezka_pliku);

        }

        plik_w_strumieniu = readdir(folder);

    }

    free(plik_w_strumieniu);
    closedir(folder);
};






void find_nftw(char* sciezka, int mtime, int atime, int maxdepth){

};


int main(int argc, char** argv){

    if (argc < 2){
        printf("Podaj:  komendę  <plik2>\n");
        return 1;
    }

    int mtime = max_days;
    int atime = max_days;
    int maxdepth = 0;
    int tryb = 1;

    char* sciezka_temp = argv[1];
    if(sciezka_temp == NULL) return 2;
    char* sciezka = realpath(sciezka_temp, NULL);

    for (int i=2; i<argc;){
        if( strcmp(argv[i], "-mtime") == 0 ){
            if (argc-1==i){
                printf("Brak podanej wartosci argumentu %s.", argv[i]);
                return 2;
            }
            mtime = (int) strtol(argv[i+1], (char**)NULL, 10);
            i = i + 2;
        }

        else if( strcmp(argv[i], "-atime") == 0 ){
            if (argc-1==i){
                printf("Brak podanej wartosci argumentu %s.", argv[i]);
                return 2;
            }
            atime = (int) strtol(argv[i+1], (char**)NULL, 10);
            i = i + 2;
        }

        else if( strcmp(argv[i], "-maxdepth") == 0 ){
            if (argc-1==i){
                printf("Brak podanej wartosci argumentu %s.", argv[i]);
                return 2;
            }
            maxdepth = (int) strtol(argv[i+1], (char**)NULL, 10);
            i = i + 2;
        }

        else if( strcmp(argv[i], "-tryb") == 0 ){
            if (argc-1==i){
                printf("Brak podanej wartosci argumentu %s.", argv[i]);
                return 2;
            }
            tryb = (int) strtol(argv[i+1], (char**)NULL, 10);
            i = i + 2;
        }

        else{
            printf("Nie istnieje taki atrybut '%s'", argv[i]);
            return 2;
        }
    }

    if( tryb == 1) {
        //find_stat(sciezka, mtime, atime, maxdepth);
    }
    else if( tryb == 2) {
        //find_nftw(sciezka, mtime, atime, maxdepth);
    }
    else {
        printf("Nie istnieje taki tryb %d", tryb);
        return 2;
    }

    free(sciezka);
}






/*









































#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <sys/stat.h>
#include <sys/times.h>
#include <sys/types.h>
#include <dirent.h>
#include <stdbool.h>
#include <limits.h>
#include <ftw.h>

struct time_compare {
    int m_time_days;
    int m_time_operator;
    int a_time_days;
    int a_time_operator;
};

const char* date_format = "%Y-%m-%d %H:%M:%S";
char date_string[20];
struct time_compare* tc;
int max_depth;

int day_diff(time_t date) {
    return difftime(time(NULL), date) / 86400;
}

bool is_time_correct(const struct stat* stats) {
    bool is_correct = true;

    if(tc->m_time_days >= 0) {
        int diff = day_diff(stats->st_mtime);
        switch(tc->m_time_operator) {
            case -1:
                is_correct = is_correct && diff < tc->m_time_days;
                break;
            case 0:
                is_correct = is_correct && diff == tc->m_time_days;
                break;
            case 1:
                is_correct = is_correct && diff > tc->m_time_days;
                break;
        }
    }

    if(tc->a_time_days >= 0) {
        int diff = day_diff(stats->st_atime);
        switch(tc->a_time_operator) {
            case -1:
                is_correct = is_correct && diff < tc->a_time_days;
                break;
            case 0:
                is_correct = is_correct &&  diff == tc->a_time_days;
                break;
            case 1:
                is_correct = is_correct &&  diff > tc->a_time_days;
                break;
        }
    }

    return is_correct;
}

char* file_type(mode_t mode) {
    if(S_ISREG(mode))
        return "file";
    if(S_ISDIR(mode))
        return "dir";
    if(S_ISCHR(mode))
        return "char dev";
    if(S_ISBLK(mode))
        return "block dev";
    if(S_ISFIFO(mode))
        return "fifo";
    if(S_ISLNK(mode))
        return "slink";
    if(S_ISSOCK(mode))
        return "sock";
    return "unrecognized";
}

void print_file_info(const char* file_path, const struct stat* stats) {
    printf("file: %s\n", file_path);
    printf("\thardlinks: %lu\n", stats->st_nlink);
    printf("\ttype: %s\n", wypisz_typ_pliku(stats->st_mode));
    printf("\tsize: %ld bytes\n", stats->st_size);
    strftime(date_string, 20, date_format, localtime(&stats->st_atime));
    printf("\tlast access: %s\n", date_string);
    strftime(date_string, 20, date_format, localtime(&stats->st_mtime));
    printf("\tlast modification: %s\n", date_string);
}


static int nftw_callback(const char* file_path, const struct stat* stats, int type_flag, struct FTW* ftw_buf) {
    if(is_time_correct(stats) && (max_depth < 0 || ftw_buf->level <= max_depth)) {
        wypisz_info_o_pliku(file_path, stats);
    }
    return 0;
}

*/