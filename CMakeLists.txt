cmake_minimum_required(VERSION 3.17)
project(language_guess)

set(CMAKE_CXX_STANDARD 17)

add_executable(language_guess main.cpp LanguageProcessor.cpp LanguageProcessor.h)

configure_file(data.txt "${CMAKE_BINARY_DIR}/data.txt")
configure_file(input.txt "${CMAKE_BINARY_DIR}/input.txt")
