#####################################################################
## Internal API
#####################################################################

macro(internal_resolve_ifcodegen_path)
    if (DEFINED QtInterfaceFramework_BINARY_DIR AND (NOT DEFINED QT_BUILD_STANDALONE_TESTS))
        set (_VIRTUALENV ${QtInterfaceFramework_BINARY_DIR}/src/tools/ifcodegen/qtif_qface_virtualenv)
        set (_GENERATOR_PATH ${QtInterfaceFramework_SOURCE_DIR}/src/tools/ifcodegen)
        set (_IFGENERATOR_CONFIG ${QtInterfaceFramework_BINARY_DIR}/src/tools/ifcodegen/.config)
    else()
        set (_GENERATOR_PATH ${QTIF_INSTALL_PREFIX}/${QT6_INSTALL_BINS}/ifcodegen)
        set (_VIRTUALENV ${_GENERATOR_PATH}/qtif_qface_virtualenv)
    endif()
endmacro()

function(internal_ifcodegen_import)
    cmake_parse_arguments(
        PARSE_ARGV 0
        ARG
        "NO_GENERATE;VERBOSE" "TARGET;PREFIX;IDL_FILES;OUTPUT_DIR" ""
    )

    if (DEFINED ARG_KEYWORDS_MISSING_VALUES)
        message(FATAL_ERROR "Keywords can't be empty: ${ARG_KEYWORDS_MISSING_VALUES}")
    endif()

    if (ARG_TARGET AND NOT TARGET "${ARG_TARGET}")
         message(FATAL_ERROR "Trying to extend non-existing target \"${ARG_TARGET}\".")
    endif()

    if (NOT ARG_TARGET AND NOT ARG_PREFIX)
        message(FATAL_ERROR "Either PREFIX or TARGET needs to be provided to this function")
    endif()

    if (NOT ARG_NO_GENERATE)
        set(generator_target ${ARG_TARGET})
        qt6_ifcodegen_generate(${ARGN})
    endif()

    set(OUTPUT_DIR ${ARG_OUTPUT_DIR})
    if (NOT DEFINED ARG_OUTPUT_DIR)
        set(OUTPUT_DIR ${CMAKE_CURRENT_BINARY_DIR})
    endif()
    if (NOT DEFINED ARG_IDL_FILES)
        message(FATAL_ERROR
            "Called without input files. Please specify some using the IDL_FILES argument."
            )
    endif()
    get_filename_component(IDL_FILES "${ARG_IDL_FILES}" REALPATH BASE_DIR)
    get_filename_component(IFCODEGEN_BASE_NAME "${ARG_IDL_FILES}" NAME_WLE)

    if (ARG_PREFIX)
        set(VAR_PREFIX "${ARG_PREFIX}")
    else()
        set(VAR_PREFIX "_${ARG_TARGET}")
    endif()

    if (ARG_TARGET)
        set(CURRENT_TARGET "${ARG_TARGET}")
    endif()
    include(${OUTPUT_DIR}/${IFCODEGEN_BASE_NAME}.cmake)

    if (ARG_VERBOSE)
        message("Successfully imported the following variables:")
        list(APPEND CMAKE_MESSAGE_INDENT "  ")
        get_cmake_property(ALL_VARS VARIABLES)
        foreach (VAR ${ALL_VARS})
            if (VAR MATCHES "^${VAR_PREFIX}_*")
                message("${VAR}: ${${VAR}}")
            endif()
        endforeach()
        list(POP_BACK CMAKE_MESSAGE_INDENT)
   endif()
endfunction()

#####################################################################
## Public API
#####################################################################

macro(set_ifcodegen_variable name)
    set(${name} ${ARGN})
    set(${name} ${ARGN} PARENT_SCOPE)
endmacro()

