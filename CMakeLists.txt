cmake_minimum_required(VERSION 3.16)

include(.cmake.conf)
project(QtInterfaceFramework
    VERSION "${QT_REPO_MODULE_VERSION}"
    DESCRIPTION "Qt Interface Framework Libraries"
    HOMEPAGE_URL "https://qt.io/"
    LANGUAGES CXX C
)

# Make sure we only use latest private CMake API, aka no compatibility wrappers.
set(QT_NO_INTERNAL_COMPATIBILITY_FUNCTIONS TRUE)

find_package(Qt6 ${PROJECT_VERSION} CONFIG REQUIRED COMPONENTS BuildInternals Core)
find_package(Qt6 ${PROJECT_VERSION} CONFIG OPTIONAL_COMPONENTS Gui Qml Quick QuickTest RemoteObjects DBus Widgets)
qt_internal_project_setup()

find_package(FLEX)

include(src/interfaceframework/Qt6InterfaceFrameworkMacros.cmake)

if(INTEGRITY)
    message(NOTICE "Skipping the build as the condition \"NOT INTEGRITY\" is not met.")
    return()
endif()
if(WINRT)
    message(NOTICE "Skipping the build as the condition \"NOT WINRT\" is not met.")
    return()
endif()

if(IF_COVERAGE)
    if(NOT (LINUX AND (GCC OR CLANG)))
        message(FATAL_ERROR "Coverage builds are only supported on Linux, using GCC or Clang.")
        return()
    endif()

    set(QT_BUILD_TESTS ON)
    set(GCC_COVERAGE_COMPILE_FLAGS "-fprofile-arcs -ftest-coverage")
    set(GCC_COVERAGE_LINK_FLAGS    "-fprofile-arcs -fprofile-generate -lgcov --coverage")
    set(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} ${GCC_COVERAGE_COMPILE_FLAGS}")
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${GCC_COVERAGE_LINK_FLAGS}")

    set(GCOV_EXCLUDE
        '/usr/*'
        '${QT_BUILD_INTERNALS_RELOCATABLE_INSTALL_PREFIX}/*'
        '${CMAKE_CURRENT_LIST_DIR}/tests/*'
        '${CMAKE_CURRENT_LIST_DIR}/examples/*'
        '${CMAKE_CURRENT_LIST_DIR}/src/3rdparty/*'
        'moc_*'
        '*.l'
        '${CMAKE_CURRENT_BINARY_DIR}/*'
    )

    if(QT_SOURCE_TREE)
        string(REGEX REPLACE "/qtbase$" "" qtSrcDir ${QT_SOURCE_TREE})
        list(APPEND GCOV_EXCLUDE '${qtSrcDir}/*')
    endif()

    add_custom_target(check_coverage
        find . -name \"*.gcov-info\" -print0 | xargs -0 rm -f &&
        lcov -c -i -d . --rc lcov_branch_coverage=1 --rc geninfo_auto_base=1 -o "${CMAKE_CURRENT_BINARY_DIR}/base.gcov-info" >/dev/null 2>&1 &&
        ctest --test-dir tests/auto --verbose &&
        lcov -c -d . --rc lcov_branch_coverage=1 --rc geninfo_auto_base=1 -o "${CMAKE_CURRENT_BINARY_DIR}/test.gcov-info" >/dev/null &&
        lcov --rc lcov_branch_coverage=1 -o "${CMAKE_CURRENT_BINARY_DIR}/temp.gcov-info" `find . -name \"*.gcov-info\" | xargs -n1 echo -a` >/dev/null &&
        lcov --rc lcov_branch_coverage=1 -o "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}.gcov-info" -r temp.gcov-info ${GCOV_EXCLUDE} >/dev/null &&
        rm -f base.gcov-info test.gcov-info temp.gcov-info &&
        genhtml -o branch-coverage -s -f --legend --branch-coverage --rc lcov_branch_coverage=1 --demangle-cpp ${PROJECT_NAME}.gcov-info >/dev/null &&
        echo \"\\n\\nCoverage info is available at file://`pwd`/branch-coverage/index.html\\n\"

        DEPENDS
            tests/auto/install
        USES_TERMINAL
    )
endif()

if (IF_SANITIZE)
    if (MSVC)
        set(ECM_ENABLE_SANITIZERS address)
    else()
        set(ECM_ENABLE_SANITIZERS address undefined)
    endif()
    include(3rdparty/extra-cmake-modules/modules/ECMEnableSanitizers)
endif()


qt_build_repo()

if(NOT QT_BUILD_STANDALONE_TESTS)
    # Copy mkspecs for users preferring qmake builds
    set(mkspecs_install_dir "${INSTALL_MKSPECSDIR}")
    qt_path_join(mkspecs_install_dir ${QT_INSTALL_DIR} ${mkspecs_install_dir})

    qt_copy_or_install(DIRECTORY mkspecs/
        DESTINATION "${mkspecs_install_dir}"
        FILES_MATCHING PATTERN "*.pr[if]"
    )
endif()
