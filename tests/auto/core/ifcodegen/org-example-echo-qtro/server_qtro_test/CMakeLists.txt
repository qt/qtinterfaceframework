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
    PUBLIC_LIBRARIES
        echo_qtro_frontend
        Qt::Core
        Qt::InterfaceFramework
        Qt::Test
        Qt::IfRemoteObjectsHelperPrivate
)
set_target_properties(tst_org-example-echo-qtro PROPERTIES RUNTIME_OUTPUT_DIRECTORY ../)

# Interface Framework Generator:
qt6_ifcodegen(tst_org-example-echo-qtro
    QFACE_SOURCES ../../org.example.echo.qface
    QFACE_FORMAT server_qtro
)
