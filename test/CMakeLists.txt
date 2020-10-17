cmake_minimum_required(VERSION 3.11.3)
add_executable(TetrisTest test.cpp)
target_link_libraries(TetrisTest gtest_main My_Func_Lib)