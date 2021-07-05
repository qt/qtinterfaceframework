# Generated from frontend.pro.
set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

add_library(include_test_frontend)
set_target_properties(include_test_frontend PROPERTIES RUNTIME_OUTPUT_DIRECTORY ../)

# Interface Framework Generator:
qt6_ifcodegen(include_test_frontend
    IFCODEGEN_SOURCES ../../include-test.qface
    IFCODEGEN_TEMPLATE frontend
    IFCODEGEN_IMPORT_PATH ../../qface\ imports
)
target_include_directories(include_test_frontend PUBLIC
    ${CMAKE_CURRENT_BINARY_DIR}
)

target_link_libraries(include_test_frontend PUBLIC
    include_test_common
    Qt::Core
    Qt::Gui
    Qt::InterfaceFramework
    Qt::InterfaceFrameworkPrivate
    Qt::Qml
    Qt::Quick
)
