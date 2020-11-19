project(callback)

set(CMAKE_CXX_STANDARD 17)

file(GLOB SRC ${PROJECT_SOURCE_DIR}/*.cpp)

add_library(${PROJECT_NAME} STATIC ${SRC})
target_include_directories(${PROJECT_NAME} PUBLIC .)
target_link_libraries(${PROJECT_NAME} PRIVATE base_class pthread)
