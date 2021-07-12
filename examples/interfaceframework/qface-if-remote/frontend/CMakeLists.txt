# Generated from frontend.pro.

cmake_minimum_required(VERSION 3.14)
project(QtIfRemoteExample LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

if(NOT DEFINED INSTALL_EXAMPLESDIR)
  set(INSTALL_EXAMPLESDIR "examples")
endif()

set(INSTALL_EXAMPLEDIR "${INSTALL_EXAMPLESDIR}/interfaceframework/qface-if-remote")

find_package(Qt6 COMPONENTS Core)
find_package(Qt6 COMPONENTS Gui)
find_package(Qt6 COMPONENTS InterfaceFramework)
find_package(Qt6 COMPONENTS Qml)
find_package(Qt6 COMPONENTS Quick)

add_library(QtIfRemoteExample)

# Interface Framework Generator:
qt6_ifcodegen_extend_target(QtIfRemoteExample
    IDL_FILES ../example-if-remote.qface
    TEMPLATE frontend
)

set_target_properties(QtIfRemoteExample PROPERTIES RUNTIME_OUTPUT_DIRECTORY ..)

target_link_libraries(QtIfRemoteExample PUBLIC
    Qt::Core
    Qt::Gui
    Qt::InterfaceFramework
    Qt::InterfaceFrameworkPrivate
    Qt::Qml
    Qt::Quick
)

install(TARGETS QtIfRemoteExample
    RUNTIME DESTINATION "${INSTALL_EXAMPLEDIR}"
    BUNDLE DESTINATION "${INSTALL_EXAMPLEDIR}"
    LIBRARY DESTINATION "${INSTALL_EXAMPLEDIR}"
)
