#####################################################################
## Internal API
#####################################################################

function(internal_check_if_venv VENV_PATH)
    message(STATUS "Checking venv ${VENV_PATH}")
    set(VENV_OLD_PATH_FILE "${VENV_PATH}/venvpath.txt")
    if (NOT EXISTS "${VENV_OLD_PATH_FILE}")
        message(STATUS "Still empty venv, bailing out")
        return()
    endif()
    file(READ "${VENV_OLD_PATH_FILE}" VENV_OLD_PATH)
    if ("${VENV_OLD_PATH}" STREQUAL "")
        message(STATUS "Cannot get previous venv path, bailing out")
        return()
    endif()
    set(REQUIREMENTS_FILE "${VENV_PATH}/requirements.txt")
    if (${VENV_PATH} STREQUAL ${VENV_OLD_PATH} AND EXISTS "${REQUIREMENTS_FILE}")
        include(QtFindPackageHelpers)
        # Make sure we search in the host Qt path if set. Otherwise me might end up with a
        # interpreter for the target
        if (NOT "${QT_HOST_PATH}" STREQUAL "")
            set(CMAKE_PREFIX_PATH "${QT_HOST_PATH}")
        endif()
        qt_find_package(Python3 PROVIDED_TARGETS Python3::Interpreter MODULE_NAME interfaceframework)
        if (NOT Python3_EXECUTABLE)
            message(STATUS "Could not locate Python3 for venv regeneration.")
            return()
        endif()

        if ("${CMAKE_HOST_SYSTEM_NAME}" STREQUAL "Windows")
            set(PIPEXE "${VENV_PATH}/Scripts/pip3.exe")
        else()
            set(PIPEXE "${VENV_PATH}/bin/pip3")
        endif()

        message(STATUS "Regenerating outdated venv originally at ${VENV_OLD_PATH}")
        file(READ "${REQUIREMENTS_FILE}" REQUIREMENTS)
        file(REMOVE_RECURSE "${VENV_PATH}")

        execute_process(COMMAND "${Python3_EXECUTABLE}" -m venv "${VENV_PATH}")
        file(WRITE "${REQUIREMENTS_FILE}" "${REQUIREMENTS}")
        file(WRITE "${VENV_OLD_PATH_FILE}" "${VENV_PATH}")
        execute_process(COMMAND ${CMAKE_COMMAND} -E env VIRTUAL_ENV="${VENV_PATH}" --unset=PYTHONHOME "${PIPEXE}" install -r "${VENV_PATH}/requirements.txt")
    endif()
endfunction()

