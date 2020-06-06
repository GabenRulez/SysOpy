#ifndef CONFIG_H
#define CONFIG_H

#define MAX_KLIENTOW 10
#define ID_SERWERA 1

typedef struct msg_buf {
    long m_type;
    char m_text[1024];
    key_t klucz_kolejki;
    int ID_klienta;
    int ID_polaczonego_klienta;
} msg_buf;

typedef enum m_type {
    STOP = 1, DISCONNECT = 2, INIT = 3, LIST = 4, CONNECT = 5
} m_type;

const int ROZMIAR_BUFORA = sizeof(msg_buf) - sizeof(long);

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
