cmake_minimum_required(VERSION 3.16)

set(PROJECT_NAME client)
project(${PROJECT_NAME} LANGUAGES CXX)


set(CMAKE_CXX_STANDARD 17)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

find_package(Qt6 REQUIRED COMPONENTS Widgets Network)

set(SOURCES
    src/main.cpp
    src/MainWindow.cpp
    src/TcpClient.cpp
    src/MessageParser.cpp
)

set(HEADERS
    src/MainWindow.h
    src/INetworkClient.h
    src/TcpClient.h
    src/MessageParser.h
)

set(UI
    src/MainWindow.ui
)

add_executable(${PROJECT_NAME}
    ${SOURCES}
    ${HEADERS}
    ${UI}
)

target_link_libraries(${PROJECT_NAME} PRIVATE Qt6::Widgets Qt6::Network)


