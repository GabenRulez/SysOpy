cmake_minimum_required(VERSION 3.15)
project(sysopy C)

set(CMAKE_C_STANDARD 11)

add_executable(sysopy cw05/zad3/main.c cw05/zad3/producent.c cw05/zad3/konsument.c)