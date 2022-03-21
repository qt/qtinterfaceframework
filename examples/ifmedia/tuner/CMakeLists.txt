cmake_minimum_required(VERSION 3.16)
project(tuner LANGUAGES CXX)

set(CMAKE_AUTOMOC ON)

if(NOT DEFINED INSTALL_EXAMPLESDIR)
    set(INSTALL_EXAMPLESDIR "examples")
endif()

set(INSTALL_EXAMPLEDIR "${INSTALL_EXAMPLESDIR}/ifmedia/tuner")

find_package(Qt6 REQUIRED COMPONENTS Core Gui InterfaceFramework Qml Quick)

qt_add_executable(tuner
    main.cpp
)

set_target_properties(tuner PROPERTIES
    WIN32_EXECUTABLE TRUE
    MACOSX_BUNDLE TRUE
)

target_link_libraries(tuner PUBLIC
    Qt::Core
    Qt::Gui
    Qt::InterfaceFramework
    Qt::Qml
    Qt::Quick
)

# Resources:
set(qml_resource_files
    "main.qml"
)

qt_add_resources(tuner "qml"
    PREFIX
        "/"
    FILES
        ${qml_resource_files}
)

install(TARGETS tuner
    RUNTIME DESTINATION "${INSTALL_EXAMPLEDIR}"
    BUNDLE DESTINATION "${INSTALL_EXAMPLEDIR}"
    LIBRARY DESTINATION "${INSTALL_EXAMPLEDIR}"
)
