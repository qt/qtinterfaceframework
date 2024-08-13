set(CMAKE_INCLUDE_CURRENT_DIR ON)

set_source_files_properties(instrumentcluster.xml PROPERTIES INCLUDE "")
set(cluster_SRCS)
qt_add_dbus_adaptor(cluster_SRCS
    instrumentcluster.xml
    dbus_conversion.h
    "" # empty parent_class value on purpose to not pass -l flag
    instrumentcluster_adaptor
)

qt_add_executable(chapter7-demo-server WIN32
    dbus_conversion.h
    instrumentcluster.cpp instrumentcluster.h
    main.cpp
    ${cluster_SRCS}
)

set_target_properties(chapter7-demo-server PROPERTIES RUNTIME_OUTPUT_DIRECTORY ../)

target_link_libraries(chapter7-demo-server PUBLIC
    libIc_chapter7
    Qt::DBus
)

install(TARGETS chapter7-demo-server
    RUNTIME DESTINATION "${INSTALL_EXAMPLEDIR}"
    BUNDLE DESTINATION "${INSTALL_EXAMPLEDIR}"
    LIBRARY DESTINATION "${INSTALL_EXAMPLEDIR}"
)
