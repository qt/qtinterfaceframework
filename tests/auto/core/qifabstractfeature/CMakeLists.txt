#####################################################################
## tst_qifabstractfeature Test:
#####################################################################

# Collect test data
file(GLOB_RECURSE test_data_glob
    RELATIVE ${CMAKE_CURRENT_SOURCE_DIR}
    testdata/*)
list(APPEND test_data ${test_data_glob})

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
