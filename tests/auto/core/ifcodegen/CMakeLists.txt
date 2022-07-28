# Generated from ifcodegen.pro.

# Skip building these tests in a top-level build configured with -make tests
# to avoid configuration errors like
#  CMake Error: AUTOMOC for target tst_template_full_path: The "moc"
#  executable ".../qtbase/libexec/moc" does not exist.
#
# It happens because targets are added using non-internal API which do not call
# qt_autogen_tools_initial_setup.
if(QT_SUPERBUILD)
    return()
endif()

add_subdirectory(custom-template)
add_subdirectory(org-example-echo)
add_subdirectory(flat-cmake-hierarchy-test)
add_subdirectory(org-example-echo-noprivate)
add_subdirectory(org-example-echo-noanno)
add_subdirectory(include-test)
if(QT_FEATURE_remoteobjects)
    add_subdirectory(org-example-echo-qtro)
endif()

# Only do this when using standalone tests, as we require all qmake module files to be in place
# The CMAKE_OSX_ARCHITECTURES is defined when building several architectures in one go
# Running qmake in that setup doesn't work
if (QT_BUILD_STANDALONE_TESTS OR QT_BUILD_QMAKE_TEST)
    if (NOT CMAKE_CROSSCOMPILING AND NOT CMAKE_OSX_ARCHITECTURES)
        include(ProcessorCount)
        ProcessorCount(CPU_COUNT)
        if(CPU_COUNT EQUAL 0)
            set(CPU_COUNT 4)
        endif()
        set(MAKE_ARGS "-j${CPU_COUNT}")

        if (MSVC)
            set(MAKE_CMD "jom")
        else()
            set(MAKE_CMD "make")
        endif()

        find_program(MAKE_PATH NAMES ${MAKE_CMD} HINTS $ENV{CI_JOM_PATH})
        if (MAKE_PATH STREQUAL "MAKE_PATH-NOTFOUND")
            message(STATUS "Disabling qmake autotests: Couldn't find ${MAKE_CMD} binary")
            return()
        endif()

        include(ExternalProject)
        ExternalProject_Add(ifcodegen-qmake
          SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}
          CONFIGURE_COMMAND ${QT_CMAKE_EXPORT_NAMESPACE}::qmake ${CMAKE_CURRENT_SOURCE_DIR}
          BUILD_COMMAND ${CMAKE_COMMAND} -E env --unset=MAKEFLAGS ${MAKE_PATH} ${MAKE_ARGS}
          INSTALL_COMMAND ""
          TEST_COMMAND ${CMAKE_COMMAND} -E env --unset=MAKEFLAGS ${MAKE_PATH} check
        )
    endif()
endif()
