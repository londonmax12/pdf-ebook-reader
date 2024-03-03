cmake_minimum_required(VERSION 3.0)
project(pdf_reader)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(SOURCES
    src/main.cpp
    src/pdf/pdf_file.cpp
    src/pdf/pdf_reader.cpp
    src/pdf/pdf_tokenizer.cpp
    src/pdf/pdf_tokens.cpp
    src/pdf/pdf_parser.cpp
)
set(HEADERS
    include/pdf/pdf_file.h
    include/pdf/pdf_reader.h
    include/pdf/pdf_tokenizer.h
    include/pdf/pdf_tokens.h
    include/pdf/pdf_parser.h
)

include_directories(include)

add_executable(pdf_reader ${SOURCES} ${HEADERS})
