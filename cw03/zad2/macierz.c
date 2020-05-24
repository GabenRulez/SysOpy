#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct macierz{
    int kolumny;
    int wiersze;
    int szerokosc_komorki;
    char* sciezka_do_pliku;
}; typedef struct macierz macierz;

macierz* init_macierz(int kolumny, int wiersze, int szerokosc_komorki, char* sciezka_do_pliku){
    macierz* temp = (macierz*) malloc(sizeof(macierz));
    temp->kolumny = kolumny;
    temp->wiersze = wiersze;
    temp->szerokosc_komorki = szerokosc_komorki;
    temp->sciezka_do_pliku = calloc(1024, sizeof(char));
    strcpy(temp->sciezka_do_pliku, sciezka_do_pliku);
    return temp;
}

struct plik_lista{
    macierz** macierze_A;
    macierz** macierze_B;
    macierz** macierze_C;
    int ile_macierzy;
}; typedef struct plik_lista plik_lista ;

plik_lista* init_plik_lista(int size){
    plik_lista* temp = (plik_lista*) malloc(sizeof(plik_lista));
    temp->ile_macierzy = size;
    temp->macierze_A = (macierz**) calloc(size, sizeof(macierz*));
    temp->macierze_B = (macierz**) calloc(size, sizeof(macierz*));
    temp->macierze_C = (macierz**) calloc(size, sizeof(macierz*));
    return temp;
}

int ile_kolumn(char* plik){
    FILE* temp = fopen(plik, "r");
    char* pojedynczy_wiersz = calloc(1048576, sizeof(char));
    fgets(pojedynczy_wiersz, 1048576, temp);

    strtok(pojedynczy_wiersz, " ");
    int kolumny = 1;    // jako ze da sie cos przeczytac

    while( strtok(NULL, " ") != NULL ) kolumny++;
    free(pojedynczy_wiersz);
    fclose(temp);
    return kolumny;
}

int ile_wierszy(char* plik){
    FILE* temp = fopen(plik, "r");
    char* wiersz = calloc(1048576, sizeof(char));
    
    int wiersze = 0;
    while( fgets(wiersz, 1048576, temp) != NULL ) wiersze++;

    free(wiersz);
    fclose(temp);
    return wiersze;
}


macierz* stworz_macierz(char* nazwa_pliku, int kolumny, int wiersze){
    FILE* plik = fopen(nazwa_pliku, "w+");
    macierz* temp = init_macierz(kolumny, wiersze, 1024, nazwa_pliku);
    for(int y=0; y<temp->kolumny; y++ ){
        for(int x=0; x<temp->wiersze; x++){
            fwrite(" ", 1, 1, plik);
        }
        fwrite("\n", 1, 1, plik);
    }
    fclose(plik);
    return temp;
}


plik_lista* czytaj_plik_lista(char* nazwa_pliku){
    FILE* plik = fopen(nazwa_pliku, "r");

    char* temp = calloc(3 * 1024, sizeof(char));
    int ile_macierzy = 0;
    while( fgets(temp, 3 * 1024, plik) != NULL ){
        ile_macierzy = ile_macierzy + 1;
    }

    plik_lista* temp_lista = init_plik_lista(ile_macierzy);
    rewind(plik);

    int iterator = 0;
    while( fgets(temp, 3 * 1024, plik) != NULL ){
        char* plik_A = strtok(temp, " ");
        char* plik_B = strtok(NULL, " ");
        char* plik_C = strtok(NULL, "\n");

        macierz* macierz_A = malloc(sizeof(macierz));
        macierz_A->sciezka_do_pliku = calloc(1024, sizeof(char));
        strcpy(macierz_A->sciezka_do_pliku, plik_A);
        macierz_A->kolumny = ile_kolumn(plik_A);
        macierz_A->wiersze = ile_wierszy(plik_A);
        temp_lista->macierze_A[iterator] = macierz_A;

        macierz* macierz_B = malloc(sizeof(macierz));
        macierz_B->sciezka_do_pliku = calloc(1024, sizeof(char));
        strcpy(macierz_B->sciezka_do_pliku, plik_B);
        macierz_B->kolumny = ile_kolumn(plik_B);
        macierz_B->wiersze = ile_wierszy(plik_B);
        temp_lista->macierze_B[iterator] = macierz_B;

        if(macierz_A->kolumny != macierz_B->wiersze){
            printf("Macierzy nie da sie pomnozyc.");
            return 2;
        }

        macierz* macierz_C = stworz_macierz(plik_C, macierz_B->kolumny, macierz_A->wiersze);
        temp_lista->macierze_C[iterator] = macierz_C;

        iterator = iterator + 1;
    }
    free(temp);
    fclose(plik);
    return temp_lista;
}

