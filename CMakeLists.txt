cmake_minimum_required(VERSION 4.3)

project(LexicalAnalyzer LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_executable(LexicalAnalyzer src/main.cpp)

target_include_directories(LexicalAnalyzer PRIVATE include)

target_sources(LexicalAnalyzer PRIVATE src/analyzer.cpp)

add_compile_options(/utf-8)