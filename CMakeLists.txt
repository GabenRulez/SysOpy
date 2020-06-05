cmake_minimum_required(VERSION 3.15)
project(sysopy C)

set(CMAKE_C_STANDARD 11)

add_executable(sysopy cw06/zad1/serwer.c cw06/zad1/klient.c)