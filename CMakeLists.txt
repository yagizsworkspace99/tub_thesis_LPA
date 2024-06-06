cmake_minimum_required(VERSION 3.26)
project(tempus)

set(CMAKE_CXX_STANDARD 17)


add_executable(tempus main.cpp
        include/adj_list.cpp
        include/seed_selection.cpp
        include/seed_selection.h
        include/LPA.cpp
        include/LPA.h
        include/oriented_graph.cpp
        include/oriented_graph.h

        include/NMI.cpp include/NMI.h include/ModularityDensity.cpp include/ModularityDensity.h)


include_directories(tempus PUBLIC
        include
        include/libcuckoo
        include/parlaylib/
        include/parlaylib/include/
        include/parlaylib/include/parlay
        )

add_subdirectory(include/libcuckoo/)
add_subdirectory(Google_tests)
add_subdirectory(include/parlaylib/)

target_link_libraries(Google_Tests_run gtest gtest_main)

