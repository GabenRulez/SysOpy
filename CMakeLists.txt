cmake_minimum_required(VERSION 3.15)
project(sysopy C)

set(CMAKE_C_STANDARD 11)

add_executable(sysopy cw07/zad2/main.c cw07/zad2/pomocnicze.h cw07/zad2/pracownik_1.c cw07/zad2/pracownik_2.c cw07/zad2/pracownik_3.c)