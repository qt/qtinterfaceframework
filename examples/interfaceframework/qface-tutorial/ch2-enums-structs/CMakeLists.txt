cmake_minimum_required(VERSION 3.16)
project(qface-ch2 LANGUAGES CXX)

find_package(Qt6 REQUIRED COMPONENTS Core)

qt_standard_project_setup()

add_subdirectory(instrument-cluster)
add_subdirectory(frontend)
