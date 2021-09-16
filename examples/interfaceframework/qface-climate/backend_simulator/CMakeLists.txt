# Generated from backend_simulator.pro.
set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

if(NOT DEFINED INSTALL_EXAMPLESDIR)
  set(INSTALL_EXAMPLESDIR "examples")
endif()

set(INSTALL_EXAMPLEDIR "${INSTALL_EXAMPLESDIR}/interfaceframework/qface-climate/interfaceframework/")

find_package(Qt6 COMPONENTS Core)
find_package(Qt6 COMPONENTS Gui)
find_package(Qt6 COMPONENTS InterfaceFramework)

#! [2]
qt_add_plugin(climate_backend_simulator)
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
    Qt::Core
    Qt::Gui
    Qt::InterfaceFramework
)
#! [1]

install(TARGETS climate_backend_simulator
    RUNTIME DESTINATION "${INSTALL_EXAMPLEDIR}"
    BUNDLE DESTINATION "${INSTALL_EXAMPLEDIR}"
    LIBRARY DESTINATION "${INSTALL_EXAMPLEDIR}"
)
