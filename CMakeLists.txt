cmake_minimum_required(VERSION 3.5.0)
project(argparse)

SET(SRC_FILES
	argparse.c
	argparse.h
)
   
ADD_LIBRARY(argparse ${SRC_FILES})