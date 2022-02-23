# Generated from frontend.pro.
set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

if(NOT DEFINED INSTALL_EXAMPLESDIR)
  set(INSTALL_EXAMPLESDIR "examples")
endif()

set(INSTALL_EXAMPLEDIR "${INSTALL_EXAMPLESDIR}/interfaceframework/qface-tutorial/chapter2-enums-structs")

find_package(Qt6 COMPONENTS Core)
find_package(Qt6 COMPONENTS InterfaceFramework)
find_package(Qt6 COMPONENTS Qml)
find_package(Qt6 COMPONENTS Quick)

qt_add_library(libIc_chapter2)
set_target_properties(libIc_chapter2 PROPERTIES OUTPUT_NAME "InstrumentCluster")
set_target_properties(libIc_chapter2 PROPERTIES RUNTIME_OUTPUT_DIRECTORY ../)

# Interface Framework Generator:
qt_ifcodegen_extend_target(libIc_chapter2
    IDL_FILES ../instrument-cluster.qface
    TEMPLATE frontend
)

target_link_libraries(libIc_chapter2 PUBLIC
    Qt::Core
    Qt::InterfaceFramework
    Qt::InterfaceFrameworkPrivate
    Qt::Qml
    Qt::Quick
)

set(import_path "${CMAKE_CURRENT_BINARY_DIR}/frontend/qml")
if (NOT ${import_path} IN_LIST QML_IMPORT_PATH)
    list (APPEND QML_IMPORT_PATH "${import_path}")
    set(QML_IMPORT_PATH ${QML_IMPORT_PATH} CACHE INTERNAL "" FORCE)
endif()

install(TARGETS libIc_chapter2
    RUNTIME DESTINATION "${INSTALL_EXAMPLEDIR}"
    BUNDLE DESTINATION "${INSTALL_EXAMPLEDIR}"
    LIBRARY DESTINATION "${INSTALL_EXAMPLEDIR}"
)
