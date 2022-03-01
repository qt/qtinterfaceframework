# Generated from backend_qtro.pro.
set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

if(NOT DEFINED INSTALL_EXAMPLESDIR)
  set(INSTALL_EXAMPLESDIR "examples")
endif()

set(INSTALL_EXAMPLEDIR "${INSTALL_EXAMPLESDIR}/interfaceframework/qface-tutorial/chapter5-ipc")

find_package(Qt6 COMPONENTS Core)
find_package(Qt6 COMPONENTS Gui)
find_package(Qt6 COMPONENTS InterfaceFramework)

qt_add_plugin(ic_chapter5_qtro)
set_target_properties(ic_chapter5_qtro PROPERTIES LIBRARY_OUTPUT_DIRECTORY ../interfaceframework)

# Interface Framework Generator:
qt_ifcodegen_extend_target(ic_chapter5_qtro
    IDL_FILES ../instrument-cluster.qface
    TEMPLATE backend_qtro
)

target_link_libraries(ic_chapter5_qtro PUBLIC
    libIc_chapter5
)

install(TARGETS ic_chapter5_qtro
    RUNTIME DESTINATION "${INSTALL_EXAMPLEDIR}"
    BUNDLE DESTINATION "${INSTALL_EXAMPLEDIR}"
    LIBRARY DESTINATION "${INSTALL_EXAMPLEDIR}"
)