int main(int argc, char** argv) {
    if (argc < 5) {
        printf("Podaj:  find <plik z lista> <ilosc podprocesow> <ilosc sekund na podproces> <tryb: 0-oddzielne plik / 1-wszystkie w jednym pliku>\n");
        return 1;
    }

    char *sciezka_lista = argv[1];
    char *sciezka_lista_bezwzgledna = realpath(sciezka_lista, NULL);
    int ilosc_podprocesow = (int) strtol(argv[2], (char **) NULL, 10);
    int limit_czasowy = (int) strtol(argv[3], (char**) NULL, 10);
    int tryb = (int) strtol(argv[4], (char**) NULL, 10);

    plik_lista* lista = czytaj_plik_lista(sciezka_lista);


    if( tryb == 0 ){    //oddzielne pliki

    }
    else if (tryb == 1){    // wszystkie do tego samego pliku

    }
    else{
        printf("Nie ma takiego trybu.");
        return 1;
    }


    free(sciezka_lista_bezwzgledna);
    return 0;
}









#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <unistd.h>
#include <sys/file.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/times.h>

enum mode{ NORMAL, PASTE };

struct matrix{
    char* file_name;
    int rows;
    int cols;
    int chars_per_num;
}; typedef struct matrix matrix;


struct list{
    matrix** As;
    matrix** Bs;
    matrix** Cs;
    int len;
}; typedef struct list list;

#define MIN_VAL -100
#define MAX_VAL 100
#define MAX_ROW_LEN 100000
#define LIST_LINE_LEN 100

int get_idx(matrix* M, int r, int c){
    return M->chars_per_num*M->cols*r + c*M->chars_per_num;
}

void count_rows(matrix* M) {
    FILE* f = fopen(M->file_name, "r");
    char str[FILENAME_MAX];
    int i = 0;
    while(fgets(str, sizeof str, f) != NULL) i++;
    M->rows = i;
    fclose(f);
}

void count_columns(matrix* M) {
    FILE* f = fopen(M->file_name, "r");
    char* str = calloc(MAX_ROW_LEN, sizeof(char));
    fgets(str, MAX_ROW_LEN, f);
    int i = 1;
    strtok(str, " ");
    while(strtok(NULL, " ") != NULL) i++;
    M->cols = i;
    free(str);
    fclose(f);
}

int get_val_at(matrix* M, FILE* f, int r, int c){
    rewind(f);
    int i = 0;
    char* str = calloc(MAX_ROW_LEN, sizeof(char));
    while(fgets(str, MAX_ROW_LEN, f) != NULL && i<r && i<M->rows) i++;
    i = 0;
    char* token;
    token = strtok(str, " ");
    while(token != NULL && i<c && i<M->cols){
        token = strtok(NULL, " ");
        i++;
    }
    int ret;
    if(token == NULL){
        printf("Couldn't find given position\n");
        exit(EXIT_FAILURE);
    }
    ret = atoi(token);
    free(str);
    return ret;
}

void fill_file(FILE* f, int rows, int len){
    char* space = " ";
    char* endl = "\n";
    for(int j =0; j<rows; j++){
        for(int i = 0; i<len - 1; i++) {
            fwrite(space, 1, 1, f);
        }
        fwrite(endl, 1, 1, f);
    }
}

void delete_matrix(matrix* M){
    free(M->file_name);
    free(M);
}

matrix* create_matrix(int rows, int cols, char* file_name){
    matrix* C = malloc(sizeof(matrix));
    C->file_name = calloc(FILENAME_MAX, sizeof(char));
    strcpy(C->file_name, file_name);
    C->rows = rows;
    C->cols = cols;
    FILE* f = fopen(C->file_name, "w+");
    C->chars_per_num = (int) log10(abs(MAX_VAL*MIN_VAL)) + 3;	//+1 for sign, +1 because log100=2, +1 for a space after
    int r_len = C->chars_per_num*C->cols;
    fill_file(f, C->rows, r_len);
    fclose(f);
    return C;
}

matrix* read_matrix(char* file_name){
    matrix* M = malloc(sizeof(matrix));
    M->file_name = calloc(FILENAME_MAX, sizeof(char));
    strcpy(M->file_name, file_name);
    count_columns(M);
    count_rows(M);
    return M;
}

void write_in_pos(matrix *M, FILE* f, int r, int c, int val){
    rewind(f);
    int idx = get_idx(M, r, c);
    char* num = (char*) calloc(M->chars_per_num, sizeof(char));
    sprintf(num, "%d", val);
    int j = M->chars_per_num-1;
    while(num[j]==0) num[j--] = ' ';
    fseek(f, idx, 0);
    if(fwrite(num, sizeof(char), M->chars_per_num-1, f) < M->chars_per_num-1){
        printf("error in writing");
    }
    free(num);
}

