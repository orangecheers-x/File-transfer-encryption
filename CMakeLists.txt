cmake_minimum_required(VERSION 3.21)
project(dsj)

set(CMAKE_CXX_STANDARD 14)

add_executable(dsj main.cpp DESEncryption.cpp DESEncryption.h)
