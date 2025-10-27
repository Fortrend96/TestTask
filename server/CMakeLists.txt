cmake_minimum_required(VERSION 3.16)
set(PROJECT_NAME server)
project(${PROJECT_NAME} LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)

add_executable(${PROJECT_NAME}
    src/main.cpp
    src/ServerApp.cpp
    src/ServerApp.h
    src/TcpServer.cpp
    src/TcpServer.h
    src/ClientSession.cpp
    src/ClientSession.h
    src/IntQueue.cpp
    src/IntQueue.h
    src/Sorter.cpp
    src/Sorter.h
)

target_include_directories(${PROJECT_NAME}
    PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

add_subdirectory(tests)
