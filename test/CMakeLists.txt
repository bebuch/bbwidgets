cmake_minimum_required(VERSION 3.20)

file(GLOB SOURCE_FILES "*.cpp")

add_executable(tests ${SOURCE_FILES})
target_link_libraries(tests PRIVATE ${PROJECT_NAME})
