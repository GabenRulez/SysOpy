cmake_minimum_required(VERSION 3.15)
project(sysopy C)

set(CMAKE_C_STANDARD 11)

add_executable(sysopy cw07/zad1/main.c cw07/zad1/pomocnicze.h cw07/zad1/pracownik_1.c cw07/zad1/pracownik_2.c cw07/zad1/pracownik_3.c)