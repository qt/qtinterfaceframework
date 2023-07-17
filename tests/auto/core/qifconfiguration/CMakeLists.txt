#####################################################################
## tst_qifconfiguration Test:
#####################################################################

# Collect test data
file(GLOB_RECURSE test_data_glob
    RELATIVE ${CMAKE_CURRENT_SOURCE_DIR}
    testdata/*)
list(APPEND test_data ${test_data_glob})

qt_internal_add_test(tst_qifconfiguration
    SOURCES
        tst_qifconfiguration.cpp
    LIBRARIES
        Qt::Gui
        Qt::Qml
        Qt::InterfaceFramework
        Qt::InterfaceFrameworkPrivate
    TESTDATA ${test_data}
)
