# Generated from mediaplayer.pro.

cmake_minimum_required(VERSION 3.16)
project(qtifmediaplayer LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

if(NOT DEFINED INSTALL_EXAMPLESDIR)
  set(INSTALL_EXAMPLESDIR "examples")
endif()

set(INSTALL_EXAMPLEDIR "${INSTALL_EXAMPLESDIR}/ifmedia/mediaplayer")

find_package(Qt6 COMPONENTS Core)
find_package(Qt6 COMPONENTS Gui)
find_package(Qt6 COMPONENTS Qml)
find_package(Qt6 COMPONENTS Quick)
find_package(Qt6 COMPONENTS InterfaceFramework)

qt_add_executable(qtifmediaplayer
    main.cpp
)
set_target_properties(qtifmediaplayer PROPERTIES
    WIN32_EXECUTABLE TRUE
    MACOSX_BUNDLE TRUE
)
target_link_libraries(qtifmediaplayer PUBLIC
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

qt_add_resources(qtifmediaplayer "qml"
    PREFIX
        "/"
    FILES
        ${qml_resource_files}
)

install(TARGETS qtifmediaplayer
    RUNTIME DESTINATION "${INSTALL_EXAMPLEDIR}"
    BUNDLE DESTINATION "${INSTALL_EXAMPLEDIR}"
    LIBRARY DESTINATION "${INSTALL_EXAMPLEDIR}"
)
