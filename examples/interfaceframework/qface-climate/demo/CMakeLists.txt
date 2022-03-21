set(CMAKE_AUTOMOC ON)

if(NOT DEFINED INSTALL_EXAMPLESDIR)
    set(INSTALL_EXAMPLESDIR "examples")
endif()

set(INSTALL_EXAMPLEDIR "${INSTALL_EXAMPLESDIR}/interfaceframework/qface-climate")

find_package(Qt6 REQUIRED COMPONENTS Core Gui InterfaceFramework Qml Quick)

qt_add_executable(qface-climate
    main.cpp
)

set_target_properties(qface-climate PROPERTIES RUNTIME_OUTPUT_DIRECTORY ..)
set_target_properties(qface-climate PROPERTIES
    WIN32_EXECUTABLE TRUE
    MACOSX_BUNDLE FALSE
)

target_compile_definitions(qface-climate PUBLIC
    QT_DEPRECATED_WARNINGS
)

target_link_libraries(qface-climate PUBLIC
    QtIfClimateExample
    Qt::Quick
)

set(import_path "${CMAKE_CURRENT_BINARY_DIR}/frontend/qml")
if (NOT ${import_path} IN_LIST QML_IMPORT_PATH)
    list (APPEND QML_IMPORT_PATH "${import_path}")
    set(QML_IMPORT_PATH ${QML_IMPORT_PATH} CACHE INTERNAL "" FORCE)
endif()

# Resources:
set(qml_resource_files
    "main.qml"
)

qt_add_resources(qface-climate "qml"
    PREFIX
        "/"
    FILES
        ${qml_resource_files}
)

install(TARGETS qface-climate
    RUNTIME DESTINATION "${INSTALL_EXAMPLEDIR}"
    BUNDLE DESTINATION "${INSTALL_EXAMPLEDIR}"
    LIBRARY DESTINATION "${INSTALL_EXAMPLEDIR}"
)
