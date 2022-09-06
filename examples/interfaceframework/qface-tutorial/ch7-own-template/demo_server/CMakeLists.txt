set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOMOC ON)

if(NOT DEFINED INSTALL_EXAMPLESDIR)
    set(INSTALL_EXAMPLESDIR "examples")
endif()

set(INSTALL_EXAMPLEDIR "${INSTALL_EXAMPLESDIR}/interfaceframework/qface-tutorial/chapter7-own-template")

find_package(Qt6 REQUIRED COMPONENTS Core DBus InterfaceFramework)

set_source_files_properties(instrumentcluster.xml PROPERTIES INCLUDE "")
set(cluster_SRCS)
qt_add_dbus_adaptor(cluster_SRCS
    instrumentcluster.xml
    dbus_conversion.h
    "" # empty parent_class value on purpose to not pass -l flag
    instrumentcluster_adaptor
)

qt_add_executable(chapter7-demo-server
    dbus_conversion.h
    instrumentcluster.cpp instrumentcluster.h
    main.cpp
    ${cluster_SRCS}
)

set_target_properties(chapter7-demo-server PROPERTIES RUNTIME_OUTPUT_DIRECTORY ../)
set_target_properties(chapter7-demo-server PROPERTIES
    WIN32_EXECUTABLE TRUE
    MACOSX_BUNDLE FALSE
)

target_link_libraries(chapter7-demo-server PUBLIC
    libIc_chapter7
    Qt::DBus
)

install(TARGETS chapter7-demo-server
    RUNTIME DESTINATION "${INSTALL_EXAMPLEDIR}"
    BUNDLE DESTINATION "${INSTALL_EXAMPLEDIR}"
    LIBRARY DESTINATION "${INSTALL_EXAMPLEDIR}"
)
