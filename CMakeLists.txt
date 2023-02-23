cmake_minimum_required(VERSION 3.20)

project(bbwidgets LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


find_package(Qt6 REQUIRED COMPONENTS Widgets)
qt_standard_project_setup()


if(WIN32)
    enable_language("RC")
endif()

if(MSVC)
    add_compile_options(/W4 /WX /external:anglebrackets /external:W0)
else()
    add_compile_options(-Wall -Wextra -pedantic -Werror)
endif()

add_subdirectory("lib")
add_subdirectory("example")
add_subdirectory("test")

