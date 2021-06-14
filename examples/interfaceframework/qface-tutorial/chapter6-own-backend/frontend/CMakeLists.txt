# Generated from frontend.pro.

cmake_minimum_required(VERSION 3.14)
project(QtIfInstrumentCluster LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

if(NOT DEFINED INSTALL_EXAMPLESDIR)
  set(INSTALL_EXAMPLESDIR "examples")
endif()

set(INSTALL_EXAMPLEDIR "${INSTALL_EXAMPLESDIR}/interfaceframework/qface-tutorial/chapter6-own-backend")

find_package(Qt6 COMPONENTS Core)
find_package(Qt6 COMPONENTS Gui)
find_package(Qt6 COMPONENTS InterfaceFramework)
find_package(Qt6 COMPONENTS Qml)
find_package(Qt6 COMPONENTS Quick)

add_library(QtIfInstrumentCluster_Chapter6)
set_target_properties(QtIfInstrumentCluster_Chapter6 PROPERTIES RUNTIME_OUTPUT_DIRECTORY ../)

# Interface Framework Generator:
qt6_ifcodegen(QtIfInstrumentCluster_Chapter6
    QFACE_SOURCES ../instrument-cluster.qface
    QFACE_FORMAT frontend
)

target_include_directories(QtIfInstrumentCluster_Chapter6 PUBLIC
    ${CMAKE_CURRENT_BINARY_DIR}
)

target_link_libraries(QtIfInstrumentCluster_Chapter6 PUBLIC
    Qt::Core
    Qt::Gui
    Qt::InterfaceFramework
    Qt::InterfaceFrameworkPrivate
    Qt::Qml
    Qt::Quick
)

install(TARGETS QtIfInstrumentCluster_Chapter6
    RUNTIME DESTINATION "${INSTALL_EXAMPLEDIR}"
    BUNDLE DESTINATION "${INSTALL_EXAMPLEDIR}"
    LIBRARY DESTINATION "${INSTALL_EXAMPLEDIR}"
)
