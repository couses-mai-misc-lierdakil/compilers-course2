cmake_minimum_required(VERSION 3.0)

project(calc)

find_package(BISON)

ADD_CUSTOM_COMMAND(OUTPUT lex.yy.cpp
COMMAND ${CMAKE_CURRENT_SOURCE_DIR}/3rdparty/re-flex/bin/reflex --header-file ${CMAKE_CURRENT_SOURCE_DIR}/lexer.l
DEPENDS lexer.l parser.tab.cc)
ADD_CUSTOM_COMMAND(OUTPUT parser.tab.cc
COMMAND ${BISON_EXECUTABLE} -v -d ${CMAKE_CURRENT_SOURCE_DIR}/parser.y
DEPENDS parser.y)


file(GLOB SOURCES LIST_DIRECTORIES false *.cpp)
file(GLOB REFLEX_SOURCES LIST_DIRECTORIES false 3rdparty/re-flex/lib/*.cpp 3rdparty/re-flex/unicode/*.cpp)
set (CMAKE_CXX_STANDARD 17)
add_executable(${PROJECT_NAME} ${SOURCES} ${REFLEX_SOURCES} parser.tab.cc lex.yy.cpp)
target_include_directories(${PROJECT_NAME} PRIVATE
${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_BINARY_DIR}
${CMAKE_CURRENT_SOURCE_DIR}/3rdparty/re-flex/include)
