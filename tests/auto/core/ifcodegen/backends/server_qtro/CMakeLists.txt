set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

qt_add_executable(org-example-echo-qtro-server)
set_target_properties(org-example-echo-qtro-server PROPERTIES RUNTIME_OUTPUT_DIRECTORY ../)

target_sources(org-example-echo-qtro-server
    PRIVATE
        contactsmodelservice.cpp contactsmodelservice.h
        echoservice.cpp echoservice.h
        echozonedservice.cpp echozonedservice.h
        main.cpp
)

# Interface Framework Generator:
qt6_ifcodegen_extend_target(org-example-echo-qtro-server
    IDL_FILES ../../org.example.echo.qface
    TEMPLATE server_qtro
)

target_link_libraries(org-example-echo-qtro-server
    PRIVATE
        echo_qtro_frontend
        cmdlistener
)
