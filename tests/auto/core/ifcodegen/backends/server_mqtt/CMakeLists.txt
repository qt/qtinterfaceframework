set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

find_package(Qt6 COMPONENTS Mqtt)

qt_add_executable(org-example-echo-mqtt-server)
set_target_properties(org-example-echo-mqtt-server PROPERTIES RUNTIME_OUTPUT_DIRECTORY ../)

target_sources(org-example-echo-mqtt-server
    PRIVATE
        main.cpp
        echoservice.h
        echoservice.cpp
)

target_link_libraries(org-example-echo-mqtt-server
    PRIVATE
        echo_qtro_frontend
        cmdlistener
        Qt6::Mqtt
)
