
qt_add_executable(ic_ch2 WIN32
    main.cpp
)

set_target_properties(ic_ch2 PROPERTIES RUNTIME_OUTPUT_DIRECTORY ../)

target_link_libraries(ic_ch2 PUBLIC
    libIc_ch2
    Qt::Quick
)

qt_add_resources(ic_ch2 "app"
    PREFIX
        "/"
    FILES
        "Cluster.qml"
        "Dial.qml"
        "Fuel.qml"
        "Label.qml"
        "LeftDial.qml"
        "RightDial.qml"
        "Top.qml"
)

qt_add_resources(ic_ch2 "images"
    PREFIX
        "/images"
    BASE
        "../../images"
    FILES
        "../../images/+--.png"
        "../../images/P-R-N-D.png"
        "../../images/dial_cursor.png"
        "../../images/dial_cursor_right.png"
        "../../images/dial_fill_color.png"
        "../../images/dial_fill_color_left.png"
        "../../images/dial_pattern.png"
        "../../images/fuel.png"
        "../../images/fuel_level.png"
        "../../images/fuelsymbol_orange.png"
        "../../images/left_dial.png"
        "../../images/mask_overlay.png"
        "../../images/middle-bkg.png"
        "../../images/middle-circle.png"
        "../../images/right_dial.png"
        "../../images/top_bar.png"
)

install(TARGETS ic_ch2
    RUNTIME DESTINATION "${INSTALL_EXAMPLEDIR}"
    BUNDLE DESTINATION "${INSTALL_EXAMPLEDIR}"
    LIBRARY DESTINATION "${INSTALL_EXAMPLEDIR}"
)
