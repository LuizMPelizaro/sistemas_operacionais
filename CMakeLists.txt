cmake_minimum_required(VERSION 3.22)
project(atividade_threds C)

set(CMAKE_C_STANDARD 11)

add_executable(atividade_threds.exe main.c allocate_pid.h pids.h)
