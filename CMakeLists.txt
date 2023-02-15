cmake_minimum_required(VERSION 3.20)

project(bbwidgets LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(WIN32)
    enable_language("RC")
endif()


find_package(Qt6 REQUIRED COMPONENTS Widgets)
qt_standard_project_setup()

add_subdirectory("lib")
add_subdirectory("example")
