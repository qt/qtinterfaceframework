# Generated from qifpendingreply.pro.

#####################################################################
## tst_qifpendingreply Test:
#####################################################################

# Collect test data
file(GLOB_RECURSE test_data_glob
    RELATIVE ${CMAKE_CURRENT_SOURCE_DIR}
    testdata/*)
list(APPEND test_data ${test_data_glob})

qt_internal_add_test(tst_qifpendingreply
    SOURCES
        tst_qifpendingreply.cpp
    DEFINES
        SRCDIR=\\\"${CMAKE_CURRENT_SOURCE_DIR}/\\\"
    PUBLIC_LIBRARIES
        Qt::Gui
        Qt::InterfaceFramework
        Qt::Qml
    TESTDATA ${test_data}
)

#### Keys ignored in scope 1:.:.:qifpendingreply.pro:<TRUE>:
# OTHER_FILES = "testdata/*"
# QMAKE_PROJECT_NAME = "$$TARGET"
# TEMPLATE = "app"
