cmake_minimum_required(VERSION 3.21)
project(dsj)

set(CMAKE_CXX_STANDARD 17)


add_executable(dsj main.cpp DESEncryption.cpp DESEncryption.h DHKeyExchange.cpp DHKeyExchange.h Sender.cpp Sender.h Receiver.cpp Receiver.h)

include_directories("/usr/local/include")
target_link_libraries(dsj "/usr/local/lib/libgmp.a")