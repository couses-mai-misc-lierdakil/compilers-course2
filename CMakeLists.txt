cmake_minimum_required(VERSION 2.8)

project(lexer)
file(GLOB SOURCES LIST_DIRECTORIES false *.cpp)
file(GLOB HEADERS LIST_DIRECTORIES false *.h)
set (CMAKE_CXX_STANDARD 17)
add_custom_target(headers ${HEADERS})
add_executable(${PROJECT_NAME} ${SOURCES})
