# Generated from frontend.pro.
set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

if(NOT DEFINED INSTALL_EXAMPLESDIR)
  set(INSTALL_EXAMPLESDIR "examples")
endif()

set(INSTALL_EXAMPLEDIR "${INSTALL_EXAMPLESDIR}/interfaceframework/qface-ifaddressbook")

find_package(Qt6 COMPONENTS Core)
find_package(Qt6 COMPONENTS InterfaceFramework)
find_package(Qt6 COMPONENTS Qml)
find_package(Qt6 COMPONENTS Quick)

qt6_add_library(QtIfAdressBookExample)

# Interface Framework Generator:
qt6_ifcodegen_extend_target(QtIfAdressBookExample
    IDL_FILES ../example-if-addressbook.qface
    TEMPLATE frontend
)

set_target_properties(QtIfAdressBookExample PROPERTIES RUNTIME_OUTPUT_DIRECTORY ../)

target_link_libraries(QtIfAdressBookExample PUBLIC
    Qt::Core
    Qt::InterfaceFramework
    Qt::InterfaceFrameworkPrivate
    Qt::Qml
    Qt::Quick
)

install(TARGETS QtIfAdressBookExample
    RUNTIME DESTINATION "${INSTALL_EXAMPLEDIR}"
    BUNDLE DESTINATION "${INSTALL_EXAMPLEDIR}"
    LIBRARY DESTINATION "${INSTALL_EXAMPLEDIR}"
)
