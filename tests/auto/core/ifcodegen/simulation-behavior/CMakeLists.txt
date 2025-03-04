set(CMAKE_AUTOMOC ON)

add_subdirectory(frontend)
add_subdirectory(qmlplugin)
add_subdirectory(simulator)

#####################################################################
## tst_simulation-behavior Test:
#####################################################################

# Collect test data
file(GLOB_RECURSE test_data_glob
    RELATIVE ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/data/*.qml)
list(APPEND test_data ${test_data_glob})

qt_internal_add_test(tst_simulation-behavior
    GUI
    QMLTEST
    SOURCES
        tst_simulation-behavior.cpp
    LIBRARIES
        Qt::Gui

    TESTDATA ${test_data}
    QML_IMPORTPATH
            ${CMAKE_CURRENT_BINARY_DIR}/imports/
)
set_property(TARGET tst_simulation-behavior APPEND PROPERTY QT_QML_IMPORT_PATH  "${CMAKE_CURRENT_BINARY_DIR}/imports/")

#####################################################################
## Scopes:
#####################################################################

qt_internal_extend_target(tst_simulation-behavior CONDITION ANDROID OR IOS
    DEFINES
        QT_QMLTEST_DATADIR=":/data"
)

qt_internal_extend_target(tst_simulation-behavior  CONDITION NOT ANDROID AND NOT IOS
    DEFINES
        QT_QMLTEST_DATADIR="${CMAKE_CURRENT_SOURCE_DIR}/data"
)

# QT_ANDROID_EXTRA_PLUGINS requires a list of directories, not files!
set_target_properties(tst_simulation-behavior PROPERTIES
    QT_ANDROID_EXTRA_PLUGINS "${CMAKE_CURRENT_BINARY_DIR}/interfaceframework"
)

