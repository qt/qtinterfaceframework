# ControlPanelHelper QML Module
# Provides reusable UI components for Qt InterfaceFramework Control Panel

set_source_files_properties(
    ${CMAKE_CURRENT_SOURCE_DIR}/ControlPanelStyle.qml
    PROPERTIES
        QT_QML_SINGLETON_TYPE TRUE
)

qt_internal_add_qml_module(IfControlPanelHelper
    URI "QtInterfaceFramework.ControlPanelHelper"
    VERSION "1.0"
    CONFIG_MODULE_NAME ifcontrolpanel_helper
    NO_PRIVATE_MODULE

    QML_FILES
        # Singleton
        ControlPanelStyle.qml

        # Input Controls
        controls/BooleanControl.qml
        controls/ComboBoxControl.qml
        controls/RangeControl.qml
        controls/SpinControl.qml
        controls/TextControl.qml
        controls/UnsupportedControl.qml

        # Composite Widgets
        components/PropertyCard.qml
        components/SearchField.qml
        components/ZoneSelector.qml
        components/PropertySection.qml
        components/NoResultsFound.qml
        components/CustomControlPanelComponent.qml

        # Layout Panels
        panels/InterfaceSelector.qml
        panels/TopBar.qml
        panels/Footer.qml

    DEPENDENCIES
        QtQuick
        QtQuick.Controls
        QtQuick.Layouts
)

target_link_libraries(IfControlPanelHelper
    PUBLIC
        Qt::Core
        Qt::Qml
)
