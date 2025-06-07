cmake_minimum_required(VERSION 3.28)
project(app1)

set(CMAKE_CXX_STANDARD 20)

file(GLOB SOURCES "*.cpp" "*.h")

add_executable(app1 ${SOURCES})
