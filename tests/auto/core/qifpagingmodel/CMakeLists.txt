#####################################################################
## tst_qifpagingmodel Test:
#####################################################################

qt_internal_add_test(tst_qifpagingmodel
    SOURCES
        tst_qifpagingmodel.cpp
    LIBRARIES
        Qt::CorePrivate
        Qt::Gui
        Qt::InterfaceFramework
        Qt::InterfaceFrameworkPrivate
        Qt::Quick
)
