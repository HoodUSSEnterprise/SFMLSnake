cmake_minimum_required(VERSION 3.10)

project(Snake)

set(CMAKE_CXX_STANDARD 17) # C++17

add_executable(Snake code.cpp)

target_include_directories(Snake PRIVATE "D:/SFML-2.6.1/include")
target_link_directories(Snake PRIVATE "D:/SFML-2.6.1/lib")
target_link_libraries(Snake PRIVATE "sfml-window" "sfml-graphics" "sfml-system")

add_custom_command(TARGET Snake POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "D:/SFML-2.6.1/bin/sfml-graphics-2.dll"
        $<TARGET_FILE_DIR:Snake>
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "D:/SFML-2.6.1/bin/sfml-window-2.dll"
        $<TARGET_FILE_DIR:Snake>
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "D:/SFML-2.6.1/bin/sfml-system-2.dll"
        $<TARGET_FILE_DIR:Snake>
)