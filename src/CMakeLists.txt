cmake_minimum_required(VERSION 3.11.3)
add_executable(TetrisApp main.cpp)
target_link_libraries(TetrisApp sfml-graphics sfml-window sfml-system)