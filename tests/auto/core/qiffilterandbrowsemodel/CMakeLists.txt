#####################################################################
## tst_qiffilterandbrowsemodel Test:
#####################################################################

qt_internal_add_test(tst_qiffilterandbrowsemodel
    SOURCES
        tst_qiffilterandbrowsemodel.cpp
    DEFINES
        SRCDIR=\\\"${CMAKE_CURRENT_SOURCE_DIR}/\\\"
    LIBRARIES
        Qt::Gui
        Qt::InterfaceFramework
        Qt::Quick
)

qt_add_resources(tst_qiffilterandbrowsemodel "qml"
    PREFIX "/"
    FILES testdata/listview.qml
)
