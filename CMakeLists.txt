cmake_minimum_required(VERSION 3.20)
project(surph)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY src/)

link_directories(/usr/lib/SFML)

add_executable(${CMAKE_PROJECT_NAME} src/main.cpp)
target_link_libraries(${CMAKE_PROJECT_NAME} sfml-graphics sfml-window sfml-system pthread)
