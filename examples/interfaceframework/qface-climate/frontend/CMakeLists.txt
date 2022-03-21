set(CMAKE_AUTOMOC ON)

if(NOT DEFINED INSTALL_EXAMPLESDIR)
    set(INSTALL_EXAMPLESDIR "examples")
endif()

set(INSTALL_EXAMPLEDIR "${INSTALL_EXAMPLESDIR}/interfaceframework/qface-climate")

find_package(Qt6 REQUIRED COMPONENTS Core Gui InterfaceFramework Qml Quick)

#! [0]

#! [0]

#! [1]
qt_add_library(QtIfClimateExample)

# Interface Framework Generator:
qt_ifcodegen_extend_target(QtIfClimateExample
    IDL_FILES ../example-climate.qface
    TEMPLATE frontend
)

#! [1]

set_target_properties(QtIfClimateExample PROPERTIES RUNTIME_OUTPUT_DIRECTORY ..)

install(TARGETS QtIfClimateExample
    RUNTIME DESTINATION "${INSTALL_EXAMPLEDIR}"
    BUNDLE DESTINATION "${INSTALL_EXAMPLEDIR}"
    LIBRARY DESTINATION "${INSTALL_EXAMPLEDIR}"
)
