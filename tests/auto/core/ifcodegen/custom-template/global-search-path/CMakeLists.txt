cmake_minimum_required(VERSION 3.16)
project(tst_template_global_search_path LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

find_package(Qt6 COMPONENTS Core)
find_package(Qt6 COMPONENTS Gui)
find_package(Qt6 COMPONENTS Test)

qt_add_executable(tst_template_global_search_path)

set(QT_IFCODEGEN_TEMPLATE_SEARCH_PATH "../templates")

# Interface Framework Generator:
qt_ifcodegen_extend_target(tst_template_global_search_path
    IDL_FILES ../../org.example.echo.qface
    TEMPLATE custom-test
)

target_link_libraries(tst_template_global_search_path PUBLIC
    Qt::Core
    Qt::Gui
    Qt::Test
)
