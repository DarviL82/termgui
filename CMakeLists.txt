cmake_minimum_required(VERSION 3.23)
project(termgui)

set(CMAKE_CXX_STANDARD 23)

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/third_party/myhtml)

add_executable(${PROJECT_NAME} src/main.cpp src/html_parser.hpp src/html_parser.cpp)

target_include_directories(${PROJECT_NAME} PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/third_party/myhtml/source)

target_link_libraries(${PROJECT_NAME} PUBLIC myhtml)