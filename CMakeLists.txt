cmake_minimum_required(VERSION 3.15)
project(sysopy C)

set(CMAKE_C_STANDARD 11)

add_executable(sysopy Kol2-2020/Semafory/semafor.c Kol2-2020/InetSocket/main.c)