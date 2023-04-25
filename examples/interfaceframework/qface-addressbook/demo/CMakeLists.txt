set(CMAKE_AUTOMOC ON)

if(NOT DEFINED INSTALL_EXAMPLESDIR)
    set(INSTALL_EXAMPLESDIR "examples")
endif()

set(INSTALL_EXAMPLEDIR "${INSTALL_EXAMPLESDIR}/interfaceframework/qface-addressbook")

find_package(Qt6 REQUIRED COMPONENTS Core Gui InterfaceFramework Qml Quick)

qt_add_executable(qface-addressbook
    main.cpp
)

set_target_properties(qface-addressbook PROPERTIES RUNTIME_OUTPUT_DIRECTORY ../)
set_target_properties(qface-addressbook PROPERTIES
    WIN32_EXECUTABLE TRUE
    MACOSX_BUNDLE FALSE
)
set_property(TARGET qface-addressbook APPEND PROPERTY QT_ANDROID_EXTRA_PLUGINS
    "${CMAKE_CURRENT_BINARY_DIR}/../interfaceframework")

target_compile_definitions(qface-addressbook PUBLIC
    QT_DEPRECATED_WARNINGS
)

target_link_libraries(qface-addressbook PUBLIC
    QtIfAdressBookExample
    Qt::Quick
)

# Resources:
set(qml_resource_files
    "main.qml"
)

qt_add_resources(qface-addressbook "qml"
    PREFIX
        "/"
    FILES
        ${qml_resource_files}
)

install(TARGETS qface-addressbook
    RUNTIME DESTINATION "${INSTALL_EXAMPLEDIR}"
    BUNDLE DESTINATION "${INSTALL_EXAMPLEDIR}"
    LIBRARY DESTINATION "${INSTALL_EXAMPLEDIR}"
)
