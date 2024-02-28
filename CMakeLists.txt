cmake_minimum_required(VERSION 3.0)
project(pdf_reader)

file(GLOB SOURCES "src/*.cpp")

file(GLOB HEADERS "include/*.h")

include_directories(include)

add_executable(pdf_reader ${SOURCES} ${HEADERS})
