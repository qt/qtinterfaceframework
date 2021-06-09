# Generated from qifabstractfeature.pro.

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
    DEFINES
        SRCDIR=\\\"${CMAKE_CURRENT_SOURCE_DIR}/\\\"
    PUBLIC_LIBRARIES
        Qt::Gui
        Qt::InterfaceFramework
        Qt::InterfaceFrameworkPrivate
        Qt::Qml
    TESTDATA ${test_data}
)

#### Keys ignored in scope 1:.:.:qifabstractfeature.pro:<TRUE>:
# QMAKE_PROJECT_NAME = "$$TARGET"
# TEMPLATE = "app"
