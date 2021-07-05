# Generated from qmlplugin.pro.
set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

# Interface Framework Generator:
qt6_ifcodegen(qmlplugin
    IFCODEGEN_SOURCES ../../include-test.qface
    IFCODEGEN_TEMPLATE qmlplugin
    IFCODEGEN_IMPORT_PATH ../../qface\ imports
)

target_link_libraries(qmlplugin PRIVATE
    include_test_frontend
    Qt::InterfaceFramework
)
