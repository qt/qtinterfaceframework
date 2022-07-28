# Generated from server_qtro_test.pro.
set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

qt_internal_add_test(tst_org-example-echo-qtro
    SOURCES
        contactsmodelservice.cpp contactsmodelservice.h
        echoservice.cpp echoservice.h
        echozonedservice.cpp echozonedservice.h
        main.cpp
        server.cpp server.h
        tst_echoqtro.cpp tst_echoqtro.h
    LIBRARIES
        echo_qtro_frontend
)

# This dummy QML file is not used by the test itself, but helps
# to let tools like androiddeployqt know what QML dependencies we
# have and package everything correctly
qt_add_resources(tst_org-example-echo-qtro "qml"
    PREFIX "/"
    FILES qml_dependencies.qml
)

set_target_properties(tst_org-example-echo-qtro PROPERTIES RUNTIME_OUTPUT_DIRECTORY ../)

# Interface Framework Generator:
qt6_ifcodegen_extend_target(tst_org-example-echo-qtro
    IDL_FILES ../../org.example.echo.qface
    TEMPLATE server_qtro
)

# QT_ANDROID_EXTRA_PLUGINS requires a list of directories, not files!
set_target_properties(tst_org-example-echo-qtro PROPERTIES
    QT_ANDROID_EXTRA_PLUGINS "${CMAKE_CURRENT_BINARY_DIR}/../interfaceframework"
)
