cmake_minimum_required(VERSION 4.1)
project(CODE_ALPHA_TASK_1)

set(CMAKE_CXX_STANDARD 14)

include_directories(.)

add_executable(CODE_ALPHA_TASK_1
        AcademicYear.cpp
        AcademicYear.h
        Degree.cpp
        Degree.h
        main.cpp
        Semester.cpp
        Semester.h)
