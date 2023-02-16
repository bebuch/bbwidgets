cmake_minimum_required(VERSION 3.20)

file(GLOB SOURCE_FILES "*.cpp")
file(GLOB HEADER_FILES "*.hpp" "../include/${PROJECT_NAME}/*.hpp")

add_library(${PROJECT_NAME} SHARED ${SOURCE_FILES} ${HEADER_FILES})
target_compile_definitions(${PROJECT_NAME} PRIVATE SHARED_LIBRARY)
target_link_libraries(${PROJECT_NAME} PUBLIC Qt6::Widgets)

add_custom_command(
    TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        $<TARGET_FILE:Qt6::Widgets>
        $<TARGET_FILE:Qt6::Gui>
        $<TARGET_FILE:Qt6::Core>
        $<TARGET_FILE_DIR:${PROJECT_NAME}>
    COMMAND ${CMAKE_COMMAND} -E make_directory
        $<TARGET_FILE_DIR:${PROJECT_NAME}>/platforms
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        $<TARGET_FILE:Qt6::QWindowsIntegrationPlugin>
        $<TARGET_FILE_DIR:${PROJECT_NAME}>/platforms)

install(TARGETS ${PROJECT_NAME} RUNTIME DESTINATION bin)
