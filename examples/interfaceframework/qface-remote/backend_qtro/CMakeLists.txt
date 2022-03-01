# Generated from backend_qtro.pro.
set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

if(NOT DEFINED INSTALL_EXAMPLESDIR)
  set(INSTALL_EXAMPLESDIR "examples")
endif()

set(INSTALL_EXAMPLEDIR "${INSTALL_EXAMPLESDIR}/interfaceframework/qface-remote/interfaceframework/")

find_package(Qt6 COMPONENTS Core)
find_package(Qt6 COMPONENTS Gui)
find_package(Qt6 COMPONENTS InterfaceFramework)

#! [2]
qt_add_plugin(remote_backend_qtro)
#! [2]
#! [0]
set_target_properties(remote_backend_qtro PROPERTIES LIBRARY_OUTPUT_DIRECTORY ../interfaceframework)
#! [0]

#! [2]
# Interface Framework Generator:
qt_ifcodegen_extend_target(remote_backend_qtro
    IDL_FILES ../example-remote.qface
    TEMPLATE backend_qtro
)
#! [2]

#! [1]
target_link_libraries(remote_backend_qtro PUBLIC
    QtIfRemoteExample
)
#! [1]

install(TARGETS remote_backend_qtro
    RUNTIME DESTINATION "${INSTALL_EXAMPLEDIR}"
    BUNDLE DESTINATION "${INSTALL_EXAMPLEDIR}"
    LIBRARY DESTINATION "${INSTALL_EXAMPLEDIR}"
)
