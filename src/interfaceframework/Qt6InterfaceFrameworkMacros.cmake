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
        "VERBOSE" "IFCODEGEN_TEMPLATE;IFCODEGEN_OUTPUT_DIR;IFCODEGEN_SOURCES;IFCODEGEN_HEADERS_OUTPUT_DIR" "IFCODEGEN_ANNOTATIONS;IFCODEGEN_IMPORT_PATH"
    )

    if (DEFINED ARG_KEYWORDS_MISSING_VALUES)
        message(FATAL_ERROR "Keywords can't be empty: ${ARG_KEYWORDS_MISSING_VALUES}")
    endif()

    if (NOT DEFINED ARG_IFCODEGEN_TEMPLATE)
        set(ARG_IFCODEGEN_TEMPLATE frontend)
    endif()
    set(IFCODEGEN_OUTPUT_DIR ${ARG_IFCODEGEN_OUTPUT_DIR})
    if (NOT DEFINED ARG_IFCODEGEN_OUTPUT_DIR)
        set(IFCODEGEN_OUTPUT_DIR ${CMAKE_CURRENT_BINARY_DIR})
    endif()
    if (NOT DEFINED ARG_IFCODEGEN_SOURCES)
        message(FATAL_ERROR "IFCODEGEN_SOURCES can't be empty")
    endif()
    get_filename_component(IFCODEGEN_SOURCES "${ARG_IFCODEGEN_SOURCES}" REALPATH BASE_DIR)
    get_filename_component(IFCODEGEN_SOURCE_DIR "${IFCODEGEN_SOURCES}" DIRECTORY)
    get_filename_component(IFCODEGEN_BASE_NAME "${IFCODEGEN_SOURCES}" NAME_WLE)
    get_filename_component(IFCODEGEN_SOURCE_ANNOTATION ${IFCODEGEN_SOURCE_DIR}/${IFCODEGEN_BASE_NAME}.yaml REALPATH BASE_DIR)

    set(IFCODEGEN_TEMPLATE_PWD "${GENERATOR_PATH}/templates/${ARG_IFCODEGEN_TEMPLATE}")
    if(EXISTS ${IFCODEGEN_TEMPLATE_PWD})
        set(TEMPLATE_PATH ${IFCODEGEN_TEMPLATE_PWD})
        set(IFCODEGEN_TEMPLATE ${ARG_IFCODEGEN_TEMPLATE})
    else()
        get_filename_component(TEMPLATE_PATH "${ARG_IFCODEGEN_TEMPLATE}" REALPATH)
        if(EXISTS ${TEMPLATE_PATH})
            set(IFCODEGEN_TEMPLATE ${TEMPLATE_PATH})
        endif()
    endif()
    if (NOT DEFINED IFCODEGEN_TEMPLATE)
        message(FATAL_ERROR "Invalid IFCODEGEN_TEMPLATE: Couldn't find the template folder: ${TEMPLATE_PATH}")
    endif()

    set(IDE_FILES)

    # Register all source files to cause a cmake rerun
    set(GEN_DEPENDENCIES)
    list(APPEND GEN_DEPENDENCIES ${IFCODEGEN_SOURCES})
    list(APPEND IDE_FILES ${IFCODEGEN_SOURCES})
    if (EXISTS ${IFCODEGEN_SOURCE_ANNOTATION})
        list(APPEND GEN_DEPENDENCIES ${IFCODEGEN_SOURCE_ANNOTATION})
        list(APPEND IDE_FILES ${IFCODEGEN_SOURCE_ANNOTATION})
    endif()
    # Also register all files which are part of the current template
    file(GLOB TEMPLATE_FILES ${TEMPLATE_PATH}/*)
    list(APPEND GEN_DEPENDENCIES ${TEMPLATE_FILES})
    list(APPEND GEN_DEPENDENCIES ${TEMPLATE_PATH}.yaml)
    # Most templates also have a dependency to a common folder
    file(GLOB COMMON_TEMPLATE_FILES ${GENERATOR_PATH}/templates/*common*/*)
    list(APPEND GEN_DEPENDENCIES ${COMMON_TEMPLATE_FILES})

    set(GENERATOR_ARGUMENTS --template=${IFCODEGEN_TEMPLATE} --force)
    foreach(ANNOTATION ${ARG_IFCODEGEN_ANNOTATIONS})
        get_filename_component(ANNOTATION_PATH "${ANNOTATION}" REALPATH BASE_DIR)
        list(APPEND GENERATOR_ARGUMENTS -A ${ANNOTATION_PATH})
        # Dependency for regeneration
        list(APPEND GEN_DEPENDENCIES ${ANNOTATION_PATH})
        list(APPEND IDE_FILES ${ANNOTATION_PATH})
    endforeach()

    foreach(IMPORT ${ARG_IFCODEGEN_IMPORT_PATH})
        get_filename_component(IMPORT_PATH "${IMPORT}" REALPATH BASE_DIR)
        list(APPEND GENERATOR_ARGUMENTS -I ${IMPORT_PATH})
        # Dependency for regeneration
        file(GLOB IFCODEGEN_FILES ${IMPORT_PATH}/*.qface)
        list(APPEND GEN_DEPENDENCIES ${IFCODEGEN_FILES})
    endforeach()

    # Show qface and annotations in IDE
    # If the generate function is called directly ${generator_target} is not defined and we will
    # fallback to the IDL base name
    set(IDE_PREFIX ${generator_target})
    if (NOT IDE_PREFIX)
        set(IDE_PREFIX ${IFCODEGEN_BASE_NAME})
    endif()
    add_custom_target(${IDE_PREFIX}_qface_files SOURCES
        ${IDE_FILES}
    )

    set_property(DIRECTORY APPEND PROPERTY CMAKE_CONFIGURE_DEPENDS ${GEN_DEPENDENCIES})

    # If the generator was run successfully before
    # Check for the timestamps to determine when to run it again.
    set(RUN_GENERATOR FALSE)
    foreach(DEP ${GEN_DEPENDENCIES})
        if (${DEP} IS_NEWER_THAN ${IFCODEGEN_OUTPUT_DIR}/.stamp-ifcodegen)
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

        message(STATUS "Running ifcodegen for ${IFCODEGEN_SOURCES} with template ${IFCODEGEN_TEMPLATE}")
        set(GENERATOR_CMD
                ${PYTHON_EXECUTABLE}
                ${GENERATOR_PATH}/generate.py
                ${GENERATOR_ARGUMENTS}
                ${IFCODEGEN_SOURCES}
                ${IFCODEGEN_OUTPUT_DIR}
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
            execute_process(COMMAND ${CMAKE_COMMAND} -E touch ${IFCODEGEN_OUTPUT_DIR}/.stamp-ifcodegen)
        else()
            message(FATAL_ERROR "Error while running the ifcodegen:\n${GENERATOR_CMD_STR}\n${GENERATOR_LOG}")
        endif()
    endif()

    # hack for the developer-build to have all headers where the Qt build expects them
    if (DEFINED ARG_IFCODEGEN_HEADERS_OUTPUT_DIR)
        file(GLOB HEADER_FILES ${IFCODEGEN_OUTPUT_DIR}/*.h)
        file(COPY ${HEADER_FILES} DESTINATION ${ARG_IFCODEGEN_HEADERS_OUTPUT_DIR})
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
        "" "IFCODEGEN_SOURCES;IFCODEGEN_OUTPUT_DIR" ""
    )

    if (DEFINED ARG_KEYWORDS_MISSING_VALUES)
        message(FATAL_ERROR "Keywords can't be empty: ${ARG_KEYWORDS_MISSING_VALUES}")
    endif()

    set(IFCODEGEN_OUTPUT_DIR ${ARG_IFCODEGEN_OUTPUT_DIR})
    if (NOT DEFINED ARG_IFCODEGEN_OUTPUT_DIR)
        set(IFCODEGEN_OUTPUT_DIR ${CMAKE_CURRENT_BINARY_DIR})
    endif()
    if (NOT DEFINED ARG_IFCODEGEN_SOURCES)
        message(FATAL_ERROR "IFCODEGEN_SOURCES can't be empty")
    endif()
    get_filename_component(IFCODEGEN_SOURCES "${ARG_IFCODEGEN_SOURCES}" REALPATH BASE_DIR)
    get_filename_component(IFCODEGEN_BASE_NAME "${ARG_IFCODEGEN_SOURCES}" NAME_WLE)

    set(CURRENT_TARGET "${target}")
    include(${IFCODEGEN_OUTPUT_DIR}/${IFCODEGEN_BASE_NAME}.cmake)
endfunction()

if(NOT QT_NO_CREATE_VERSIONLESS_FUNCTIONS)
    function(qt_ifcodegen_include)
        qt6_ifcodegen_include(${ARGV})
    endfunction()
endif()
