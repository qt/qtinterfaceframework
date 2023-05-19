#####################################################################
## tst_qifsimulationengine Test:
#####################################################################

qt_internal_add_test(tst_qifsimulationengine
    SOURCES
        tst_qifsimulationengine.cpp
    LIBRARIES
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
    "FunctionTest.qml"
    "FunctionTestMain.qml"
)

qt_internal_add_resource(tst_qifsimulationengine "resource"
    PREFIX
        "/"
    FILES
        ${resource_resource_files}
)