# Determines the location of the correct ifcodegen installation
# Inside the interface framework the features are used to determine whether a virtualenv needs to be used
# In all other cases the existence of the virtualenv is checked and depending on that the
# QT_IFCODEGEN_VIRTUALENV_PATH is set.
# All following functions just use the existence of this variable instead of the enabled features
function(qt_ensure_ifcodegen)
    if(DEFINED QT_IFCODEGEN_GENERATOR_PATH)
        return()
    endif()

    if (NOT "${QT_HOST_PATH}" STREQUAL "")
        set (QT_IFCODEGEN_GENERATOR_PATH ${QT_HOST_PATH}/${QT6_HOST_INFO_LIBEXECDIR}/ifcodegen CACHE FILEPATH "ifcodegen generator")
        set (QT_IFCODEGEN_VIRTUALENV_PATH ${QT_IFCODEGEN_GENERATOR_PATH}/ifcodegen_venv CACHE FILEPATH "ifcodegen virtualenv")
        set (QT_IFCODEGEN_TYPE "host")

        # Expand the variable manually as we use a CACHE variable
        if (NOT EXISTS "${QT_IFCODEGEN_VIRTUALENV_PATH}")
            unset(QT_IFCODEGEN_VIRTUALENV_PATH CACHE)
        endif()
    elseif (DEFINED QtInterfaceFramework_BINARY_DIR AND (NOT DEFINED QT_BUILD_STANDALONE_TESTS))
        set (QT_IFCODEGEN_GENERATOR_PATH ${QtInterfaceFramework_SOURCE_DIR}/src/tools/ifcodegen CACHE FILEPATH "ifcodegen generator")
        set (QT_IFCODEGEN_IFGENERATOR_CONFIG ${QtInterfaceFramework_BINARY_DIR}/src/tools/ifcodegen/.config CACHE FILEPATH "ifcodegen config")
        set (QT_IFCODEGEN_TYPE "source")

        if (QT_FEATURE_compiled_ifcodegen)
            set (QT_IFCODEGEN_GENERATOR_PATH ${QT_BUILD_DIR}/${INSTALL_LIBEXECDIR}/ifcodegen CACHE FILEPATH "ifcodegen generator" FORCE)
        elseif (QT_FEATURE_python3_venv AND NOT QT_FEATURE_system_qface)
            set (QT_IFCODEGEN_VIRTUALENV_PATH ${QtInterfaceFramework_BINARY_DIR}/src/tools/ifcodegen/ifcodegen_venv CACHE FILEPATH "ifcodegen virtualenv")
        endif()
    endif()

    # Fallback to the installed codepath if the host path doesn't exist
    if (NOT EXISTS ${QT_IFCODEGEN_GENERATOR_PATH})
        set (QT_IFCODEGEN_GENERATOR_PATH ${QTIF_INSTALL_PREFIX}/${QT6_INSTALL_LIBEXECS}/ifcodegen CACHE FILEPATH "ifcodegen generator")
        set (QT_IFCODEGEN_VIRTUALENV_PATH ${QT_IFCODEGEN_GENERATOR_PATH}/ifcodegen_venv CACHE FILEPATH "ifcodegen virtualenv")
        set (QT_IFCODEGEN_TYPE "installed")

        # Expand the variable manually as we use a CACHE variable
        if (NOT EXISTS "${QT_IFCODEGEN_VIRTUALENV_PATH}")
            unset(QT_IFCODEGEN_VIRTUALENV_PATH CACHE)
        endif()
    endif()

    # On windows we need to append the .exe suffix if the codegen is not a directory
    if (CMAKE_HOST_WIN32 AND NOT IS_DIRECTORY ${QT_IFCODEGEN_GENERATOR_PATH})
        set (QT_IFCODEGEN_GENERATOR_PATH "${QT_IFCODEGEN_GENERATOR_PATH}.exe" CACHE FILEPATH "ifcodegen generator" FORCE)
    endif()

    # Templates are either from source or from the "installed" Qt, we never want to use the "host"
    # templates when cross-compiling
    if (DEFINED QtInterfaceFramework_BINARY_DIR AND (NOT DEFINED QT_BUILD_STANDALONE_TESTS))
        set (QT_IFCODEGEN_TEMPLATES_PATH ${QtInterfaceFramework_SOURCE_DIR}/src/tools/ifcodegen/templates CACHE FILEPATH "ifcodegen templates")
        set (QT_IFCODEGEN_TEMPLATE_TYPE "source")
    else()
        set (QT_IFCODEGEN_TEMPLATES_PATH ${QTIF_INSTALL_PREFIX}/${QT6_INSTALL_DATA}/ifcodegen-templates CACHE FILEPATH "ifcodegen templates")
        set (QT_IFCODEGEN_TEMPLATE_TYPE "installed")
    endif()

    message(STATUS "Using ${QT_IFCODEGEN_TYPE} ifcodegen found at: ${QT_IFCODEGEN_GENERATOR_PATH}")
    message(STATUS "Using ${QT_IFCODEGEN_TEMPLATE_TYPE} ifcodegen templates found at: ${QT_IFCODEGEN_TEMPLATES_PATH}")
    if (QT_IFCODEGEN_VIRTUALENV_PATH)
        message(STATUS "Using ${QT_IFCODEGEN_TYPE} ifcodegen virtualenv found at: ${QT_IFCODEGEN_VIRTUALENV_PATH}")
        internal_check_if_venv("${QT_IFCODEGEN_VIRTUALENV_PATH}")
    endif()

    if (NOT QT_FEATURE_compiled_ifcodegen AND NOT EXISTS ${QT_IFCODEGEN_GENERATOR_PATH})
        message(FATAL_ERROR "Couldn't find a working ifcodegen location, tried: ${QT_IFCODEGEN_GENERATOR_PATH}")
    endif()
