function(qt6_ifcodegen generator_target)
    qt6_ifcodegen_generate(${ARGN})
    qt6_ifcodegen_include(${generator_target} ${ARGN})
endfunction()

if(NOT QT_NO_CREATE_VERSIONLESS_FUNCTIONS)
    function(qt_ifcodegen)
        qt6_ifcodegen(${ARGV})
    endfunction()
endif()

macro(internal_resolve_ifcodegen_path)
    if (DEFINED QtInterfaceFramework_BINARY_DIR AND (NOT DEFINED QT_BUILD_STANDALONE_TESTS))
        set (VIRTUALENV ${QtInterfaceFramework_BINARY_DIR}/src/tools/ifcodegen/qtif_qface_virtualenv)
        set (GENERATOR_PATH ${QtInterfaceFramework_SOURCE_DIR}/src/tools/ifcodegen)
        set (IFGENERATOR_CONFIG ${QtInterfaceFramework_BINARY_DIR}/src/tools/ifcodegen/.config)
    else()
        set (GENERATOR_PATH ${QTIF_INSTALL_PREFIX}/${QT6_INSTALL_BINS}/ifcodegen)
        set (VIRTUALENV ${GENERATOR_PATH}/qtif_qface_virtualenv)
    endif()
endmacro()

function(qt6_ifcodegen_generate)
    internal_resolve_ifcodegen_path()

    if (QT_FEATURE_python3_virtualenv AND NOT QT_FEATURE_system_qface
        AND NOT EXISTS ${VIRTUALENV}/bin/activate AND NOT EXISTS ${VIRTUALENV}/Scripts/activate.bat)
        return()
    endif()

    cmake_parse_arguments(
        PARSE_ARGV 0
        ARG
        "VERBOSE" "QFACE_FORMAT;QFACE_OUTPUT_DIR;QFACE_SOURCES;QFACE_HEADERS_OUTPUT_DIR" "QFACE_ANNOTATIONS;QFACE_IMPORT_PATH"
    )

    if (DEFINED ARG_KEYWORDS_MISSING_VALUES)
        message(FATAL_ERROR "Keywords can't be empty: ${ARG_KEYWORDS_MISSING_VALUES}")
    endif()

    if (NOT DEFINED ARG_QFACE_FORMAT)
        set(ARG_QFACE_FORMAT frontend)
    endif()
    set(QFACE_OUTPUT_DIR ${ARG_QFACE_OUTPUT_DIR})
    if (NOT DEFINED ARG_QFACE_OUTPUT_DIR)
        set(QFACE_OUTPUT_DIR ${CMAKE_CURRENT_BINARY_DIR})
    endif()
    if (NOT DEFINED ARG_QFACE_SOURCES)
        message(FATAL_ERROR "QFACE_SOURCES can't be empty")
    endif()
    get_filename_component(QFACE_SOURCES "${ARG_QFACE_SOURCES}" REALPATH BASE_DIR)
    get_filename_component(QFACE_SOURCE_DIR "${QFACE_SOURCES}" DIRECTORY)
    get_filename_component(QFACE_BASE_NAME "${QFACE_SOURCES}" NAME_WLE)
    get_filename_component(QFACE_SOURCE_ANNOTATION ${QFACE_SOURCE_DIR}/${QFACE_BASE_NAME}.yaml REALPATH BASE_DIR)

    set(QFACE_TEMPLATE_PWD "${GENERATOR_PATH}/templates/${ARG_QFACE_FORMAT}")
    if(EXISTS ${QFACE_TEMPLATE_PWD})
        set(FORMAT_PATH ${QFACE_TEMPLATE_PWD})
        set(QFACE_FORMAT ${ARG_QFACE_FORMAT})
    else()
        get_filename_component(FORMAT_PATH "${ARG_QFACE_FORMAT}" REALPATH)
        if(EXISTS ${FORMAT_PATH})
            set(QFACE_FORMAT ${FORMAT_PATH})
        endif()
    endif()
    if (NOT DEFINED QFACE_FORMAT)
        message(FATAL_ERROR "Invalid QFACE_FORMAT: Couldn't find the template folder: ${FORMAT_PATH}")
    endif()

    set(IDE_FILES)

    # Register all source files to cause a cmake rerun
    set(GEN_DEPENDENCIES)
    list(APPEND GEN_DEPENDENCIES ${QFACE_SOURCES})
    list(APPEND IDE_FILES ${QFACE_SOURCES})
    if (EXISTS ${QFACE_SOURCE_ANNOTATION})
        list(APPEND GEN_DEPENDENCIES ${QFACE_SOURCE_ANNOTATION})
        list(APPEND IDE_FILES ${QFACE_SOURCE_ANNOTATION})
    endif()
    # Also register all files which are part of the current template
    file(GLOB FORMAT_FILES ${FORMAT_PATH}/*)
    list(APPEND GEN_DEPENDENCIES ${FORMAT_FILES})
    list(APPEND GEN_DEPENDENCIES ${FORMAT_PATH}.yaml)
    # Most templates also have a dependency to a common folder
    file(GLOB COMMON_FORMAT_FILES ${GENERATOR_PATH}/templates/*common*/*)
    list(APPEND GEN_DEPENDENCIES ${COMMON_FORMAT_FILES})

    set(GENERATOR_ARGUMENTS --format=${QFACE_FORMAT} --force)
    foreach(ANNOTATION ${ARG_QFACE_ANNOTATIONS})
        get_filename_component(ANNOTATION_PATH "${ANNOTATION}" REALPATH BASE_DIR)
        list(APPEND GENERATOR_ARGUMENTS -A ${ANNOTATION_PATH})
        # Dependency for regeneration
        list(APPEND GEN_DEPENDENCIES ${ANNOTATION_PATH})
        list(APPEND IDE_FILES ${ANNOTATION_PATH})
    endforeach()

    foreach(IMPORT ${ARG_QFACE_IMPORT_PATH})
        get_filename_component(IMPORT_PATH "${IMPORT}" REALPATH BASE_DIR)
        list(APPEND GENERATOR_ARGUMENTS -I ${IMPORT_PATH})
        # Dependency for regeneration
        file(GLOB QFACE_FILES ${IMPORT_PATH}/*.qface)
        list(APPEND GEN_DEPENDENCIES ${QFACE_FILES})
    endforeach()

    # Show qface and annotations in IDE
    # If the generate function is called directly ${generator_target} is not defined and we will
    # fallback to the IDL base name
    set(IDE_PREFIX ${generator_target})
    if (NOT IDE_PREFIX)
        set(IDE_PREFIX ${QFACE_BASE_NAME})
    endif()
    add_custom_target(${IDE_PREFIX}_qface_files SOURCES
        ${IDE_FILES}
    )

    set_property(DIRECTORY APPEND PROPERTY CMAKE_CONFIGURE_DEPENDS ${GEN_DEPENDENCIES})

    # If the generator was run successfully before
    # Check for the timestamps to determine when to run it again.
    set(RUN_GENERATOR FALSE)
    foreach(DEP ${GEN_DEPENDENCIES})
        if (${DEP} IS_NEWER_THAN ${QFACE_OUTPUT_DIR}/.stamp-ifcodegen)
            set(RUN_GENERATOR TRUE)
            break()
        endif()
    endforeach()

    if (RUN_GENERATOR)
        if (QT_FEATURE_python3_virtualenv AND NOT QT_FEATURE_system_qface)
            if ("${CMAKE_HOST_SYSTEM_NAME}" STREQUAL "Windows")
                set(PYTHON_EXECUTABLE ${VIRTUALENV}/Scripts/python.exe)
                file(TO_NATIVE_PATH "${VIRTUALENV}" VIRTUALENV)
            else()
                set(PYTHON_EXECUTABLE ${VIRTUALENV}/bin/python)
                set(ENV{LC_ALL} en_US.UTF-8)
                set(ENV{LD_LIBRARY_PATH} ${VIRTUALENV}/bin)
            endif()
            set(ENV{PYTHONHOME} ${VIRTUALENV})
            set(ENV{VIRTUAL_ENV} ${VIRTUALENV})
        else()
            qt_find_package(Python3 PROVIDED_TARGETS Python3::Interpreter MODULE_NAME interfaceframework)
            set(PYTHON_EXECUTABLE ${Python3_EXECUTABLE})
        endif()
        if (DEFINED IFGENERATOR_CONFIG)
            set(ENV{IFGENERATOR_CONFIG} ${IFGENERATOR_CONFIG})
        endif()

        message(STATUS "Running ifcodegen for ${QFACE_SOURCES} with template ${QFACE_FORMAT}")
        set(GENERATOR_CMD
                ${PYTHON_EXECUTABLE}
                ${GENERATOR_PATH}/generate.py
                ${GENERATOR_ARGUMENTS}
                ${QFACE_SOURCES}
                ${QFACE_OUTPUT_DIR}
            )
        list(JOIN GENERATOR_CMD " " GENERATOR_CMD_STR)
        execute_process(
                        COMMAND ${GENERATOR_CMD}
                        RESULT_VARIABLE RET_CODE
                        OUTPUT_VARIABLE GENERATOR_LOG
                        ERROR_VARIABLE GENERATOR_LOG
                       )

        message(VERBOSE ${GENERATOR_CMD_STR}\n${GENERATOR_LOG})
        # Touch the stamp file if the generator run was successful
        if("${RET_CODE}" EQUAL "0")
            if (ARG_VERBOSE OR IFCODEGEN_VERBOSE)
                message(${GENERATOR_CMD_STR}\n${GENERATOR_LOG})
            endif()
            execute_process(COMMAND ${CMAKE_COMMAND} -E touch ${QFACE_OUTPUT_DIR}/.stamp-ifcodegen)
        else()
            message(FATAL_ERROR "Error while running the ifcodegen:\n${GENERATOR_CMD_STR}\n${GENERATOR_LOG}")
        endif()
    endif()

    # hack for the developer-build to have all headers where the Qt build expects them
    if (DEFINED ARG_QFACE_HEADERS_OUTPUT_DIR)
        file(GLOB HEADER_FILES ${QFACE_OUTPUT_DIR}/*.h)
        file(COPY ${HEADER_FILES} DESTINATION ${ARG_QFACE_HEADERS_OUTPUT_DIR})
    endif()

endfunction()

if(NOT QT_NO_CREATE_VERSIONLESS_FUNCTIONS)
    function(qt_ifcodegen_generate)
        qt6_ifcodegen_generate(${ARGV})
    endfunction()
endif()

function(qt6_ifcodegen_include target)
    internal_resolve_ifcodegen_path()

    if (QT_FEATURE_python3_virtualenv AND NOT QT_FEATURE_system_qface
        AND NOT EXISTS ${VIRTUALENV}/bin/activate AND NOT EXISTS ${VIRTUALENV}/Scripts/activate.bat)
        # Create a dummy target instead
        if (NOT TARGET ${target} AND NOT TEST ${target})
            if (DEFINED QtInterfaceFramework_BINARY_DIR)
                file(TOUCH ${CMAKE_CURRENT_BINARY_DIR}/cmake_dummy.cpp)
                add_library(${target} ${CMAKE_CURRENT_BINARY_DIR}/cmake_dummy.cpp)
            else()
                message(FATAL_ERROR "No virtualenv environment to run the ifcodegen")
            endif()
        endif()
        return()
    endif()

    cmake_parse_arguments(
        PARSE_ARGV 1
        ARG
        "" "QFACE_SOURCES;QFACE_OUTPUT_DIR" ""
    )

    if (DEFINED ARG_KEYWORDS_MISSING_VALUES)
        message(FATAL_ERROR "Keywords can't be empty: ${ARG_KEYWORDS_MISSING_VALUES}")
    endif()

    set(QFACE_OUTPUT_DIR ${ARG_QFACE_OUTPUT_DIR})
    if (NOT DEFINED ARG_QFACE_OUTPUT_DIR)
        set(QFACE_OUTPUT_DIR ${CMAKE_CURRENT_BINARY_DIR})
    endif()
    if (NOT DEFINED ARG_QFACE_SOURCES)
        message(FATAL_ERROR "QFACE_SOURCES can't be empty")
    endif()
    get_filename_component(QFACE_SOURCES "${ARG_QFACE_SOURCES}" REALPATH BASE_DIR)
    get_filename_component(QFACE_BASE_NAME "${ARG_QFACE_SOURCES}" NAME_WLE)

    set(CURRENT_TARGET "${target}")
    include(${QFACE_OUTPUT_DIR}/${QFACE_BASE_NAME}.cmake)
endfunction()

if(NOT QT_NO_CREATE_VERSIONLESS_FUNCTIONS)
    function(qt_ifcodegen_include)
        qt6_ifcodegen_include(${ARGV})
    endfunction()
endif()
