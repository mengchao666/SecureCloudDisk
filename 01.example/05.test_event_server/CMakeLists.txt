cmake_minimum_required (VERSION 3.0)

project (demo)

add_executable(main main.cpp)

target_link_libraries (main "/usr/local/lib/libevent.so")