endfunction()

function(internal_ifcodegen_import)
    cmake_parse_arguments(
        PARSE_ARGV 0
        ARG
        "NO_GENERATE;VERBOSE" "TARGET;PREFIX;IDL_FILES;TEMPLATE;OUTPUT_DIR" ""
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

    if (NOT DEFINED ARG_TEMPLATE)
        message(FATAL_ERROR
            "Called without generator template. Please specify one using the TEMPLATE argument."
            )
    endif()

    if (NOT ARG_NO_GENERATE)
        set(generator_target ${ARG_TARGET})
        qt6_ifcodegen_generate(${ARGN})
    endif()

    set(OUTPUT_DIR ${ARG_OUTPUT_DIR})
    if (NOT DEFINED ARG_OUTPUT_DIR)
        get_filename_component(TEMPLATE_NAME "${ARG_TEMPLATE}" NAME)
        set(OUTPUT_DIR ${CMAKE_CURRENT_BINARY_DIR}/${TEMPLATE_NAME})
    endif()
    make_directory(${OUTPUT_DIR})

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

    set(HEADERS ${${VAR_PREFIX}_SOURCES})
    list(FILTER HEADERS INCLUDE REGEX ".+\\.h$")
    if(CMAKE_VERSION GREATER_EQUAL 3.20)
        cmake_policy(PUSH)
        cmake_policy(SET CMP0118 NEW)
        set_source_files_properties(${HEADERS} PROPERTIES GENERATED TRUE)
        cmake_policy(POP)
    else()
        set_source_files_properties(${HEADERS} PROPERTIES GENERATED TRUE)
    endif()
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

macro(internal_check_ifcodegen_ready target)
    qt_ensure_ifcodegen()
    if ((QT_IFCODEGEN_VIRTUALENV_PATH
        AND NOT EXISTS ${QT_IFCODEGEN_VIRTUALENV_PATH}/bin/activate AND NOT EXISTS ${QT_IFCODEGEN_VIRTUALENV_PATH}/Scripts/activate.bat)
        OR NOT EXISTS ${QT_IFCODEGEN_GENERATOR_PATH})
        if (NOT "${target}" STREQUAL "")
            # Create a dummy target instead
            if (NOT TARGET ${target} AND NOT TEST ${target})
                if (DEFINED QtInterfaceFramework_BINARY_DIR)
                    file(TOUCH ${CMAKE_CURRENT_BINARY_DIR}/cmake_dummy.cpp)
                    add_library(${target} ${CMAKE_CURRENT_BINARY_DIR}/cmake_dummy.cpp)
                else()
                    message(FATAL_ERROR "No virtualenv environment to run the ifcodegen")
                endif()
            endif()
        endif()
        # IMPORTANT
        # As we are inside a macro, this return() will be executed inside the calling CMakeLists.txt
        # We only do this in our internal code as the generator is not ready yet to be used at this
        # point.
        return()
    endif()
endmacro()

# Extracts all known ifcodegen variables from the calling scope and provides them
# as NEW_ARGS
# The ARG_UNPARSED_ARGUMENTS variable can be used to access all other parameters
# The extraOptions extraOneValueArgs and extraMultiValueArgs can be used to filter
# them from the ARG_UNPARSED_ARGUMENTS and allow special handling
macro(internal_extract_ifcodegen_vars extraOptions extraOneValueArgs extraMultiValueArgs)
    set(options VERBOSE)
    set(oneValueArgs IDL_FILES TEMPLATE OUTPUT_DIR EXTRA_HEADERS_OUTPUT_DIR)
    set(multiValueArgs ANNOTATION_FILES IDL_IMPORT_PATH EXTRA_TEMPLATE_SEARCH_PATH)
    cmake_parse_arguments(
        PARSE_ARGV 1
        ARG
        "${options}${extraOptions}" "${oneValueArgs}${extraOneValueArgs}" "${multiValueArgs}${extraMultiValueArgs}"
    )
    set(NEW_ARGS "")
    foreach(I IN LISTS options)
        if (ARG_${I})
            list(APPEND NEW_ARGS ${I})
        endif()
    endforeach()
    foreach(I IN LISTS oneValueArgs)
        if (ARG_${I})
            list(APPEND NEW_ARGS ${I})
            list(APPEND NEW_ARGS ${ARG_${I}})
        endif()
    endforeach()
    foreach(I IN LISTS multiValueArgs)
        if (ARG_${I})
            list(APPEND NEW_ARGS ${I})
            list(APPEND NEW_ARGS ${ARG_${I}})
        endif()
    endforeach()
endmacro()

#####################################################################
## Public API
#####################################################################

macro(qt6_set_ifcodegen_variable name)
    set(${name} ${ARGN})
    set(${name} ${ARGN} PARENT_SCOPE)
endmacro()

if(NOT QT_NO_CREATE_VERSIONLESS_FUNCTIONS)
    macro(qt_set_ifcodegen_variable)
        qt6_set_ifcodegen_variable(${ARGV})
    endmacro()
endif()

macro(set_ifcodegen_variable)
    message(AUTHOR_WARNING
        "The set_ifcodegen_variable macro is deprecated and will be removed soon. Please use qt6_set_ifcodegen_variable instead.")
    qt6_set_ifcodegen_variable(${ARGV})
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
# IDL_IMPORT_PATH: List of additional directories, where included IDL files are
#   searched for. (OPTIONAL)
#
# OUTPUT_DIR: Overwrite the default output path. By default the generated code
#   will be put in ${CMAKE_CURRENT_BINARY_DIR}/<template-name>. Use this argument to provide
#   a replacement. (OPTIONAL)
#
# EXTRA_HEADERS_OUTPUT_DIR: An additional location where all headers will be
#   copied to. (OPTIONAL)
#
# EXTRA_TEMPLATE_SEARCH_PATH: list of additional directories, where templates are
#   searched for. (OPTIONAL)
#
# VERBOSE: Print additional messages during generation. Useful for debugging
#   purposes. (OPTIONAL)
#
function(qt6_ifcodegen_generate)
    qt_ensure_ifcodegen()

    if ((QT_IFCODEGEN_VIRTUALENV_PATH
        AND NOT EXISTS ${QT_IFCODEGEN_VIRTUALENV_PATH}/bin/activate AND NOT EXISTS ${QT_IFCODEGEN_VIRTUALENV_PATH}/Scripts/activate.bat)
        OR NOT EXISTS ${QT_IFCODEGEN_GENERATOR_PATH})
        return()
    endif()

    cmake_parse_arguments(
        PARSE_ARGV 0
        ARG
        # TARGET and PREFIX are not handled here, but we don't want the UNPARSED_ARGUMENTS error to
        # be triggered when provided (when called through the internal_ifcodegen_import() function
        "VERBOSE" "IDL_FILES;TEMPLATE;MODULE_NAME;OUTPUT_DIR;EXTRA_HEADERS_OUTPUT_DIR;TARGET;PREFIX" "ANNOTATION_FILES;IMPORT_PATH;IDL_IMPORT_PATH;EXTRA_TEMPLATE_SEARCH_PATH"
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
        get_filename_component(TEMPLATE_NAME "${ARG_TEMPLATE}" NAME)
        set(OUTPUT_DIR ${CMAKE_CURRENT_BINARY_DIR}/${TEMPLATE_NAME})
    endif()
    make_directory(${OUTPUT_DIR})

    if (NOT DEFINED ARG_IDL_FILES)
        message(FATAL_ERROR
            "Called without input files. Please specify some using the IDL_FILES argument."
            )
    endif()
    get_filename_component(IDL_FILES "${ARG_IDL_FILES}" REALPATH BASE_DIR)
    get_filename_component(IFCODEGEN_SOURCE_DIR "${IDL_FILES}" DIRECTORY)
    get_filename_component(IFCODEGEN_BASE_NAME "${IDL_FILES}" NAME_WLE)
    get_filename_component(IFCODEGEN_SOURCE_ANNOTATION ${IFCODEGEN_SOURCE_DIR}/${IFCODEGEN_BASE_NAME}.yaml REALPATH BASE_DIR)

    # Add the built-in templates as default search path
    list(APPEND TEMPLATE_SEARCH_PATH ${QT_IFCODEGEN_TEMPLATES_PATH})
    list(APPEND TEMPLATE_SEARCH_PATH_ARGUMENTS -T ${QT_IFCODEGEN_TEMPLATES_PATH})

    # Add the paths defined by the global variable
    foreach(SEARCH_PATH ${QT_IFCODEGEN_TEMPLATE_SEARCH_PATH})
        get_filename_component(SEARCH_PATH "${SEARCH_PATH}" REALPATH BASE_DIR)
        list(APPEND TEMPLATE_SEARCH_PATH ${SEARCH_PATH})
        list(APPEND TEMPLATE_SEARCH_PATH_ARGUMENTS -T ${SEARCH_PATH})
    endforeach()

    # Add the paths defined by function argument
    foreach(SEARCH_PATH ${ARG_EXTRA_TEMPLATE_SEARCH_PATH})
        get_filename_component(SEARCH_PATH "${SEARCH_PATH}" REALPATH BASE_DIR)
        list(APPEND TEMPLATE_SEARCH_PATH ${SEARCH_PATH})
        list(APPEND TEMPLATE_SEARCH_PATH_ARGUMENTS -T ${SEARCH_PATH})
    endforeach()

    # Find the used templates within the search path
    foreach(SEARCH_PATH ${TEMPLATE_SEARCH_PATH})
        set(TEMPLATE_PWD "${SEARCH_PATH}/${ARG_TEMPLATE}")
        if(EXISTS ${TEMPLATE_PWD})
            set(TEMPLATE_PATH ${TEMPLATE_PWD})
            set(TEMPLATE ${ARG_TEMPLATE})
            break()
        endif()
    endforeach()

    # We didn't find the template in the search path, check whether it's a path to a template
    if (NOT DEFINED TEMPLATE)
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
    file(GLOB COMMON_TEMPLATE_FILES ${QT_IFCODEGEN_TEMPLATES_PATH}/*common*/*)
    list(APPEND GEN_DEPENDENCIES ${COMMON_TEMPLATE_FILES})

    set(GENERATOR_ARGUMENTS ${TEMPLATE_SEARCH_PATH_ARGUMENTS} --template=${TEMPLATE} --force)
    foreach(ANNOTATION ${ARG_ANNOTATION_FILES})
        get_filename_component(ANNOTATION_PATH "${ANNOTATION}" REALPATH BASE_DIR)
        list(APPEND GENERATOR_ARGUMENTS -A ${ANNOTATION_PATH})
        # Dependency for regeneration
        list(APPEND GEN_DEPENDENCIES ${ANNOTATION_PATH})
        list(APPEND IDE_FILES ${ANNOTATION_PATH})
    endforeach()

    if (ARG_IMPORT_PATH)
        set(ARG_IDL_IMPORT_PATH ${ARG_IMPORT_PATH})
        message(WARNING
            "qt_ifcodegen's IMPORT_PATH option is deprecated, and will be "
            "removed in a future Qt version. Use the IDL_IMPORT_PATH option instead.")
    endif()

    foreach(IMPORT ${ARG_IDL_IMPORT_PATH})
        get_filename_component(IDL_IMPORT_PATH "${IMPORT}" REALPATH BASE_DIR)
        list(APPEND GENERATOR_ARGUMENTS -I ${IDL_IMPORT_PATH})
        # Dependency for regeneration
        file(GLOB IFCODEGEN_FILES ${IDL_IMPORT_PATH}/*.qface)
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
        if(NOT IS_DIRECTORY ${QT_IFCODEGEN_GENERATOR_PATH}) # Compiled ifcodegen
            if ("${CMAKE_HOST_SYSTEM_NAME}" STREQUAL "Windows")
                set (IFCODEGEN_CMD ${QT_IFCODEGEN_GENERATOR_PATH})
            else()
                set (IFCODEGEN_CMD ${CMAKE_COMMAND} -E env LC_ALL=en_US.UTF-8 ${QT_IFCODEGEN_GENERATOR_PATH})
            endif()
        elseif (QT_IFCODEGEN_VIRTUALENV_PATH) # virtualenv
            if ("${CMAKE_HOST_SYSTEM_NAME}" STREQUAL "Windows")
                set(PYTHON_EXECUTABLE ${QT_IFCODEGEN_VIRTUALENV_PATH}/Scripts/python.exe)
                file(TO_NATIVE_PATH "${QT_IFCODEGEN_VIRTUALENV_PATH}" QT_IFCODEGEN_VIRTUALENV_PATH)
            else()
                set(PYTHON_EXECUTABLE ${QT_IFCODEGEN_VIRTUALENV_PATH}/bin/python)
                list(APPEND CMD_ENV LC_ALL=en_US.UTF-8)
                list(APPEND CMD_ENV LD_LIBRARY_PATH=${QT_IFCODEGEN_VIRTUALENV_PATH}/bin)
            endif()
            list(APPEND CMD_ENV VIRTUAL_ENV=${QT_IFCODEGEN_VIRTUALENV_PATH})
        else() # qface installed in system
            include(QtFindPackageHelpers)
            # Make sure we search in the host Qt path if set. Otherwise me might end up with a
            # interpreter for the target
            if (NOT "${QT_HOST_PATH}" STREQUAL "")
                set(CMAKE_PREFIX_PATH "${QT_HOST_PATH}")
            endif()
            qt_find_package(Python3 PROVIDED_TARGETS Python3::Interpreter MODULE_NAME interfaceframework)
            set(PYTHON_EXECUTABLE ${Python3_EXECUTABLE})
            if (NOT Python3_EXECUTABLE)
                message(FATAL_ERROR "Python3 needs to be available to use the ifcodegen.")
            endif()
        endif()
        if (DEFINED QT_IFCODEGEN_IFGENERATOR_CONFIG)
            list(APPEND CMD_ENV IFGENERATOR_CONFIG=${QT_IFCODEGEN_IFGENERATOR_CONFIG})
        endif()

        if (NOT DEFINED IFCODEGEN_CMD)
            set (IFCODEGEN_CMD ${CMAKE_COMMAND} -E env ${CMD_ENV}
                               ${PYTHON_EXECUTABLE} -B
                               ${QT_IFCODEGEN_GENERATOR_PATH}/generate.py)
        endif()

        message(STATUS "Running ifcodegen for ${IDL_FILES} with template ${TEMPLATE}")
        set(GENERATOR_CMD
                ${IFCODEGEN_CMD}
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
            string(REGEX MATCHALL "([^\n]*)warning:[^\n]*" GENERATOR_WARNINGS ${GENERATOR_LOG})
            foreach(FULL_WARNING_MESSAGE ${GENERATOR_WARNINGS})
                string(REGEX MATCH "warning:[^\n]*" WARNING_MESSAGE ${FULL_WARNING_MESSAGE})
                message(WARNING "ifcodegen: ${WARNING_MESSAGE}")
                message("${FULL_WARNING_MESSAGE}")
            endforeach()
            execute_process(COMMAND ${CMAKE_COMMAND} -E touch ${OUTPUT_DIR}/.stamp-ifcodegen)
        else()
            # FATAL_ERROR doesn't print the message as is. Because of that the command cannot
            # be copied into a terminal window anymore. To fix this we report the full
            # command and log output and then exit with a fatal error
            message("ifcodegen call failed:\n${GENERATOR_CMD_STR}\n${GENERATOR_LOG}")
            message(FATAL_ERROR "Error while running ifcodegen")
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
# IDL_IMPORT_PATH: List of additional directories, where included IDL files are
#   searched for. (OPTIONAL)
#
# OUTPUT_DIR: Overwrite the default output path. By default the generated code
#   will be put in ${CMAKE_CURRENT_BINARY_DIR}/<template-name>. Use this argument to provide
#   a replacement. (OPTIONAL)
#
# EXTRA_HEADERS_OUTPUT_DIR: An additional location where all headers will be
#   copied to. (OPTIONAL)
#
# EXTRA_TEMPLATE_SEARCH_PATH: list of additional directories, where templates are
#   searched for. (OPTIONAL)
#
# VERBOSE: Print additional messages during generation. Useful for debugging
#   purposes. (OPTIONAL)
#
# NO_GENERATE: Don't call the code generator. Only include the code which was
#   already previously generated e.g. by using qt6_ifcodegen_generate (OPTIONAL)
#
# Note: this is a macro in order to keep the current variable scope
macro(qt6_ifcodegen_extend_target target)
    internal_check_ifcodegen_ready(${target})

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
# IDL_IMPORT_PATH: List of additional directories, where included IDL files are
#   searched for. (OPTIONAL)
#
# OUTPUT_DIR: Overwrite the default output path. By default the generated code
#   will be put in ${CMAKE_CURRENT_BINARY_DIR}. Use this argument to provide
#   a replacement. (OPTIONAL)
#
# EXTRA_HEADERS_OUTPUT_DIR: An additional location where all headers will be
#   copied to. (OPTIONAL)
#
# EXTRA_TEMPLATE_SEARCH_PATH: list of additional directories, where templates are
#   searched for. (OPTIONAL)
#
# VERBOSE: Print additional messages during generation. Useful for debugging
#   purposes. (OPTIONAL)
#
# NO_GENERATE: Don't call the code generator. Only include the code which was
#   already previously generated e.g. by using qt6_ifcodegen_generate (OPTIONAL)
#
# Note: this is a macro in order to keep the current variable scope
macro(qt6_ifcodegen_import_variables prefix)
    internal_check_ifcodegen_ready("")

    internal_ifcodegen_import(PREFIX ${prefix} ${ARGN})
endmacro()

if(NOT QT_NO_CREATE_VERSIONLESS_FUNCTIONS)
    # Note: this is a macro in order to keep the current variable scope
    macro(qt_ifcodegen_import_variables)
        qt6_ifcodegen_import_variables(${ARGV})
    endmacro()
endif()

# Adds a plugin target which is generated by ifcodegen.
#
# It accepts the following arguments in addition to all arguments supported by
# qt6_add_plugin. The CLASS_NAME and PLUGIN_TYPE arguments are
# automatically extracted from the template file if not set explicitly.
#
# IDL_FILES: The input IDL file for the generator. (REQUIRED)
#
# TEMPLATE: The template used by the code generator. (REQUIRED)
#
# ANNOTATION_FILES: List of additional annotation files, which should be passed
#   to the generator. (OPTIONAL)
#
# IDL_IMPORT_PATH: List of additional directories, where included IDL files are
#   searched for. (OPTIONAL)
#
# OUTPUT_DIR: Overwrite the default output path. By default the generated code
#   will be put in ${CMAKE_CURRENT_BINARY_DIR}. Use this argument to provide
#   a replacement. (OPTIONAL)
#
# EXTRA_HEADERS_OUTPUT_DIR: An additional location where all header files will be
#   copied to. (OPTIONAL)
#
# EXTRA_TEMPLATE_SEARCH_PATH: list of additional directories, where templates are
#   searched for. (OPTIONAL)
#
# VERBOSE: Print additional messages during generation. Useful for debugging
#   purposes. (OPTIONAL)
function(qt6_ifcodegen_add_plugin target)
    internal_check_ifcodegen_ready(${target})

    # Extracts all ifcodegen variables into NEW_ARGS
    internal_extract_ifcodegen_vars("" ";CLASS_NAME;PLUGIN_TYPE" "")

    # Debugging helper
    # message("qt6_ifcodegen_import_variables(GEN ${NEW_ARGS})")
    qt6_ifcodegen_import_variables(GEN ${NEW_ARGS})

    if (DEFINED ARG_CLASS_NAME)
        set(backend_class ${ARG_CLASS_NAME})
    else()
        if (NOT GEN_PLUGIN_CLASS_NAME)
            message(FATAL_ERROR "No CLASS_NAME provided and no PLUGIN_CLASS_NAME found in ifcodegen template variable")
        endif()
        set(backend_class ${GEN_PLUGIN_CLASS_NAME})
    endif()
    if (DEFINED ARG_PLUGIN_TYPE)
        set(plugin_type ${ARG_PLUGIN_TYPE})
    else()
        if (GEN_PLUGIN_TYPE)
            set(plugin_type ${GEN_PLUGIN_TYPE})
        else()
            set(plugin_type "interfaceframework")
        endif()
    endif()

    # Debugging helper
    # message("qt6_add_plugin(${target} CLASS_NAME ${backend_class} PLUGIN_TYPE ${plugin_type} ${ARG_UNPARSED_ARGUMENTS})")
    qt6_add_plugin(${target} CLASS_NAME ${backend_class} PLUGIN_TYPE ${plugin_type} ${ARG_UNPARSED_ARGUMENTS})
    qt6_ifcodegen_extend_target(${target} ${NEW_ARGS})
endfunction()

if(NOT QT_NO_CREATE_VERSIONLESS_FUNCTIONS)
    # Note: this is a macro in order to keep the current variable scope
    macro(qt_ifcodegen_add_plugin)
        qt6_ifcodegen_add_plugin(${ARGV})
    endmacro()
endif()

# Adds a QML module which is generated by ifcodegen.
#
# It accepts the following arguments in addition to all arguments supported by
# qt6_add_qml_module. The URI and VERSION arguments are
# automatically extracted from the template file if not set explicitly.
#
# IDL_FILES: The input IDL file for the generator. (REQUIRED)
#
# TEMPLATE: The template used by the code generator. (REQUIRED)
#
# ANNOTATION_FILES: List of additional annotation files, which should be passed
#   to the generator. (OPTIONAL)
#
# IDL_IMPORT_PATH: List of additional directories, where included IDL files are
#   searched for. (OPTIONAL)
#
# OUTPUT_DIR: Overwrite the default output path. By default the generated code
#   will be put in ${CMAKE_CURRENT_BINARY_DIR}. Use this argument to provide
#   a replacement. (OPTIONAL)
#
# EXTRA_HEADERS_OUTPUT_DIR: An additional location where all header files will be
#   copied to. (OPTIONAL)
#
# EXTRA_TEMPLATE_SEARCH_PATH: list of additional directories, where templates are
#   searched for. (OPTIONAL)
#
# VERBOSE: Print additional messages during generation. Useful for debugging
#   purposes. (OPTIONAL)
function(qt6_ifcodegen_add_qml_module target)
    internal_check_ifcodegen_ready(${target})

    internal_extract_ifcodegen_vars("" ";URI;VERSION" "")

    # Debugging helper
    # message("qt6_ifcodegen_import_variables(GEN ${NEW_ARGS})")
    qt6_ifcodegen_import_variables(GEN ${NEW_ARGS})

    if (DEFINED ARG_URI)
        set(uri ${ARG_URI})
    else()
        if (NOT GEN_URI)
            message(FATAL_ERROR "No URI provided and no URI found in ifcodegen template variable")
        endif()
        set(uri ${GEN_URI})
    endif()
    if (DEFINED ARG_VERSION)
        set(version VERSION ${ARG_VERSION})
    else()
        if (GEN_VERSION)
            set(version VERSION ${GEN_VERSION})
        endif()
    endif()

    # Debugging helper
    # message("qt6_add_qml_module(${target} URI ${uri} ${version} ${ARG_UNPARSED_ARGUMENTS})")
    qt6_add_qml_module(${target} URI ${uri} ${version} ${ARG_UNPARSED_ARGUMENTS})
    qt6_ifcodegen_extend_target(${target} ${NEW_ARGS})
endfunction()

if(NOT QT_NO_CREATE_VERSIONLESS_FUNCTIONS)
    # Note: this is a macro in order to keep the current variable scope
    macro(qt_ifcodegen_add_qml_module)
        qt6_ifcodegen_add_qml_module(${ARGV})
    endmacro()
endif()
