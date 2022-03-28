cmake_minimum_required (VERSION 3.0)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../01.example/10.test_thread_pool/include)

add_compile_options(-std=c++11)

set(SRC_LIST  ${CMAKE_CURRENT_SOURCE_DIR}/xdisk_server.cpp)

project (demo)

FIND_LIBRARY(Libevent libevent.a /opt/homebrew/Cellar/libevent/2.1.12/lib/)

FIND_LIBRARY(Libxcom libxcom.dylib ../01.example/10.test_thread_pool/build/)

add_executable(main ${SRC_LIST})

target_link_libraries (main ${Libevent} ${Libxcom})