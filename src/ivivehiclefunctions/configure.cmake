

#### Inputs



#### Libraries



#### Tests



#### Features

qt_feature("vehiclefunctions_simulation_backend" PRIVATE
    LABEL "Simulation Backend"
    CONDITION QT_FEATURE_ifvehiclefunctions
)
qt_feature("vehiclefunctions_qtro_backend" PRIVATE
    LABEL "Qt Remote Objects Backend"
    CONDITION QT_FEATURE_ifvehiclefunctions AND QT_FEATURE_remoteobjects
)
qt_feature("vehiclefunctions_qtro_simulation_server" PRIVATE
    LABEL "Qt Remote Objects Simulation Server"
    CONDITION QT_FEATURE_ifvehiclefunctions AND QT_FEATURE_remoteobjects
)
qt_feature("ifvehiclefunctions" PUBLIC
    LABEL "Qt Interface Framework VehicleFunctions"
    CONDITION QT_FEATURE_interfaceframework AND QT_FEATURE_ifcodegen
)
qt_configure_add_summary_section(NAME "Qt Interface Framework VehicleFunctions")
qt_configure_add_summary_entry(ARGS "vehiclefunctions_qtro_simulation_server")
qt_configure_add_summary_section(NAME "Backends")
qt_configure_add_summary_entry(ARGS "vehiclefunctions_simulation_backend")
qt_configure_add_summary_entry(ARGS "vehiclefunctions_qtro_backend")
qt_configure_end_summary_section() # end of "Backends" section
qt_configure_end_summary_section() # end of "Qt Interface Framework VehicleFunctions" section
qt_configure_add_summary_entry(
    ARGS "ifvehiclefunctions"
    CONDITION NOT QT_FEATURE_ifvehiclefunctions
)
qt_configure_add_report_entry(
    TYPE WARNING
    MESSAGE "Cannot build the Interface Framework VehicleFunctions module because its dependencies are not satisfied or it was disabled. The Interface Framework VehicleFunctions module needs the Interface Framework Core module and the Interface Framework Generator."
    CONDITION NOT QT_FEATURE_ifvehiclefunctions
)
