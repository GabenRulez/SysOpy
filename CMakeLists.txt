cmake_minimum_required(VERSION 3.15)
project(sysopy C)

set(CMAKE_C_STANDARD 11)

add_executable(sysopy cw08/zad1/main.c cw08/zad1/funkcje_tekstowe.h cw08/zad1/pomocnicze.h)