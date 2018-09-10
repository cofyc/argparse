cmake_minimum_required(VERSION 3.0)
project(argparse)

add_library(${PROJECT_NAME}
    ${PROJECT_SOURCE_DIR}/argparse.c
    ${PROJECT_SOURCE_DIR}/argparse.h
    )

add_executable(test_argparse test_argparse.c)
target_link_libraries(test_argparse ${PROJECT_NAME})
