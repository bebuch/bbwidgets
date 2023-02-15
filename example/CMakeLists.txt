cmake_minimum_required(VERSION 3.20)

add_executable(led WIN32 "led.cpp")
target_link_libraries(led PRIVATE ${PROJECT_NAME})

install(TARGETS led RUNTIME DESTINATION example)
