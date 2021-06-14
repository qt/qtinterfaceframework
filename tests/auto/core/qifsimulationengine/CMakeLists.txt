# Generated from qifsimulationengine.pro.

#####################################################################
## tst_qifsimulationengine Test:
#####################################################################

qt_internal_add_test(tst_qifsimulationengine
    SOURCES
        tst_qifsimulationengine.cpp
    PUBLIC_LIBRARIES
        Qt::Gui
        Qt::InterfaceFramework
        Qt::InterfaceFrameworkPrivate
        Qt::Qml
)

# Resources:
set(resource_resource_files
    "invalid-data.json"
    "simple.json"
    "simple.qml"
)

qt_internal_add_resource(tst_qifsimulationengine "resource"
    PREFIX
        "/"
    FILES
        ${resource_resource_files}
)


#### Keys ignored in scope 1:.:.:qifsimulationengine.pro:<TRUE>:
# QMAKE_PROJECT_NAME = "$$TARGET"
# TEMPLATE = "app"
