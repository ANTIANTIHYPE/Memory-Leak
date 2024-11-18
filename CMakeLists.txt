cmake_minimum_required(VERSION 3.10 FATAL_ERROR)
project(MemoryLeak VERSION 1.0.1 LANGUAGES C)

include_directories("etc")

add_executable(${PROJECT_NAME} "main.c")

set_property(TARGET ${PROJECT_NAME} PROPERTY C_STANDARD 11)
set_property(TARGET ${PROJECT_NAME} PROPERTY C_STANDARD_REQUIRED YES)