cmake_minimum_required(VERSION 3.16)
project(qface-remote LANGUAGES CXX)

set(CMAKE_AUTOMOC ON)

if(NOT DEFINED INSTALL_EXAMPLESDIR)
    set(INSTALL_EXAMPLESDIR "examples")
endif()

set(INSTALL_EXAMPLEDIR "${INSTALL_EXAMPLESDIR}/interfaceframework/qface-remote")

find_package(Qt6 REQUIRED COMPONENTS Core Gui InterfaceFramework Qml Quick)

qt_add_executable(qface-remote
    main.cpp
)

set_target_properties(qface-remote PROPERTIES RUNTIME_OUTPUT_DIRECTORY ..)
set_target_properties(qface-remote PROPERTIES
    WIN32_EXECUTABLE TRUE
    MACOSX_BUNDLE FALSE
)
set_property(TARGET qface-remote APPEND PROPERTY QT_ANDROID_EXTRA_PLUGINS
    "${CMAKE_CURRENT_BINARY_DIR}/../interfaceframework")

target_compile_definitions(qface-remote PUBLIC
    QT_DEPRECATED_WARNINGS
)

target_link_libraries(qface-remote PUBLIC
    QtIfRemoteExample
    Qt::Quick
)

# Resources:
set(qml_resource_files
    "main.qml"
)

qt_add_resources(qface-remote "qml"
    PREFIX
        "/"
    FILES
        ${qml_resource_files}
)

install(TARGETS qface-remote
    RUNTIME DESTINATION "${INSTALL_EXAMPLEDIR}"
    BUNDLE DESTINATION "${INSTALL_EXAMPLEDIR}"
    LIBRARY DESTINATION "${INSTALL_EXAMPLEDIR}"
)
