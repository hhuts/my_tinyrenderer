cmake_minimum_required(VERSION 3.10)
project(TinyRenderer CXX)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(SOURCES
    main.cpp
    model.cpp
    our_gl.cpp
    tgaimage.cpp
    matrix_vector_math.cpp
)

add_executable(main ${SOURCES})

target_include_directories(main PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

