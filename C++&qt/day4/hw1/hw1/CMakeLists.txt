cmake_minimum_required(VERSION 3.10)
project(project111 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

find_package(Qt5 REQUIRED COMPONENTS Core Gui Widgets Network)

# ui 파일 경로
set(CMAKE_AUTOUIC_SEARCH_PATHS ${CMAKE_CURRENT_SOURCE_DIR}/ui)

add_executable(hw1
    src/main.cpp
    src/mainwindow.cpp
    include/mainwindow.h
    ui/mainwindow.ui
)

target_include_directories(hw1
    PRIVATE include
)

target_link_libraries(hw1
    PRIVATE Qt5::Core Qt5::Gui Qt5::Widgets Qt5::Network
)
