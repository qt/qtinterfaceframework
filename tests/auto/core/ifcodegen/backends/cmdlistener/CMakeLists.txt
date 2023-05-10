set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

include(GenerateExportHeader)

qt_add_qml_module(cmdlistener
    URI cmdlistener
    VERSION 1.0
    SOURCES
        cmdlistener.h
        cmdlistener.cpp
)

set_target_properties(cmdlistener PROPERTIES RUNTIME_OUTPUT_DIRECTORY ../)

target_include_directories(cmdlistener
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${CMAKE_CURRENT_BINARY_DIR}
)

generate_export_header(cmdlistener)
