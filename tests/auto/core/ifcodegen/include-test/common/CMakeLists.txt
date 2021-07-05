# Generated from common.pro.
set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

add_library(include_test_common)
set_target_properties(include_test_common PROPERTIES RUNTIME_OUTPUT_DIRECTORY ../)

# Interface Framework Generator:
qt6_ifcodegen(include_test_common
    IFCODEGEN_SOURCES ../../qface\ imports/common.qface
    IFCODEGEN_TEMPLATE frontend
)
target_include_directories(include_test_common PUBLIC
    ${CMAKE_CURRENT_BINARY_DIR}
)

target_link_libraries(include_test_common PUBLIC
    Qt::Core
    Qt::Gui
    Qt::InterfaceFramework
    Qt::InterfaceFrameworkPrivate
    Qt::Qml
    Qt::Quick
)

