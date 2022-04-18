cmake_minimum_required(VERSION 3.21)
project(dsj)

set(CMAKE_CXX_STANDARD 17)

add_executable(dsj main.cpp DESEncryption.cpp DESEncryption.h test.cpp test.h)
