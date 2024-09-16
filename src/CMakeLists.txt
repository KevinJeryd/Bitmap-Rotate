cmake_minimum_required(VERSION 3.16)
project(TerminalBeepLib)

set(INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../lib)
set(SOURCES TerminalBeep.cpp ${INCLUDE_DIR}/TerminalBeep.h)

add_library(${PROJECT_NAME} ${SOURCES})

target_include_directories(${PROJECT_NAME} PRIVATE ${INCLUDE_DIR})