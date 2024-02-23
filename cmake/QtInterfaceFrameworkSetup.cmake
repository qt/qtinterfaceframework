function(qt_config_python3_package_test name)
    if(DEFINED "TEST_${name}")
        return()
    endif()

    cmake_parse_arguments(arg "" "LABEL;PACKAGE;CONDITION;VERSION"
        "" ${ARGN})

    if(NOT arg_CONDITION)
        set(arg_CONDITION VERSION_EQUAL)
    endif()

    message(STATUS "Performing Test ${arg_LABEL}")

    execute_process(COMMAND ${Python3_EXECUTABLE} "-c" "import sys; sys.exit(\"${arg_PACKAGE}\" in sys.stdlib_module_names)"
                    RESULT_VARIABLE is_standard_module
                    ERROR_QUIET
                   )

    if(${is_standard_module} EQUAL 1)
            execute_process(COMMAND ${Python3_EXECUTABLE} "-c" "import ${arg_PACKAGE}"
                            RESULT_VARIABLE ${name}_RESULT
                            ERROR_QUIET
                           )
            if(${${name}_RESULT} EQUAL 0)
                set(${name}_VERSION ${Python3_VERSION})
            endif()
    else()
        if (Python3_VERSION VERSION_LESS "3.8")
            execute_process(COMMAND ${Python3_EXECUTABLE} "-c" "import pkg_resources; print(pkg_resources.get_distribution(\"${arg_PACKAGE}\").version)"
                            RESULT_VARIABLE ${name}_RESULT
                            OUTPUT_VARIABLE ${name}_VERSION
                            ERROR_QUIET
                           )
        else()
            execute_process(COMMAND ${Python3_EXECUTABLE} "-c" "import importlib.metadata; print(importlib.metadata.version(\"${arg_PACKAGE}\"))"
                            RESULT_VARIABLE ${name}_RESULT
                            OUTPUT_VARIABLE ${name}_VERSION
                            ERROR_QUIET
                           )
        endif()
    endif()

    set(HAVE_${name} FALSE)
    set(status_label "Failed")

    if(${${name}_RESULT} EQUAL 0)
        if(DEFINED arg_VERSION)
            message(STATUS "Checking for version:")
            message(STATUS "Expected: ${arg_VERSION}")
            message(STATUS "Condition: ${arg_CONDITION}")
            message(STATUS "Got: ${${name}_VERSION}")
            if("${${name}_VERSION}" ${arg_CONDITION} "${arg_VERSION}")
                set(HAVE_${name} TRUE)
                set(status_label "Success")
            endif()
        else()
            set(HAVE_${name} TRUE)
            set(status_label "Success")
        endif()
    endif()
    message(STATUS "Performing Test ${arg_LABEL} - ${status_label}")

    set(TEST_${name} "${HAVE_${name}}" CACHE INTERNAL "${arg_LABEL}")
endfunction()
