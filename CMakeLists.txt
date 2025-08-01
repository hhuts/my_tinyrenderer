cmake_minimum_required(VERSION 3.31)  # 版本号不需要补丁号(3.31.4→3.31)

project(tinyrender LANGUAGES CXX)  # LANGUAGES参数更规范

add_executable(mini_tinyrender 
    main.cpp 
    matrix_vector_math.cpp  # &符号在CMake中可能导致问题
    model.cpp 
    our_gl.cpp 
    tgaimage.cpp
)