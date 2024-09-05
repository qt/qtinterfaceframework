cmake_minimum_required(VERSION 3.16)
project(remote LANGUAGES CXX)

if(NOT DEFINED INSTALL_EXAMPLESDIR)
    set(INSTALL_EXAMPLESDIR "examples")
endif()

set(INSTALL_EXAMPLEDIR "${INSTALL_EXAMPLESDIR}/interfaceframework/remote")

find_package(Qt6 REQUIRED COMPONENTS InterfaceFramework Quick)

qt_standard_project_setup(REQUIRES 6.7)

## Application ##

qt_add_executable(remote WIN32
    main.cpp
)

qt_add_qml_module(remote
    URI Example
    QML_FILES
        Main.qml
)

target_link_libraries(remote PRIVATE
    remote_frontend
    Qt::Quick
)

## Frontend ##

#! [0]
qt_ifcodegen_add_qml_module(remote_frontend
    IDL_FILES example-remote.qface
    TEMPLATE frontend
)
#! [0]

## Backend ##

#! [1]
qt_ifcodegen_add_plugin(remote_backend_qtro
    IDL_FILES example-remote.qface
    TEMPLATE backend_qtro
)
#! [1]

#! [3]
set_target_properties(remote_backend_qtro PROPERTIES
    LIBRARY_OUTPUT_DIRECTORY interfaceframework
)
#! [3]

#! [2]
target_link_libraries(remote_backend_qtro PRIVATE
    remote_frontend
)
#! [2]

## Server ##

#! [4]
qt_add_executable(remote-server WIN32
    server/serverMain.cpp
    server/processingservice.h
    server/processingservice.cpp
)

qt_ifcodegen_extend_target(remote-server
    IDL_FILES example-remote.qface
    TEMPLATE server_qtro
)

target_link_libraries(remote-server PRIVATE
    remote_frontend
)
#! [4]

set_property(TARGET remote APPEND PROPERTY QT_ANDROID_EXTRA_PLUGINS
    "${CMAKE_CURRENT_BINARY_DIR}/interfaceframework"
)
set(QT_USE_TARGET_ANDROID_BUILD_DIR TRUE CACHE BOOL
    "Support multiple apk targets in one folder")

install(TARGETS remote remote_frontend remote-server
    RUNTIME DESTINATION "${INSTALL_EXAMPLEDIR}"
    BUNDLE DESTINATION "${INSTALL_EXAMPLEDIR}"
    LIBRARY DESTINATION "${INSTALL_EXAMPLEDIR}"
)
install(TARGETS remote_backend_qtro
    RUNTIME DESTINATION "${INSTALL_EXAMPLEDIR}/interfaceframework"
    BUNDLE DESTINATION "${INSTALL_EXAMPLEDIR}/interfaceframework"
    LIBRARY DESTINATION "${INSTALL_EXAMPLEDIR}/interfaceframework"
)
