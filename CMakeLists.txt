cmake_minimum_required(VERSION 3.16)
project(TestTask LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

# Опция тестов
option(BUILD_TESTS "Build tests" ON)

add_subdirectory(server)
add_subdirectory(client)
