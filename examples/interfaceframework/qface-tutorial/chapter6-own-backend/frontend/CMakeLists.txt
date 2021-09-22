# Generated from frontend.pro.

cmake_minimum_required(VERSION 3.16)
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

qt6_add_library(libIc_chapter6)
set_target_properties(libIc_chapter6 PROPERTIES OUTPUT_NAME "InstrumentCluster")
set_target_properties(libIc_chapter6 PROPERTIES RUNTIME_OUTPUT_DIRECTORY ../)

# Interface Framework Generator:
qt6_ifcodegen_extend_target(libIc_chapter6
    IDL_FILES ../instrument-cluster.qface
    TEMPLATE frontend
)

target_link_libraries(libIc_chapter6 PUBLIC
    Qt::Core
    Qt::Gui
    Qt::InterfaceFramework
    Qt::InterfaceFrameworkPrivate
    Qt::Qml
    Qt::Quick
)

install(TARGETS libIc_chapter6
    RUNTIME DESTINATION "${INSTALL_EXAMPLEDIR}"
    BUNDLE DESTINATION "${INSTALL_EXAMPLEDIR}"
    LIBRARY DESTINATION "${INSTALL_EXAMPLEDIR}"
)
