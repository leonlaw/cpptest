cmake_minimum_required(VERSION 3.0)
project(cpptest)

set(CMAKE_CXX_STANDARD 17)

add_executable(${PROJECT_NAME} main.cpp)
target_link_libraries(${PROJECT_NAME} PUBLIC callback functional_programming base_class)

add_subdirectory(base_class)
add_subdirectory(callback)
add_subdirectory(functional_programming)
