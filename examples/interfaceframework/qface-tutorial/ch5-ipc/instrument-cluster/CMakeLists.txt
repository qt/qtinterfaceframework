
qt_add_executable(ic_chapter5 WIN32
    main.cpp
)

set_target_properties(ic_chapter5 PROPERTIES RUNTIME_OUTPUT_DIRECTORY ../)
set_property(TARGET ic_chapter5 APPEND PROPERTY QT_ANDROID_EXTRA_PLUGINS
    "${CMAKE_CURRENT_BINARY_DIR}/../interfaceframework")

target_link_libraries(ic_chapter5 PUBLIC
    libIc_chapter5
    Qt::Quick
)

qt_add_resources(ic_chapter5 "app"
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

qt_add_resources(ic_chapter5 "images"
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

install(TARGETS ic_chapter5
    RUNTIME DESTINATION "${INSTALL_EXAMPLEDIR}"
    BUNDLE DESTINATION "${INSTALL_EXAMPLEDIR}"
    LIBRARY DESTINATION "${INSTALL_EXAMPLEDIR}"
)
