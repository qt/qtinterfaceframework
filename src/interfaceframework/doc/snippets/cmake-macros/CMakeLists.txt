#! [ifcodegen_extend_target]
find_package(Qt6 COMPONENTS InterfaceFramework)

qt6_add_library(AdressBook)

# Generate code and extend the AddressBook target
# with the generated files
qt6_ifcodegen_extend_target(AdressBook
    IDL_FILES addressbook.qface
    TEMPLATE frontend
)

target_link_libraries(AdressBook PUBLIC
    Qt::Core
    Qt::InterfaceFramework
    Qt::InterfaceFrameworkPrivate
    Qt::Qml
    Qt::Quick
)
#! [ifcodegen_extend_target]

#! [ifcodegen_import_variables]
find_package(Qt6 COMPONENTS InterfaceFramework)

# Generate code and import variables into the
# current scope. The variables start with BOOK.
qt6_ifcodegen_import_variables(BOOK
    IDL_FILES addressbook.qface
    TEMPLATE frontend
)

# Use the import variables to build the module
qt_add_qml_module(addressbook_plugin
    OUTPUT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/${BOOK_URI_PATH}"
    URI ${BOOK_URI}
    VERSION ${BOOK_VERSION}
    PLUGIN_TARGET addressbook_plugin
    NO_PLUGIN_OPTIONAL
    NO_GENERATE_PLUGIN_SOURCE
    SOURCES
        ${BOOK_SOURCES}
)

target_link_libraries(AdressBook PUBLIC
    Qt::Core
    Qt::InterfaceFramework
    Qt::InterfaceFrameworkPrivate
    Qt::Qml
    Qt::Quick
)
#! [ifcodegen_import_variables]
