cmake_minimum_required(VERSION 3.14)
project(tst_template_extra_search_path LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

find_package(Qt6 COMPONENTS Core)
find_package(Qt6 COMPONENTS Gui)
find_package(Qt6 COMPONENTS Test)

qt_add_executable(tst_template_extra_search_path)

# Interface Framework Generator:
qt_ifcodegen_extend_target(tst_template_extra_search_path
    IDL_FILES ../../org.example.echo.qface
    TEMPLATE custom-test
    EXTRA_TEMPLATE_SEARCH_PATH ..
)

target_link_libraries(tst_template_extra_search_path PUBLIC
    Qt::Core
    Qt::Gui
    Qt::Test
)
