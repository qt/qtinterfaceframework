# Generated from server_qtro.pro.

cmake_minimum_required(VERSION 3.14)
project(qface-if-remote-server LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

if(NOT DEFINED INSTALL_EXAMPLESDIR)
  set(INSTALL_EXAMPLESDIR "examples")
endif()

set(INSTALL_EXAMPLEDIR "${INSTALL_EXAMPLESDIR}/interfaceframework/qface-if-remote-server")

find_package(Qt6 COMPONENTS Core)
find_package(Qt6 COMPONENTS Qml)
find_package(Qt6 COMPONENTS Quick)
find_package(Qt6 COMPONENTS InterfaceFramework)

qt_add_executable(qface-if-remote-server
    main.cpp
    processingservice.cpp processingservice.h
)

#! [0]
# Interface Framework Generator:
qt6_ifcodegen_extend_target(qface-if-remote-server
    IDL_FILES ../example-if-remote.qface
    TEMPLATE server_qtro
)
#! [0]

set_target_properties(qface-if-remote-server PROPERTIES RUNTIME_OUTPUT_DIRECTORY ..)

set_target_properties(qface-if-remote-server PROPERTIES
    WIN32_EXECUTABLE TRUE
    MACOSX_BUNDLE FALSE
)

target_compile_definitions(qface-if-remote-server PUBLIC
    QT_DEPRECATED_WARNINGS
)

target_link_libraries(qface-if-remote-server PUBLIC
    QtIfRemoteExample
    Qt::Core
)

install(TARGETS qface-if-remote-server
    RUNTIME DESTINATION "${INSTALL_EXAMPLEDIR}"
    BUNDLE DESTINATION "${INSTALL_EXAMPLEDIR}"
    LIBRARY DESTINATION "${INSTALL_EXAMPLEDIR}"
)
