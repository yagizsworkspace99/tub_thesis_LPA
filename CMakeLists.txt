cmake_minimum_required(VERSION 3.26)
project(tempus)

set(CMAKE_CXX_STANDARD 17)


add_executable(tempus main.cpp
        include/adj_list.cpp)

include_directories(tempus PUBLIC
        include
        include/libcuckoo
        include/parlaylib/
        include/parlaylib/include/
        include/parlaylib/include/parlay
)

add_subdirectory(
        include/libcuckoo/
        include/parlaylib/
)