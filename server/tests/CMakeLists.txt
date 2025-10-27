cmake_minimum_required(VERSION 3.16)
project(TestTask_Tests LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Пути к исходникам сервера
include_directories(${CMAKE_SOURCE_DIR}/server)

# Добавляем тестовый исполняемый файл
add_executable(tests
    test_queue.cpp
    test_sorter.cpp
    ${CMAKE_SOURCE_DIR}/server/src/IntQueue.cpp
    ${CMAKE_SOURCE_DIR}/server/src/Sorter.cpp
)

find_package(GTest REQUIRED)
target_link_libraries(tests PRIVATE GTest::GTest GTest::Main pthread)

# Регистрация тестов для CTest
include(GoogleTest)
gtest_discover_tests(tests)
