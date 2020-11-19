project(functional_programming)

set(CMAKE_CXX_STANDARD 17)

file(GLOB SRC ${PROJECT_SOURCE_DIR}/*.cpp)

add_library(${PROJECT_NAME} STATIC ${SRC})
target_include_directories(${PROJECT_NAME} PUBLIC .)
