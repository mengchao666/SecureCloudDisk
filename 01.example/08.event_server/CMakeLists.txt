cmake_minimum_required (VERSION 3.0)

project (demo)

FIND_LIBRARY(Libevent libevent.a /opt/homebrew/Cellar/libevent/2.1.12/lib/)

add_executable(main main.cpp)

target_link_libraries (main ${Libevent})
