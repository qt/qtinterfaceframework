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
    set(QT_BUILD_TESTS_BY_DEFAULT ON)
    add_compile_options(-fprofile-arcs -ftest-coverage -fprofile-update=atomic)
    add_link_options(-fprofile-arcs -fprofile-generate --coverage)
    add_compile_definitions(QT_IF_COVERAGE)
    link_libraries(gcov)

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

    if(QT_BUILD_STANDALONE_TESTS)
        # Coin splits the build into two separate directories, so we need to merge the coverage info
        set(GCOV_EXTRA_FOLDER "../qtinterfaceframework_build")
        set(LCOV_EXTRA_ARGS -d ${GCOV_EXTRA_FOLDER})
        list(APPEND GCOV_EXCLUDE '/home/qt/work/qt/qtinterfaceframework_build/*')
    endif()
    set(COVERAGE_DEPENDS all)
    if(QT_WILL_INSTALL)
        list(APPEND COVERAGE_DEPENDS tests/auto/install)
    endif()

    add_custom_target(check_coverage
        COMMAND sh -c 'which lcov >/dev/null || { echo "The lcov tool is not available." && exit 2 \; }'
        COMMAND sh -c 'which genhtml >/dev/null || { echo "The genhtml tool is not available." && exit 2 \; }'
        COMMAND find . \\\( -name \"*.gcov-info\" -o -name \"*.gcda\" \\\) -print0 | xargs -0 rm -f
        COMMAND lcov -keep-going -c -i -d . ${LCOV_EXTRA_ARGS} --rc branch_coverage=1 --rc geninfo_auto_base=1 -o "${CMAKE_CURRENT_BINARY_DIR}/base.gcov-info"
        COMMAND ctest --test-dir tests/auto --verbose --repeat until-pass:3
        COMMAND lcov -keep-going -c -d . ${LCOV_EXTRA_ARGS} --rc branch_coverage=1 --rc geninfo_auto_base=1 -o "${CMAKE_CURRENT_BINARY_DIR}/test.gcov-info"
        COMMAND lcov -keep-going --rc branch_coverage=1 -o "${CMAKE_CURRENT_BINARY_DIR}/temp.gcov-info" `find . ${GCOV_EXTRA_FOLDER} -name \"*.gcov-info\" | xargs -n1 echo -a`
        COMMAND lcov -keep-going --rc branch_coverage=1 -o "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}.gcov-info" -r temp.gcov-info ${GCOV_EXCLUDE}
        COMMAND rm -f base.gcov-info test.gcov-info temp.gcov-info
        COMMAND genhtml -o coverage-report -s -f -t "${PROJECT_NAME}" --legend --branch-coverage --rc branch_coverage=1 --rc geninfo_unexecuted_blocks=1 --demangle-cpp "c++filt" ${PROJECT_NAME}.gcov-info
        COMMAND echo \"\\n\\nCoverage info is available at file://`pwd`/coverage-report/index.html\\n\"

        DEPENDS ${COVERAGE_DEPENDS}
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
