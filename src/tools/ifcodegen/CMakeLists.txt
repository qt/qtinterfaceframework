
#####################################################################
## Virtual env creation
#####################################################################

qt_path_join(ifcodegen_install_dir "${QT_INSTALL_DIR}" "${INSTALL_LIBEXECDIR}/ifcodegen")
qt_path_join(templates_install_dir "${QT_INSTALL_DIR}" "${INSTALL_DATADIR}/ifcodegen-templates")

set(COMPILE_IFCODEGEN QT_FEATURE_ifcodegen AND NOT QT_FEATURE_host_ifcodegen AND QT_FEATURE_compiled_ifcodegen)
if(QT_FEATURE_python3_venv AND (${COMPILE_IFCODEGEN} OR NOT QT_FEATURE_system_qface))
    if (NOT CMAKE_GENERATOR MATCHES "^Ninja")
        message(FATAL_ERROR "Can't build the virtualenv with the current CMake Generator.\n"
                            "Please use Ninja as Generator.")
    endif()

    qt_find_package(Python3 PROVIDED_TARGETS Python3::Interpreter MODULE_NAME interfaceframework)

    set(VIRTUALENV_NAME ifcodegen_venv)
    set(VIRTUALENV_PATH ${CMAKE_CURRENT_BINARY_DIR}/${VIRTUALENV_NAME})
    set(IFCODEGEN_SOURCE_DIR ${QtInterfaceFramework_SOURCE_DIR}/src/3rdparty/qface)

    if ("${CMAKE_HOST_SYSTEM_NAME}" STREQUAL "Windows")
        set(VIRTUALENV_ACTIVATE ${VIRTUALENV_PATH}/Scripts/activate.bat)
        set(VIRTUALENV_ACTIVATE_COMMAND ${VIRTUALENV_ACTIVATE})
        set(VIRTUALENV_PYTHON ${VIRTUALENV_PATH}/Scripts/python.exe)
        set(IFCODEGEN_BIN ${VIRTUALENV_PATH}/Scripts/qface.exe)
    else()
        set(VIRTUALENV_ACTIVATE ${VIRTUALENV_PATH}/bin/activate)
        set(VIRTUALENV_ACTIVATE_COMMAND . ${VIRTUALENV_ACTIVATE})
        set(VIRTUALENV_PYTHON ${VIRTUALENV_PATH}/bin/python)
        set(IFCODEGEN_BIN ${VIRTUALENV_PATH}/bin/qface)
    endif()
    set(VIRTUALENV_DEACTIVATE_COMMAND deactivate)

    add_custom_command(OUTPUT ${VIRTUALENV_ACTIVATE}
                       COMMAND ${Python3_EXECUTABLE} -m venv ${VIRTUALENV_PATH}
                       COMMENT "Setting up virtualenv for qface, name: ${VIRTUALENV_NAME}"
                       )

    # This is not very nice, but it gives us at least a good way to handle virtualenv rebuilding when
    # someone is working on the qface sources
    file(GLOB_RECURSE IFCODEGEN_SOURCE_FILES ${IFCODEGEN_SOURCE_DIR}/*.py)

    # If the upstream python packages introduce a regression this option can be used to install
    # the minimum version for all required python package and produce a working setup
    # Those packages might be outdated and may contain security holes, but they are known to be
    # working.
    set(INSTALL_MINIMAL_QFACE_PACKAGES_COMMAND)
    if (QT_USE_MINIMAL_QFACE_PACKAGES)
        set(INSTALL_MINIMAL_QFACE_PACKAGES_COMMAND COMMAND pip3 install -r ${IFCODEGEN_SOURCE_DIR}/requirements_minimal.txt)
    endif()

    # On the CI we use the special wheel folder when available to not download all packages again on each build
    set(PYTHON3_WHEEL_CACHE "$ENV{PYTHON3_WHEEL_CACHE}" CACHE PATH "Python3 wheel cache")
    if (EXISTS "${PYTHON3_WHEEL_CACHE}")
        message(STATUS "Using wheel cache for python package installation: ${PYTHON3_WHEEL_CACHE}")
        set(PIP3_INSTALL_COMMAND pip3 install --no-index --find-links=${PYTHON3_WHEEL_CACHE} ${IFCODEGEN_SOURCE_DIR} --verbose)
    else()
        set(PIP3_INSTALL_COMMAND pip3 install --upgrade ${IFCODEGEN_SOURCE_DIR})
    endif()

    if (${COMPILE_IFCODEGEN})
        set(INSTALL_NUITKA_COMMAND COMMAND pip3 install nuitka ordered-set zstandard)
        if ("${CMAKE_HOST_SYSTEM_NAME}" STREQUAL "Linux")
            set(EXTRA_LINUX_PACKAGES_COMMAND COMMAND pip3 install patchelf)
        endif()
    endif()

    add_custom_command(OUTPUT ${IFCODEGEN_BIN}
                       COMMAND ${VIRTUALENV_ACTIVATE_COMMAND}
                       ${INSTALL_MINIMAL_QFACE_PACKAGES_COMMAND}
                       COMMAND ${PIP3_INSTALL_COMMAND}
                       ${INSTALL_NUITKA_COMMAND}
                       ${EXTRA_LINUX_PACKAGES_COMMAND}
                       COMMAND pip3 freeze -l > ${VIRTUALENV_PATH}/frozen.txt
                       COMMAND ${Python3_EXECUTABLE} ${CMAKE_CURRENT_SOURCE_DIR}/relaxfreeze.py ${VIRTUALENV_PATH}/frozen.txt ${IFCODEGEN_SOURCE_DIR}/requirements.txt ${VIRTUALENV_PATH}/requirements.txt --keep qface
                       COMMAND ${CMAKE_COMMAND} -E echo ${VIRTUALENV_PATH} > ${VIRTUALENV_PATH}/venvpath.txt
                       COMMAND ${VIRTUALENV_DEACTIVATE_COMMAND}
                       DEPENDS ${VIRTUALENV_ACTIVATE}
                               ${IFCODEGEN_SOURCE_DIR}/requirements.txt
                               ${IFCODEGEN_SOURCE_FILES}|
                       COMMENT "Installing qface development version into ${VIRTUALENV_NAME}"
                       )
    if (${COMPILE_IFCODEGEN})
        set(MAKE_IFCODEGEN_COMMAND
                COMMAND python -m nuitka ${CMAKE_CURRENT_SOURCE_DIR}/generate.py
                               --onefile
                               --standalone
                               --include-data-files=${CMAKE_CURRENT_BINARY_DIR}/.config=
                               --include-package-data=qface
                               --output-filename=ifcodegen
                               --output-dir=${QT_BUILD_DIR}/${INSTALL_LIBEXECDIR}
                               --onefile-tempdir-spec="%CACHE_DIR%/%COMPANY%/%PRODUCT%/%VERSION%"
                               --company-name="The Qt Company"
                               --product-name="ifcodegen"
                               --product-version="${QT_REPO_MODULE_VERSION}"
        )
        set(MAKE_IFCODEGEN_MESSAGE "Building a relocatable ifcodegen")
    else()
        set(MAKE_IFCODEGEN_COMMAND
                COMMAND python ${CMAKE_CURRENT_SOURCE_DIR}/verify_generator.py)
        set(MAKE_IFCODEGEN_MESSAGE "Verifying generator")
    endif()

    add_custom_command(OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/.stamp-generator-verified
                              ${CMAKE_CURRENT_BINARY_DIR}/.stamp-cmake-rerun
                       COMMAND ${VIRTUALENV_ACTIVATE_COMMAND}
                       ${MAKE_IFCODEGEN_COMMAND}
                       COMMAND ${CMAKE_COMMAND} -E touch .stamp-generator-verified
                       COMMAND ${CMAKE_COMMAND} -E touch .stamp-cmake-rerun
                       COMMAND ${VIRTUALENV_DEACTIVATE_COMMAND}
                       DEPENDS ${IFCODEGEN_BIN}
                       COMMENT ${MAKE_IFCODEGEN_MESSAGE}
                      )

    # main target which just relies on the stamp file to be uptodate
    add_custom_target(ifcodegen ALL
                      DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/.stamp-generator-verified
                      DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/.stamp-cmake-rerun
                      )
    # Create the rerun cmake stamp file here to be able to add cmake configure dependency
    # A dependency can only be added to an existing file
    # This file is touched during the build during the deploy virtualenv step and is marked as an output
    # there as well.
    # In the end it makes sure that virtualenv deployment is a dependency for the configure run and
    # done first. Once it is done the configure step is done again and we can use the created virtualenv
    # within CMakeLists.txt istelf
    file(TOUCH ${CMAKE_CURRENT_BINARY_DIR}/.stamp-cmake-rerun)
    set_property(DIRECTORY APPEND PROPERTY CMAKE_CONFIGURE_DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/.stamp-cmake-rerun)

    if (${COMPILE_IFCODEGEN})
        qt_copy_or_install(PROGRAMS "${QT_BUILD_DIR}/${INSTALL_LIBEXECDIR}/ifcodegen${CMAKE_EXECUTABLE_SUFFIX}"
                           DESTINATION "${INSTALL_LIBEXECDIR}")
    else()
        #####################################################################
        ## clean target
        #####################################################################

        set_property(DIRECTORY APPEND PROPERTY ADDITIONAL_CLEAN_FILES
            ${CMAKE_CURRENT_BINARY_DIR}/.stamp-generator-verified
            ${CMAKE_CURRENT_BINARY_DIR}/.stamp-cmake-rerun
            ${VIRTUALENV_PATH}
            )

        #####################################################################
        ## virtualenv INSTALL Targets
        #####################################################################
        # qt_copy_or_install does the copying at configure time
        # QT_WILL_INSTALL is the same variable which is used in qt_copy_or_install internally to determine
        # whether installing or copying is needed

        # Although virtual environments cannot be copied, artifact creation involves
        # DESTDIR modification past cmake configuration time, which can only be
        # handled by install(PROGRAM, DIRECTORY) calls. Instead, we regenerate the
        # virtual environment before first use based on the requirements.txt in it.

        if(QT_WILL_INSTALL)
            qt_install(
                DIRECTORY ${VIRTUALENV_PATH}
                USE_SOURCE_PERMISSIONS
                DESTINATION "${ifcodegen_install_dir}"
                )
        else()
            add_custom_command(OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/.stamp-copy_virtualenv
                               COMMAND ${CMAKE_COMMAND} -E copy_directory ${VIRTUALENV_PATH} "${ifcodegen_install_dir}/${VIRTUALENV_NAME}"
                               COMMAND ${CMAKE_COMMAND} -E touch .stamp-copy_virtualenv
                               DEPENDS ${IFCODEGEN_BIN}
                              )
            add_custom_target(copy_virtualenv ALL
                              DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/.stamp-copy_virtualenv
                              )
            add_dependencies(ifcodegen copy_virtualenv)
        endif()
    endif()
endif()

#####################################################################
## .config file generation
#####################################################################

file(WRITE ${CMAKE_CURRENT_BINARY_DIR}/.config "---\n")
file(APPEND ${CMAKE_CURRENT_BINARY_DIR}/.config "VERSION: \"${QT_REPO_MODULE_VERSION}\"\n")
file(APPEND ${CMAKE_CURRENT_BINARY_DIR}/.config "FEATURES:\n")
# With qmake we only added the features of interfaceframework. Now we have to add all of the complete repository
foreach(feature IN LISTS QT_KNOWN_FEATURES)
    if (QT_FEATURE_${feature})
        file(APPEND ${CMAKE_CURRENT_BINARY_DIR}/.config "  - \"${feature}\"\n")
    endif()
endforeach()

#####################################################################
## IDE Helpers
#####################################################################

FILE(GLOB_RECURSE templateFiles "templates/*")
add_custom_target(templates SOURCES ${templateFiles})
FILE(GLOB_RECURSE generatorFiles "generator/*")
add_custom_target(generator SOURCES
    ${generatorFiles}
    generate.py
)

#####################################################################
## INSTALL Targets
#####################################################################

qt_copy_or_install(
    FILES
    templates/common/generated_comment.cpp.tpl
    templates/common/generated_comment.cmake.tpl
    templates/common/generated_comment.qmake.tpl
    templates/common/generated_comment.qml.tpl
    templates/common/generated_comment.rep.tpl
    templates/common/qtif_macros.j2
    templates/common/simulation.qmltypes.tpl
    templates/common/plugins.qmltypes.tpl
    templates/common/designer.metainfo.tpl
    templates/common/qmldir.tpl
    templates/common/interface.rep.tpl
    templates/common/simulation_data.json.tpl
    templates/common/simulation.qrc.tpl
    templates/common/module_simulation.qml.tpl
    templates/common/backend_simulation.cpp.tpl
    templates/common/backend_simulation.h.tpl
    templates/common/backend_simulation.qml.tpl
    templates/common/pagingmodel_simulation.h.tpl
    templates/common/pagingmodel_simulation.cpp.tpl
    DESTINATION "${templates_install_dir}/common"
    )

qt_copy_or_install(
    FILES
    templates/frontend/backendinterface.cpp.tpl
    templates/frontend/backendinterface.h.tpl
    templates/frontend/global.h.tpl
    templates/frontend/interface.cpp.tpl
    templates/frontend/interface.h.tpl
    templates/frontend/interface_p.h.tpl
    templates/frontend/module.cpp.tpl
    templates/frontend/module.h.tpl
    templates/frontend/module_qml_enum.qdocinc.tpl
    templates/frontend/module.pri.tpl
    templates/frontend/modulefactory.cpp.tpl
    templates/frontend/modulefactory.h.tpl
    templates/frontend/struct.cpp.tpl
    templates/frontend/struct.h.tpl
    templates/frontend/CMakeLists.txt.tpl
    DESTINATION "${templates_install_dir}/frontend"
    )

qt_copy_or_install(
    FILES
    templates/qmlplugin/module.pri.tpl
    templates/qmlplugin/plugin.cpp.tpl
    templates/qmlplugin/qmldir_plugin.tpl
    templates/qmlplugin/CMakeLists.txt.tpl
    DESTINATION "${templates_install_dir}/qmlplugin"
    )

qt_copy_or_install(
    FILES
    templates/backend_simulator/plugin.cpp.tpl
    templates/backend_simulator/plugin.h.tpl
    templates/backend_simulator/plugin.json
    templates/backend_simulator/plugin.pri.tpl
    templates/backend_simulator/CMakeLists.txt.tpl
    DESTINATION "${templates_install_dir}/backend_simulator"
    )

qt_copy_or_install(
    FILES
    templates/test/tst_test.h.tpl
    templates/test/tst_test.cpp.tpl
    templates/test/module.pri.tpl
    templates/test/main.cpp.tpl
    templates/test/pagingmodel.h.tpl
    templates/test/CMakeLists.txt.tpl
    DESTINATION "${templates_install_dir}/test"
    )

qt_copy_or_install(
    FILES
    templates/frontend.yaml
    templates/qmlplugin.yaml
    templates/backend_simulator.yaml
    templates/test.yaml
    DESTINATION "${templates_install_dir}"
    )

# Don't install if we are building for a target and currently using a host Qt
if ("${QT_HOST_PATH}" STREQUAL "" OR (CMAKE_CROSSCOMPILING AND QT_FORCE_BUILD_TOOLS)
        AND NOT QT_FEATURE_compiled_ifcodegen)
    qt_copy_or_install(
        FILES
        templates/backend_qtro/backend.cpp.tpl
        generator/global_functions.py
        generator/builtin_config.py
        generator/filters.py
        generator/rule_generator.py
        DESTINATION "${ifcodegen_install_dir}/generator"
        )

    qt_copy_or_install(
        FILES
        generate.py
        ${CMAKE_CURRENT_BINARY_DIR}/.config
        DESTINATION "${ifcodegen_install_dir}"
        )
endif()


if (QT_FEATURE_remoteobjects)
    qt_copy_or_install(
        FILES
        templates/backend_qtro/backend.cpp.tpl
        templates/backend_qtro/backend.h.tpl
        templates/backend_qtro/pagingmodel.h.tpl
        templates/backend_qtro/pagingmodel.cpp.tpl
        templates/backend_qtro/plugin.cpp.tpl
        templates/backend_qtro/plugin.h.tpl
        templates/backend_qtro/plugin.json
        templates/backend_qtro/plugin.pri.tpl
        templates/backend_qtro/CMakeLists.txt.tpl
        DESTINATION "${templates_install_dir}/backend_qtro"
        )

    qt_copy_or_install(
        FILES
        templates/server_qtro/core.cpp.tpl
        templates/server_qtro/core.h.tpl
        templates/server_qtro/main.cpp.tpl
        templates/server_qtro/server.pri.tpl
        templates/server_qtro/CMakeLists.txt.tpl
        DESTINATION "${templates_install_dir}/server_qtro"
        )

    qt_copy_or_install(
        FILES
        templates/server_qtro_simulator/core.cpp.tpl
        templates/server_qtro_simulator/core.h.tpl
        templates/server_qtro_simulator/adapter.cpp.tpl
        templates/server_qtro_simulator/adapter.h.tpl
        templates/server_qtro_simulator/main.cpp.tpl
        templates/server_qtro_simulator/server.pri.tpl
        templates/server_qtro_simulator/CMakeLists.txt.tpl
        DESTINATION "${templates_install_dir}/server_qtro_simulator"
        )

    qt_copy_or_install(
        FILES
        templates/backend_qtro.yaml
        templates/server_qtro.yaml
        templates/server_qtro_simulator.yaml
        DESTINATION "${templates_install_dir}"
        )
endif()