void multiply_col(matrix* A, matrix* B, matrix* C, FILE* a, FILE* b, FILE* f, int c){
    rewind(a);
    rewind(b);
    rewind(f);
    for(int r = 0; r<A->rows; r++){
        int res = 0;
        for(int i = 0; i<A->cols; i++){
            res+=get_val_at(A, a, r, i)* get_val_at(B, b, i,c);
        }
        write_in_pos(C,f,r,c,res);
    }
}


list* read_list(char* file_name){
    FILE* f = fopen(file_name, "r");
    char* line = calloc(LIST_LINE_LEN, sizeof(char));
    int len = 0;
    while(fgets(line, LIST_LINE_LEN, f) != NULL) len++;
    list* m_list = (list*) malloc(sizeof(list));
    m_list->As = (matrix**) calloc(len, sizeof(matrix*));
    m_list->Bs = (matrix**) calloc(len, sizeof(matrix*));
    m_list->Cs = (matrix**) calloc(len, sizeof(matrix*));
    m_list->len = len;
    rewind(f);
    int i = 0;
    while(fgets(line, LIST_LINE_LEN, f) != NULL){
        char* file_A = strtok(line, " ");
        char* file_B = strtok(NULL, " ");
        char* file_C = strtok(NULL, "\n");
        m_list->As[i] = read_matrix(file_A);
        m_list->Bs[i] = read_matrix(file_B);
        m_list->Cs[i] = create_matrix(m_list->As[i]->rows, m_list->Bs[i]->cols, file_C);
        i++;
    }
    free(line);
    fclose(f);
    return m_list;
}

char* get_file_name(list* m_list, int m_idx, int chunk_idx){
    char* file_name = calloc(FILENAME_MAX, sizeof(char));
    strcpy(file_name, m_list->Cs[m_idx]->file_name);
    char buff[100];
    snprintf(buff, 10, "%d", chunk_idx);
    strcat(file_name, buff);
    return file_name;
}

void create_file_chunk(list* m_list, int m_idx, int chunk_idx, int start_col, int cols_per_proc){
    matrix* A = m_list->As[m_idx];
    matrix* B = m_list->Bs[m_idx];
    FILE* a = fopen(A->file_name, "r");
    FILE* b = fopen(B->file_name,"r");

    matrix* tmp = malloc(sizeof(matrix));

    char* file_name = get_file_name(m_list, m_idx, chunk_idx);
    tmp = create_matrix(m_list->Cs[m_idx]->rows, cols_per_proc, file_name);
    FILE* f = fopen(file_name, "r+");

    for (int i = 0; i<cols_per_proc; i++) {
        for (int r = 0; r < A->rows; r++) {
            int res = 0;
            for (int c = 0; c < A->cols; c++) {
                res += get_val_at(A, a, r, c) * get_val_at(B, b, c, start_col+i);
            }
            write_in_pos(tmp, f, r, i, res);
        }
    }
    delete_matrix(tmp);
    fclose(f);
    free(file_name);
}

void multiply(list* m_list, int proc_idx, int proc_count, int max_sec, enum mode mode, int *m_counter){
    struct tms* start = malloc(sizeof(struct tms));
    struct tms* stop = malloc(sizeof(struct tms));
    double t_start=times(start);
    int i;
    (*m_counter) = 0;
    for(i = 0; i<m_list->len; i++){
        int cols_per_proc = 1;
        int start_col;
        if(proc_idx>=m_list->Bs[i]->cols) continue;
        if(proc_count > m_list->Bs[i]->cols){
            cols_per_proc=1;
            start_col=proc_idx;
        }
        else{
            cols_per_proc = m_list->Bs[i]->cols/proc_count;
            start_col = cols_per_proc*proc_idx;
            if(proc_idx == proc_count -1){
                int rem = m_list->Bs[i]->cols - cols_per_proc*(proc_idx+1);
                cols_per_proc += rem;
            }
        }

        if(mode == PASTE) create_file_chunk(m_list, i, proc_idx, start_col, cols_per_proc);
        else{
            FILE* f = fopen(m_list->Cs[i]->file_name, "r+");
            FILE* a = fopen(m_list->As[i]->file_name, "r");
            FILE* b = fopen(m_list->Bs[i]->file_name, "r");
            flock(fileno(f), LOCK_EX);
            for(int c = start_col; c < start_col+cols_per_proc; c++){
                multiply_col(m_list->As[i], m_list->Bs[i], m_list->Cs[i], a, b, f, c);
            }
            flock(fileno(f), LOCK_UN);
            fclose(a);
            fclose(b);
            fclose(f);
        }
        (*m_counter)++;
        double t_stop = times(stop);
        double t_elapsed = (t_stop - t_start)/sysconf(_SC_CLK_TCK);
        if((int) t_elapsed >= max_sec){
            printf("time's up\n");
            break;
        }
    }
    free(start);
    free(stop);
    return;
}

