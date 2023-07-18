
qt_internal_add_test(tst_legacyqmlregistration
    SOURCES
        tst_legacyqmlregistration.cpp
    LIBRARIES
        Qt::Gui
        Qt::InterfaceFramework
        Qt::InterfaceFrameworkPrivate
        Qt::Qml
)

# This dummy QML file is not used by the test itself, but helps
# to let tools like androiddeployqt know what QML dependencies we
# have and package everything correctly
qt_add_resources(tst_legacyqmlregistration "qml"
    PREFIX "/"
    FILES qml_dependencies.qml
)

