cmake_minimum_required(VERSION 3.16)
project(qface-remote-server LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOMOC ON)

if(NOT DEFINED INSTALL_EXAMPLESDIR)
    set(INSTALL_EXAMPLESDIR "examples")
endif()

set(INSTALL_EXAMPLEDIR "${INSTALL_EXAMPLESDIR}/interfaceframework/qface-remote")

find_package(Qt6 REQUIRED COMPONENTS Core InterfaceFramework Qml Quick)

qt_add_executable(qface-remote-server
    serverMain.cpp
    processingservice.cpp processingservice.h
)

#! [0]
# Interface Framework Generator:
qt_ifcodegen_extend_target(qface-remote-server
    IDL_FILES ../example-remote.qface
    TEMPLATE server_qtro
)
#! [0]

set_target_properties(qface-remote-server PROPERTIES RUNTIME_OUTPUT_DIRECTORY ..)

set_target_properties(qface-remote-server PROPERTIES
    WIN32_EXECUTABLE TRUE
    MACOSX_BUNDLE FALSE
)

target_compile_definitions(qface-remote-server PUBLIC
    QT_DEPRECATED_WARNINGS
)

target_link_libraries(qface-remote-server PUBLIC
    QtIfRemoteExample
)

install(TARGETS qface-remote-server
    RUNTIME DESTINATION "${INSTALL_EXAMPLEDIR}"
    BUNDLE DESTINATION "${INSTALL_EXAMPLEDIR}"
    LIBRARY DESTINATION "${INSTALL_EXAMPLEDIR}"
)
