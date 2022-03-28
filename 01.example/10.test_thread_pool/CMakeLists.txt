cmake_minimum_required (VERSION 3.0)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)

add_compile_options(-std=c++11)

set(SRC_LIST  ${CMAKE_CURRENT_SOURCE_DIR}/src/XThreadPool.cpp 
              ${CMAKE_CURRENT_SOURCE_DIR}/src/XThread.cpp 
              ${CMAKE_CURRENT_SOURCE_DIR}/src/XServerTask.cpp)
              
project (demo)

FIND_LIBRARY(Libevent libevent.dylib /opt/homebrew/Cellar/libevent/2.1.12/lib/)

add_library (xcom SHARED ${SRC_LIST})

target_link_libraries (xcom ${Libevent})
