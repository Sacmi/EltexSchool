cmake_minimum_required(VERSION 3.24)
project(rb_tree C)

set(CMAKE_C_STANDARD 11)

add_executable(rb_tree main.c rb_tree.c rb_tree.h)
