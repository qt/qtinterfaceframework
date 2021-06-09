# special case begin
# Define a stub for qt-configure-module
if (NOT COMMAND qt_config_python3_package_test)
 defstub(qt_config_python3_package_test)
endif()
# special case end

#### Inputs

# input ifcodegen
set(INPUT_ifcodegen "undefined" CACHE STRING "")
set_property(CACHE INPUT_ifcodegen PROPERTY STRINGS undefined no qt system)

# input qface
set(INPUT_qface "undefined" CACHE STRING "")
set_property(CACHE INPUT_qface PROPERTY STRINGS undefined no qt system)



#### Libraries

qt_find_package(Python3 PROVIDED_TARGETS Python3::Interpreter MODULE_NAME interfaceframework)

#### Tests

qt_config_python3_package_test(virtualenv
    LABEL "Python3 virtualenv package"
    PACKAGE "virtualenv"
)
qt_config_python3_package_test(qface
    LABEL "Python3 qface package"
    PACKAGE "qface"
    VERSION "2.0.3"
)

#### Features

qt_feature("python3" PRIVATE
    LABEL "python3"
    CONDITION PYTHON3_FOUND
)
qt_feature("python3-virtualenv" PRIVATE
    LABEL "virtualenv"
    CONDITION QT_FEATURE_python3 AND TEST_virtualenv
)
qt_feature("system-qface" PUBLIC
    LABEL "System QFace"
    CONDITION TEST_qface
    ENABLE INPUT_qface STREQUAL 'system'
    DISABLE INPUT_qface STREQUAL 'no' OR INPUT_qface STREQUAL 'qt'
)
qt_feature("system-ifcodegen" PRIVATE
    LABEL "System Interface Framework Generator"
    ENABLE INPUT_ifcodegen STREQUAL 'system'
    DISABLE ( NOT INPUT_ifcodegen STREQUAL 'system' )
)
qt_feature("ifcodegen" PUBLIC
    LABEL "Interface Framework Generator"
    CONDITION QT_FEATURE_interfaceframework AND QT_FEATURE_python3 AND ( ( QT_FEATURE_python3_virtualenv AND EXISTS "${CMAKE_CURRENT_LIST_DIR}/../3rdparty/qface/setup.py" ) OR ( QT_FEATURE_system_qface ) ) OR QT_FEATURE_system_ifcodegen # special case
    ENABLE INPUT_ifcodegen STREQUAL 'qt' OR INPUT_ifcodegen STREQUAL 'system'
    DISABLE INPUT_ifcodegen STREQUAL 'no'
)
qt_feature("host-tools-only" PRIVATE
    LABEL "Only build the host tools"
    CONDITION INPUT_host_tools_only STREQUAL 'yes'
)
qt_feature("remoteobjects" PUBLIC
    LABEL "QtRemoteObjects Support"
    CONDITION TARGET Qt::RemoteObjects OR INPUT_force_ifcodegen_qtremoteobjects STREQUAL 'yes'
)
qt_feature("interfaceframework" PUBLIC
    LABEL "Qt Interface Framework Core"
)
qt_configure_add_summary_section(NAME "Qt Interface Framework Core")
qt_configure_add_summary_section(NAME "Python3")
if(PYTHON3_FOUND)
    qt_configure_add_summary_entry(TYPE "message" ARGS "Executable" MESSAGE "${Python3_EXECUTABLE}")
    qt_configure_add_summary_entry(TYPE "message" ARGS "Version" MESSAGE "${Python3_VERSION}")
else()
    qt_configure_add_summary_entry(TYPE "message" ARGS "Executable" MESSAGE "no")
endif()
qt_configure_add_summary_entry(ARGS "python3-virtualenv")
qt_configure_add_summary_entry(ARGS "system-qface")
qt_configure_end_summary_section() # end of "Python3" section
qt_configure_add_summary_entry(ARGS "ifcodegen")
qt_configure_add_summary_entry(ARGS "remoteobjects")
qt_configure_end_summary_section() # end of "Qt Interface Framework Core" section
qt_configure_add_summary_entry(
    ARGS "interfaceframework"
    CONDITION NOT QT_FEATURE_interfaceframework
)
qt_configure_add_report_entry(
    TYPE ERROR
# special case begin
    MESSAGE [[
Cannot build the Interface Framework Generator because its dependencies are not satisfied.
The Interface Framework Generator provides tooling to generate source code out of IDL files.
Make sure python3 and its 'virtualenv' packages are installed.
E.g. by running
    apt-get install python3 python3-virtualenv

And make sure the qface submodule is initialized or the correct qface version is installed on your system.
E.g. by running the following command:
    git submodule init && git submodule update
]]
# special case end
    CONDITION QT_FEATURE_interfaceframework AND NOT QT_FEATURE_ifcodegen AND ( NOT INPUT_ifcodegen STREQUAL 'no' )
)
qt_configure_add_report_entry(
    TYPE WARNING
    MESSAGE "Cannot enable the QtRemoteObjects features because the QtRemoteObjects module is not installed."
    CONDITION NOT QT_FEATURE_remoteobjects
)
