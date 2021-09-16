# Generated from frontend.pro.
set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

if(NOT DEFINED INSTALL_EXAMPLESDIR)
  set(INSTALL_EXAMPLESDIR "examples")
endif()

set(INSTALL_EXAMPLEDIR "${INSTALL_EXAMPLESDIR}/interfaceframework/qface-climate")

find_package(Qt6 COMPONENTS Core)
find_package(Qt6 COMPONENTS Gui)
#! [0]
find_package(Qt6 COMPONENTS InterfaceFramework)
#! [0]
find_package(Qt6 COMPONENTS Qml)
find_package(Qt6 COMPONENTS Quick)

#! [1]
qt6_add_library(QtIfClimateExample)

# Interface Framework Generator:
qt6_ifcodegen_extend_target(QtIfClimateExample
    IDL_FILES ../example-climate.qface
    TEMPLATE frontend
)
#! [1]

set_target_properties(QtIfClimateExample PROPERTIES RUNTIME_OUTPUT_DIRECTORY ..)

target_link_libraries(QtIfClimateExample PUBLIC
    Qt::Core
    Qt::Gui
    Qt::InterfaceFramework
    Qt::InterfaceFrameworkPrivate
    Qt::Qml
    Qt::Quick
)

install(TARGETS QtIfClimateExample
    RUNTIME DESTINATION "${INSTALL_EXAMPLEDIR}"
    BUNDLE DESTINATION "${INSTALL_EXAMPLEDIR}"
    LIBRARY DESTINATION "${INSTALL_EXAMPLEDIR}"
)
