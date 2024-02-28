cmake_minimum_required(VERSION 3.0)
project(pdf_reader)

set(SOURCES
    src/main.cpp
    src/pdf/pdf_reader.cpp
    src/pdf/pdf_tokenizer.cpp
)
set(HEADERS
    include/pdf/pdf_reader.h
    include/pdf/pdf_tokenizer.h
)

include_directories(include)

add_executable(pdf_reader ${SOURCES} ${HEADERS})
