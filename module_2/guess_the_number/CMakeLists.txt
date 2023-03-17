cmake_minimum_required(VERSION 3.24)
project(guess_the_number C)

set(CMAKE_C_STANDARD 11)

add_executable(guess_the_number main.c reader.c reader.h)
