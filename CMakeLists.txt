cmake_minimum_required(VERSION 4.0.0)
project(quickTRM)
add_executable(${PROJECT_NAME} main.c libs/string.c)
install(TARGETS quickTRM DESTINATION bin)
