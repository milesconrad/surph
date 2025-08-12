cmake_minimum_required(VERSION 3.20)
project(surph)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY build/)
file(GLOB MY_PROJECT_SOURCES "src/*.cpp")
add_executable(${CMAKE_PROJECT_NAME} ${MY_PROJECT_SOURCES})

set(SFML_BUILD_AUDIO OFF CACHE BOOL "" FORCE)
add_subdirectory(submodules/sfml)

target_include_directories(${CMAKE_PROJECT_NAME} PRIVATE submodules/sfml/include/)
target_link_libraries(
    ${CMAKE_PROJECT_NAME}
    sfml-graphics
    sfml-window
    sfml-system
    pthread
)