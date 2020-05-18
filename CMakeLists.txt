cmake_minimum_required(VERSION 3.15)
project(sysopy C)

set(CMAKE_C_STANDARD 11)

add_executable(sysopy main.c cw01/zad1/myLibrary.c cw01/zad2/main.c cw01/zad2/myLibrary.c cw01/zad3a/main.c cw01/zad3a/main_dynamic.c)