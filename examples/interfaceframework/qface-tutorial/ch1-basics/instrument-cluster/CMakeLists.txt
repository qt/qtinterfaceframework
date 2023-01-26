set(CMAKE_AUTOMOC ON)

if(NOT DEFINED INSTALL_EXAMPLESDIR)
    set(INSTALL_EXAMPLESDIR "examples")
endif()

set(INSTALL_EXAMPLEDIR "${INSTALL_EXAMPLESDIR}/interfaceframework/qface-tutorial/ch1-basics")

find_package(Qt6 REQUIRED COMPONENTS Core InterfaceFramework Gui Qml Quick)

qt_add_executable(ic_ch1
    main.cpp
)

set_target_properties(ic_ch1 PROPERTIES OUTPUT_NAME "ch1-basics")
set_target_properties(ic_ch1 PROPERTIES RUNTIME_OUTPUT_DIRECTORY ../)
set_target_properties(ic_ch1 PROPERTIES
    WIN32_EXECUTABLE TRUE
    MACOSX_BUNDLE FALSE
)

target_link_libraries(ic_ch1 PUBLIC
    libIc_ch1
    Qt::Quick
)

# Resources:
set(app_resource_files
    "Cluster.qml"
    "Dial.qml"
    "Fuel.qml"
    "Label.qml"
    "LeftDial.qml"
    "RightDial.qml"
    "Top.qml"
)

qt_add_resources(ic_ch1 "app"
    PREFIX
        "/"
    FILES
        ${app_resource_files}
)

set(images_resource_files
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

qt_add_resources(ic_ch1 "images"
    PREFIX
        "/images"
    BASE
        "../../images"
    FILES
        ${images_resource_files}
)

install(TARGETS ic_ch1
    RUNTIME DESTINATION "${INSTALL_EXAMPLEDIR}"
    BUNDLE DESTINATION "${INSTALL_EXAMPLEDIR}"
    LIBRARY DESTINATION "${INSTALL_EXAMPLEDIR}"
)
