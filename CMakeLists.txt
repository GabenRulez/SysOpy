cmake_minimum_required(VERSION 3.15)
project(sysopy C)

set(CMAKE_C_STANDARD 11)

add_executable(sysopy cw10/zad1/klient.c cw10/zad1/serwer.c cw10/zad1/pomocnicze.h)