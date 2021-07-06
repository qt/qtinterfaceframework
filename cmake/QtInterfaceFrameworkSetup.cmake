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

    execute_process(COMMAND ${Python3_EXECUTABLE} "-c" "import pkg_resources; print(pkg_resources.get_distribution(\"${arg_PACKAGE}\").version)"
                    RESULT_VARIABLE ${name}_RESULT
                    OUTPUT_VARIABLE ${name}_VERSION
                    ERROR_QUIET
                   )

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
