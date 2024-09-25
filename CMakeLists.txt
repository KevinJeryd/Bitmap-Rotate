cmake_minimum_required(VERSION 3.16)

project(BitmapRotate)

set(CMAKE_CXX_STANDARD 20)

add_executable(${PROJECT_NAME} main.cpp)

target_link_libraries(${PROJECT_NAME} BitmapRotateLib)

target_include_directories(${PROJECT_NAME} PRIVATE ${INCLUDE_DIR})

add_subdirectory(src)