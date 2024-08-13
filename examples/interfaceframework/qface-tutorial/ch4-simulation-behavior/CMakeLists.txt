cmake_minimum_required(VERSION 3.16)
project(qface-ch4 LANGUAGES CXX)

find_package(Qt6 REQUIRED COMPONENTS Core InterfaceFramework Qml Quick)

qt_standard_project_setup(REQUIRES 6.7)

if(NOT DEFINED INSTALL_EXAMPLESDIR)
    set(INSTALL_EXAMPLESDIR "examples")
endif()

set(INSTALL_EXAMPLEDIR "${INSTALL_EXAMPLESDIR}/interfaceframework/qface-tutorial/ch4-simulation-behavior")

add_subdirectory(instrument-cluster)
add_subdirectory(frontend)
add_subdirectory(backend_simulator)
