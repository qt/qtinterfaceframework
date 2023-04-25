set(CMAKE_AUTOMOC ON)

if(NOT DEFINED INSTALL_EXAMPLESDIR)
    set(INSTALL_EXAMPLESDIR "examples")
endif()

set(INSTALL_EXAMPLEDIR "${INSTALL_EXAMPLESDIR}/interfaceframework/qface-climate/interfaceframework/")

find_package(Qt6 REQUIRED COMPONENTS Core Gui InterfaceFramework)

#! [2]
qt_add_plugin(climate_backend_simulator PLUGIN_TYPE interfaceframework)
#! [2]
#! [0]
set_target_properties(climate_backend_simulator PROPERTIES LIBRARY_OUTPUT_DIRECTORY ../interfaceframework)
#! [0]

#! [2]
# Interface Framework Generator:
qt_ifcodegen_extend_target(climate_backend_simulator
    IDL_FILES ../example-climate.qface
    TEMPLATE backend_simulator
)
#! [2]

#! [1]
target_link_libraries(climate_backend_simulator PUBLIC
    QtIfClimateExample
)
#! [1]

install(TARGETS climate_backend_simulator
    RUNTIME DESTINATION "${INSTALL_EXAMPLEDIR}"
    BUNDLE DESTINATION "${INSTALL_EXAMPLEDIR}"
    LIBRARY DESTINATION "${INSTALL_EXAMPLEDIR}"
)
