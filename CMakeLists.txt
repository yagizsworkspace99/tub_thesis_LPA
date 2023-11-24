cmake_minimum_required(VERSION 3.26)
project(tempus)

set(CMAKE_CXX_STANDARD 17)


add_executable(tempus main.cpp
        include/adj_list.cpp)

include_directories(tempus PUBLIC
        include
        include/libcuckoo
)

add_subdirectory(
        include/libcuckoo/libcuckoo
)