#####################################################################
## tst_qifabstractfeature Test:
#####################################################################

qt_internal_add_test(tst_qifabstractfeature
    SOURCES
        qiffeaturetester.h
        tst_qifabstractfeature.cpp
    LIBRARIES
        Qt::Gui
        Qt::InterfaceFramework
        Qt::InterfaceFrameworkPrivate
        Qt::Qml
    TESTDATA ${test_data}
    NO_BATCH
)

qt_add_resources(tst_qifabstractfeature "qml"
    PREFIX "/"
    FILES
        testdata/autodiscovery.qml
        testdata/loader.qml
)
