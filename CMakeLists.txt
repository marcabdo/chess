cmake_minimum_required(VERSION 3.10)
project(chess_gui LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(chess_gui
    src/main.cpp
    src/Board.cpp
    src/Game.cpp
    src/Move.cpp
)

target_include_directories(chess_gui PRIVATE include)

find_package(SFML CONFIG REQUIRED COMPONENTS Graphics Window System)

target_link_libraries(chess_gui
    SFML::Graphics
    SFML::Window
    SFML::System
)

add_custom_command(
    TARGET chess_gui POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
            ${CMAKE_SOURCE_DIR}/fonts
            $<TARGET_FILE_DIR:chess_gui>/fonts
)

add_custom_command(
    TARGET chess_gui POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
            ${CMAKE_SOURCE_DIR}/images
            $<TARGET_FILE_DIR:chess_gui>/images
)

add_custom_command(
    TARGET chess_gui POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
            C:/msys64/mingw64/bin/libsfml-graphics-3.dll
            C:/msys64/mingw64/bin/libsfml-window-3.dll
            C:/msys64/mingw64/bin/libsfml-system-3.dll
            $<TARGET_FILE_DIR:chess_gui>
)


