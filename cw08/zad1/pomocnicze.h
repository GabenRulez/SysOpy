#ifndef POMOCNICZE_H
#define POMOCNICZE_H

#include <sys/time.h>
#include <time.h>

int losowy_int(int min, int max){
    return min + rand() % (max - min);
}

char* obecny_timestamp(){
    struct timeval tv;
    gettimeofday(&tv, NULL);

    struct tm *ts = gmtime(&tv.tv_sec);
    char* godzinka = (char*)calloc(80, sizeof(char));
    char* temp = (char*)calloc(4, sizeof(char));
    if(ts->tm_hour >= 10){
        sprintf(temp, "%d:", ts->tm_hour);
        strcat(godzinka, temp);
    }
    else{
        sprintf(temp, "0%d:", ts->tm_hour);
        strcat(godzinka, temp);
    }

    if(ts->tm_min >= 10){
        sprintf(temp, "%d:", ts->tm_min);
        strcat(godzinka, temp);
    }
    else{
        sprintf(temp, "0%d:", ts->tm_min);
        strcat(godzinka, temp);
    }

    if(ts->tm_sec >= 10){
        sprintf(temp, "%d.", ts->tm_sec);
        strcat(godzinka, temp);
    }
    else{
        sprintf(temp, "0%d:", ts->tm_sec);
        strcat(godzinka, temp);
    }

    if(tv.tv_usec/1000 >= 100){
        sprintf(temp, "%ld", tv.tv_usec/1000);
        strcat(godzinka, temp);
    }
    else if(tv.tv_usec/1000 >= 10){
        sprintf(temp, "0%ld", tv.tv_usec/1000);
        strcat(godzinka, temp);
    }
    else{
        sprintf(temp, "00%ld", tv.tv_usec/1000);
        strcat(godzinka, temp);
    }

    free(temp);
    return godzinka;
}

int sufit(int licznik, int mianownik){
    return licznik/mianownik + (licznik % mianownik != 0);
}

#endif //POMOCNICZE_H



