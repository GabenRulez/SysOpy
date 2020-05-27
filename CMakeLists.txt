cmake_minimum_required(VERSION 3.15)
project(sysopy C)

set(CMAKE_C_STANDARD 11)

add_executable(sysopy cw04/zad4b/sender.c cw04/zad4b/catcher.c)