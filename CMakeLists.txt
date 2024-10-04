cmake_minimum_required(VERSION 3.8)
project("MemoryLeak")
#
add_executable (${PROJECT_NAME} "main.c" "etc/color.h")
set_property(TARGET ${PROJECT_NAME} PROPERTY C_STANDARD 11)