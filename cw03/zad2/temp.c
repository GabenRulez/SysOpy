


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








void write_in_pos(matrix *M, FILE* f, int r, int c, int val){
    rewind(f);
    int idx = M->chars_per_num*M->cols*r + c*M->chars_per_num;
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