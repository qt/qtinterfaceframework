# Generated from frontend.pro.

cmake_minimum_required(VERSION 3.16)
project(QtIfRemoteExample LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

if(NOT DEFINED INSTALL_EXAMPLESDIR)
  set(INSTALL_EXAMPLESDIR "examples")
endif()

set(INSTALL_EXAMPLEDIR "${INSTALL_EXAMPLESDIR}/interfaceframework/qface-remote")

find_package(Qt6 COMPONENTS Core)
#! [0]
find_package(Qt6 COMPONENTS InterfaceFramework)
#! [0]
find_package(Qt6 COMPONENTS Qml)
find_package(Qt6 COMPONENTS Quick)

#! [1]
qt_add_library(QtIfRemoteExample)

# Interface Framework Generator:
qt_ifcodegen_extend_target(QtIfRemoteExample
    IDL_FILES ../example-remote.qface
    TEMPLATE frontend
)
#! [1]

set_target_properties(QtIfRemoteExample PROPERTIES RUNTIME_OUTPUT_DIRECTORY ..)

set(import_path "${CMAKE_CURRENT_BINARY_DIR}/frontend/qml")
if (NOT ${import_path} IN_LIST QML_IMPORT_PATH)
    list (APPEND QML_IMPORT_PATH "${import_path}")
    set(QML_IMPORT_PATH ${QML_IMPORT_PATH} CACHE INTERNAL "" FORCE)
endif()

install(TARGETS QtIfRemoteExample
    RUNTIME DESTINATION "${INSTALL_EXAMPLEDIR}"
    BUNDLE DESTINATION "${INSTALL_EXAMPLEDIR}"
    LIBRARY DESTINATION "${INSTALL_EXAMPLEDIR}"
)
