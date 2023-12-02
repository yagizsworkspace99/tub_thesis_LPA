cmake_minimum_required(VERSION 3.26)
project(tempus)

set(CMAKE_CXX_STANDARD 17)


add_executable(tempus main.cpp
        include/adj_list.cpp)

add_subdirectory(include/libcuckoo/)
add_subdirectory(include/parlaylib/)

target_link_libraries(tempus PUBLIC
        libcuckoo
        parlay
)

target_include_directories(tempus PUBLIC
        include
        include/libcuckoo/libcuckoo
        include/parlaylib/
)