void exec_paste(list* m_list, int* proc_per_m, int proc_count){
    for(int i =0; i<proc_count; i++) wait(0);

    for(int i = 0; i<m_list->len; i++){
        char** arg = calloc(proc_per_m[i]+2, sizeof(char*));
        arg[0] = (char*) calloc(6, sizeof(char));
        strcpy(arg[0], "paste");
        for(int j=0; j<proc_per_m[i]; j++){
            arg[j+1] = calloc(FILENAME_MAX, sizeof(char));
            strcpy(arg[j+1], get_file_name(m_list, i, j));
        }
        arg[proc_per_m[i]+1]=NULL;
        pid_t child_pid = fork();
        if(child_pid == 0){
            int fd = open(m_list->Cs[i]->file_name, O_WRONLY | O_CREAT | O_TRUNC, 0666);
            dup2(fd, 1);
            close(fd);
            execvp("paste", arg);
            strcpy(arg[0], "rm");
        }
        else{
            wait(0);
        }
        pid_t child_pid2 = fork();
        if(child_pid2 == 0){
            strcpy(arg[0], "rm");
            execvp("rm", arg);
        }
        else{
            wait(0);
        }
        for(int j=0; j<proc_per_m[i]+1; j++) free(arg[j]);
    }
}

int* count_proc_per_m(list* m_list, int proc_count){
    int* proc_per_m = calloc(m_list->len, sizeof(int));

    for(int i = 0; i<m_list->len; i++){
        int cols = m_list->Bs[i]->cols;
        if(cols>=proc_count) proc_per_m[i] = proc_count;
        else proc_per_m[i] = cols;
    }

    return proc_per_m;
}

void create_empty_files(list* m_list, int* proc_per_m){
    for(int m_idx =0; m_idx<m_list->len; m_idx++){
        for(int chunk_idx = 0; chunk_idx<proc_per_m[m_idx]; chunk_idx++){
            char* file_name = get_file_name(m_list, m_idx, chunk_idx);
            creat(file_name, 0666);
            free(file_name);
        }
    }

}

void delete_list(list* m_list){
    for(int i = 0; i< m_list->len; i++){
        delete_matrix(m_list->As[i]);
        delete_matrix(m_list->Bs[i]);
        delete_matrix(m_list->Cs[i]);
    }

    free(m_list->As);
    free(m_list->Bs);
    free(m_list->Cs);
    free(m_list);
}

int main(int argc, char** argv) {

    if(argc<5) {
        printf("Invalid arguments. Expected: list_name num_of_processes max_proc_time NORMAL/PASTE\n");
        exit(EXIT_FAILURE);
    }

    list* m_list = read_list(argv[1]);

    int proc_count=atoi(argv[2]);
    if(proc_count == 0){
        printf("Invalid number of processes\n");
        exit(EXIT_FAILURE);
    }
    int max_time = atoi(argv[3]);

    enum mode mode;
    if(strcmp("PASTE", argv[4]) == 0) mode=PASTE;
    else if(strcmp("NORMAL", argv[4]) == 0) mode=NORMAL;
    else{
        printf("Invalid mode. Available modes: NORMAL, PASTE\n");
        exit(EXIT_FAILURE);
    }

    int *proc_per_m = NULL;
    if(mode == PASTE) {
        proc_per_m = count_proc_per_m(m_list, proc_count);
        create_empty_files(m_list, proc_per_m);
    }

    pid_t* child_pids = calloc(proc_count, sizeof(pid_t));
    for(int i = 0; i<proc_count; i++){
        pid_t child_pid = fork();
        if(child_pid == 0){
            int num_of_fragments;
            multiply(m_list, i, proc_count, max_time, mode, &num_of_fragments);
            exit(num_of_fragments);
        }
        else if (child_pid>0){
            child_pids[i] = child_pid;
        }
        else{
            exit(EXIT_FAILURE);
        }
    }


    int* stat_loc = malloc(sizeof(int));
    for(int i = 0; i<proc_count; i++){
        waitpid(child_pids[i], stat_loc, 0);
        printf("Process of PID %d multiplied %d fragment(s) of matrices\n", child_pids[i], WEXITSTATUS(*stat_loc));
    }

    if(mode == PASTE) exec_paste(m_list, proc_per_m, proc_count);


    delete_list(m_list);

    free(stat_loc);
    return 0;

}