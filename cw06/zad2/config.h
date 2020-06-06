#ifndef CONFIG_H
#define CONFIG_H

#define MAX_KLIENTOW 10
const int MAX_MSG_LEN = 100;
const char SERVER_QUEUE_NAME[] = "/testywojtka";
#define NAME_LEN 8

typedef enum m_type {
    STOP = 1, DISCONNECT = 2, INIT = 3, LIST = 4, CONNECT = 5
} m_type;


#define SZEROKOSC_KONSOLI 80
const char* spacja_1 = " ";
const char* spacja_2 = "  ";
const char* spacja_4 = "    ";
const char* spacja_8 = "        ";
const char* spacja_16 = "                ";
const char* gwiazdka_1 = "*";
const char* gwiazdka_2 = "**";
const char* gwiazdka_4 = "****";
const char* gwiazdka_8 = "********";
const char* gwiazdka_16 = "****************";
const char* gwiazdka_32 = "********************************";
const char* gwiazdka_64 = "****************************************************************";




#endif //CONFIG_H
