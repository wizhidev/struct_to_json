cmake_minimum_required(VERSION 3.8)
project(struct_json)

set(CMAKE_C_STANDARD 99)

set(SOURCE_FILES main.c src/s2j.c src/cJSON.c inc/s2j.h inc/s2jdef.h inc/cJSON.h)
add_executable(struct_json ${SOURCE_FILES})