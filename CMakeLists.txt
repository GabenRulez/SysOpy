cmake_minimum_required(VERSION 3.15)
project(sysopy C)

set(CMAKE_C_STANDARD 11)

add_executable(sysopy Kol1-2020/ForkAndPipe/main.c Kol1-2020/SignalAllButOneWithValue/main.c)