# Calls the interfaceframework code generator.
#
# IDL_FILES: The input IDL file for the generator. (REQUIRED)
#
# TEMPLATE: The template used by the code generator. (REQUIRED)
#
# ANNOTATION_FILES: List of additional annotation files, which should be passed
#   to the generator. (OPTIONAL)
#
# IMPORT_PATH: List of additional directories, where included IDL files are
#   searched for. (OPTIONAL)
#
# OUTPUT_DIR: Overwrite the default output path. By default the generated code
#   will be put in ${CMAKE_CURRENT_BINARY_DIR}. Use this argument to provide
#   a replacement. (OPTIONAL)
#
# EXTRA_HEADERS_OUTPUT_DIR: An additional location where all headers will be
#   copied to. (OPTIONAL)
#
# VERBOSE: Print additional messages during generation. Useful for debugging
#   purposes. (OPTIONAL)
#
function(qt6_ifcodegen_generate)
    internal_resolve_ifcodegen_path()

    if (QT_FEATURE_python3_virtualenv AND NOT QT_FEATURE_system_qface
        AND NOT EXISTS ${_VIRTUALENV}/bin/activate AND NOT EXISTS ${_VIRTUALENV}/Scripts/activate.bat)
        return()
    endif()

    cmake_parse_arguments(
        PARSE_ARGV 0
        ARG
        # TARGET and PREFIX are not handled here, but we don't want the UNPARSED_ARGUMENTS error to
        # be triggered when provided (when called through the internal_ifcodegen_import() function
        "VERBOSE" "IDL_FILES;TEMPLATE;MODULE_NAME;OUTPUT_DIR;EXTRA_HEADERS_OUTPUT_DIR;TARGET;PREFIX" "ANNOTATION_FILES;IMPORT_PATH"
    )

    if (DEFINED ARG_KEYWORDS_MISSING_VALUES)
        message(FATAL_ERROR "Keywords can't be empty: ${ARG_KEYWORDS_MISSING_VALUES}")
    endif()
    if(ARG_UNPARSED_ARGUMENTS)
        message(FATAL_ERROR "Unknown/unexpected arguments: ${ARG_UNPARSED_ARGUMENTS}")
    endif()

    if (NOT DEFINED ARG_TEMPLATE)
        message(FATAL_ERROR
            "Called without generator template. Please specify one using the TEMPLATE argument."
            )
    endif()
    set(OUTPUT_DIR ${ARG_OUTPUT_DIR})
    if (NOT DEFINED ARG_OUTPUT_DIR)
        set(OUTPUT_DIR ${CMAKE_CURRENT_BINARY_DIR})
    endif()
    if (NOT DEFINED ARG_IDL_FILES)
        message(FATAL_ERROR
            "Called without input files. Please specify some using the IDL_FILES argument."
            )
    endif()
    get_filename_component(IDL_FILES "${ARG_IDL_FILES}" REALPATH BASE_DIR)
    get_filename_component(IFCODEGEN_SOURCE_DIR "${IDL_FILES}" DIRECTORY)
    get_filename_component(IFCODEGEN_BASE_NAME "${IDL_FILES}" NAME_WLE)
    get_filename_component(IFCODEGEN_SOURCE_ANNOTATION ${IFCODEGEN_SOURCE_DIR}/${IFCODEGEN_BASE_NAME}.yaml REALPATH BASE_DIR)

    set(TEMPLATE_PWD "${_GENERATOR_PATH}/templates/${ARG_TEMPLATE}")
    if(EXISTS ${TEMPLATE_PWD})
        set(TEMPLATE_PATH ${TEMPLATE_PWD})
        set(TEMPLATE ${ARG_TEMPLATE})
    else()
        get_filename_component(TEMPLATE_PATH "${ARG_TEMPLATE}" REALPATH)
        if(EXISTS ${TEMPLATE_PATH})
            set(TEMPLATE ${TEMPLATE_PATH})
        endif()
    endif()
    if (NOT DEFINED TEMPLATE)
        message(FATAL_ERROR "Invalid TEMPLATE: Couldn't find the template folder: ${TEMPLATE_PATH}")
    endif()

    set(IDE_FILES)

    # Register all source files to cause a cmake rerun
    set(GEN_DEPENDENCIES)
    list(APPEND GEN_DEPENDENCIES ${IDL_FILES})
    list(APPEND IDE_FILES ${IDL_FILES})
    if (EXISTS ${IFCODEGEN_SOURCE_ANNOTATION})
        list(APPEND GEN_DEPENDENCIES ${IFCODEGEN_SOURCE_ANNOTATION})
        list(APPEND IDE_FILES ${IFCODEGEN_SOURCE_ANNOTATION})
    endif()
    # Also register all files which are part of the current template
    file(GLOB TEMPLATE_FILES ${TEMPLATE_PATH}/*)
    list(APPEND GEN_DEPENDENCIES ${TEMPLATE_FILES})
    list(APPEND GEN_DEPENDENCIES ${TEMPLATE_PATH}.yaml)
    # Most templates also have a dependency to a common folder
    file(GLOB COMMON_TEMPLATE_FILES ${_GENERATOR_PATH}/templates/*common*/*)
    list(APPEND GEN_DEPENDENCIES ${COMMON_TEMPLATE_FILES})

    set(GENERATOR_ARGUMENTS --template=${TEMPLATE} --force)
    foreach(ANNOTATION ${ARG_ANNOTATION_FILES})
        get_filename_component(ANNOTATION_PATH "${ANNOTATION}" REALPATH BASE_DIR)
        list(APPEND GENERATOR_ARGUMENTS -A ${ANNOTATION_PATH})
        # Dependency for regeneration
        list(APPEND GEN_DEPENDENCIES ${ANNOTATION_PATH})
        list(APPEND IDE_FILES ${ANNOTATION_PATH})
    endforeach()

    foreach(IMPORT ${ARG_IMPORT_PATH})
        get_filename_component(IMPORT_PATH "${IMPORT}" REALPATH BASE_DIR)
        list(APPEND GENERATOR_ARGUMENTS -I ${IMPORT_PATH})
        # Dependency for regeneration
        file(GLOB IFCODEGEN_FILES ${IMPORT_PATH}/*.qface)
        list(APPEND GEN_DEPENDENCIES ${IFCODEGEN_FILES})
    endforeach()

    if (ARG_MODULE_NAME)
        list(APPEND GENERATOR_ARGUMENTS --module=${ARG_MODULE_NAME})
    endif()

    # Show qface and annotations in IDE
    # If the generate function is called directly ${generator_target} is not defined and we will
    # fallback to the IDL base name
    set(IDE_PREFIX ${generator_target})
    if (NOT IDE_PREFIX)
        set(IDE_PREFIX ${IFCODEGEN_BASE_NAME})
    endif()
    if (NOT TARGET ${IDE_PREFIX}_qface_files)
        add_custom_target(${IDE_PREFIX}_qface_files SOURCES
            ${IDE_FILES}
        )
    endif()

    set_property(DIRECTORY APPEND PROPERTY CMAKE_CONFIGURE_DEPENDS ${GEN_DEPENDENCIES})

    # If the generator was run successfully before
    # Check for the timestamps to determine when to run it again.
    set(RUN_GENERATOR FALSE)
    foreach(DEP ${GEN_DEPENDENCIES})
        if (${DEP} IS_NEWER_THAN ${OUTPUT_DIR}/.stamp-ifcodegen)
            set(RUN_GENERATOR TRUE)
            break()
        endif()
    endforeach()

    if (RUN_GENERATOR)
        if (QT_FEATURE_python3_virtualenv AND NOT QT_FEATURE_system_qface)
            if ("${CMAKE_HOST_SYSTEM_NAME}" STREQUAL "Windows")
                set(PYTHON_EXECUTABLE ${_VIRTUALENV}/Scripts/python.exe)
                file(TO_NATIVE_PATH "${_VIRTUALENV}" _VIRTUALENV)
            else()
                set(PYTHON_EXECUTABLE ${_VIRTUALENV}/bin/python)
                set(ENV{LC_ALL} en_US.UTF-8)
                set(ENV{LD_LIBRARY_PATH} ${_VIRTUALENV}/bin)
            endif()
            set(ENV{PYTHONHOME} ${_VIRTUALENV})
            set(ENV{VIRTUAL_ENV} ${_VIRTUALENV})
        else()
            qt_find_package(Python3 PROVIDED_TARGETS Python3::Interpreter MODULE_NAME interfaceframework)
            set(PYTHON_EXECUTABLE ${Python3_EXECUTABLE})
        endif()
        if (DEFINED _IFGENERATOR_CONFIG)
            set(ENV{IFGENERATOR_CONFIG} ${_IFGENERATOR_CONFIG})
        endif()

        message(STATUS "Running ifcodegen for ${IDL_FILES} with template ${TEMPLATE}")
        set(GENERATOR_CMD
                ${PYTHON_EXECUTABLE}
                ${_GENERATOR_PATH}/generate.py
                ${GENERATOR_ARGUMENTS}
                ${IDL_FILES}
                ${OUTPUT_DIR}
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
            execute_process(COMMAND ${CMAKE_COMMAND} -E touch ${OUTPUT_DIR}/.stamp-ifcodegen)
        else()
            message(FATAL_ERROR "Error while running the ifcodegen:\n${GENERATOR_CMD_STR}\n${GENERATOR_LOG}")
        endif()
    endif()

    # hack for the developer-build to have all headers where the Qt build expects them
    if (DEFINED ARG_EXTRA_HEADERS_OUTPUT_DIR)
        file(GLOB HEADER_FILES ${OUTPUT_DIR}/*.h)
        file(COPY ${HEADER_FILES} DESTINATION ${ARG_EXTRA_HEADERS_OUTPUT_DIR})
    endif()

endfunction()

if(NOT QT_NO_CREATE_VERSIONLESS_FUNCTIONS)
    function(qt_ifcodegen_generate)
        qt6_ifcodegen_generate(${ARGV})
    endfunction()
endif()

# Extends a target with autogenerated code from ifcodegen.
#
# IDL_FILES: The input IDL file for the generator. (REQUIRED)
#
# TEMPLATE: The template used by the code generator. (REQUIRED)
#
# ANNOTATION_FILES: List of additional annotation files, which should be passed
#   to the generator. (OPTIONAL)
#
# IMPORT_PATH: List of additional directories, where included IDL files are
#   searched for. (OPTIONAL)
#
# OUTPUT_DIR: Overwrite the default output path. By default the generated code
#   will be put in ${CMAKE_CURRENT_BINARY_DIR}. Use this argument to provide
#   a replacement. (OPTIONAL)
#
# EXTRA_HEADERS_OUTPUT_DIR: An additional location where all headers will be
#   copied to. (OPTIONAL)
#
# VERBOSE: Print additional messages during generation. Useful for debugging
#   purposes. (OPTIONAL)
#
# NO_GENERATE: Don't call the code generator. Only include the code which was
#   already previously generated e.g. by using qt6_ifcodegen_generate (OPTIONAL)
#
# Note: this is a macro in order to keep the current variable scope
macro(qt6_ifcodegen_extend_target target)
    internal_resolve_ifcodegen_path()

    if (QT_FEATURE_python3_virtualenv AND NOT QT_FEATURE_system_qface
        AND NOT EXISTS ${_VIRTUALENV}/bin/activate AND NOT EXISTS ${_VIRTUALENV}/Scripts/activate.bat)
        # Create a dummy target instead
        if (NOT TARGET ${target} AND NOT TEST ${target})
            if (DEFINED QtInterfaceFramework_BINARY_DIR)
                file(TOUCH ${CMAKE_CURRENT_BINARY_DIR}/cmake_dummy.cpp)
                add_library(${target} ${CMAKE_CURRENT_BINARY_DIR}/cmake_dummy.cpp)
            else()
                message(FATAL_ERROR "No virtualenv environment to run the ifcodegen")
            endif()
        endif()
        # IMPORTANT
        # As we are inside a macro, this return() will be executed inside the calling CMakeLists.txt
        # We only do this in our internal code as the generator is not ready yet to be used at this
        # point.
        return()
    endif()

    internal_ifcodegen_import(TARGET ${target} ${ARGN})
endmacro()

if(NOT QT_NO_CREATE_VERSIONLESS_FUNCTIONS)
    # Note: this is a macro in order to keep the current variable scope
    macro(qt_ifcodegen_extend_target)
        qt6_ifcodegen_extend_target(${ARGV})
    endmacro()
endif()

# Imports variables to build autogenerated code from ifcodegen yourself
#
# IDL_FILES: The input IDL file for the generator. (REQUIRED)
#
# TEMPLATE: The template used by the code generator. (REQUIRED)
#
# ANNOTATION_FILES: List of additional annotation files, which should be passed
#   to the generator. (OPTIONAL)
#
# IMPORT_PATH: List of additional directories, where included IDL files are
#   searched for. (OPTIONAL)
#
# OUTPUT_DIR: Overwrite the default output path. By default the generated code
#   will be put in ${CMAKE_CURRENT_BINARY_DIR}. Use this argument to provide
#   a replacement. (OPTIONAL)
#
# EXTRA_HEADERS_OUTPUT_DIR: An additional location where all headers will be
#   copied to. (OPTIONAL)
#
# VERBOSE: Print additional messages during generation. Useful for debugging
#   purposes. (OPTIONAL)
#
# NO_GENERATE: Don't call the code generator. Only include the code which was
#   already previously generated e.g. by using qt6_ifcodegen_generate (OPTIONAL)
#
# Note: this is a macro in order to keep the current variable scope
macro(qt6_ifcodegen_import_variables prefix)
    internal_resolve_ifcodegen_path()

    if (QT_FEATURE_python3_virtualenv AND NOT QT_FEATURE_system_qface
        AND NOT EXISTS ${_VIRTUALENV}/bin/activate AND NOT EXISTS ${_VIRTUALENV}/Scripts/activate.bat)
        if (DEFINED QtInterfaceFramework_BINARY_DIR)
            # IMPORTANT
            # As we are inside a macro, this return() will be executed inside the calling CMakeLists.txt
            # We only do this in our internal code as the generator is not ready yet to be used at this
            # point.
            return()
        endif()
    endif()

    internal_ifcodegen_import(PREFIX ${prefix} ${ARGN})
endmacro()

if(NOT QT_NO_CREATE_VERSIONLESS_FUNCTIONS)
    # Note: this is a macro in order to keep the current variable scope
    macro(qt_ifcodegen_import_variables)
        qt6_ifcodegen_import_variables(${ARGV})
    endmacro()
endif()
