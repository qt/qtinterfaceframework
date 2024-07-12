cmake_minimum_required(VERSION 3.16)
project(example_if_addressbook LANGUAGES CXX)

find_package(Qt6 REQUIRED COMPONENTS Core)

qt_standard_project_setup()

add_subdirectory(frontend)
add_subdirectory(backend_simulator)
add_subdirectory(demo)
