set(CMAKE_AUTOMOC ON)

if(NOT DEFINED INSTALL_EXAMPLESDIR)
    set(INSTALL_EXAMPLESDIR "examples")
endif()

set(INSTALL_EXAMPLEDIR "${INSTALL_EXAMPLESDIR}/interfaceframework/qface-tutorial/ch4-simulation-behavior")

find_package(Qt6 REQUIRED COMPONENTS Core Gui Qml Quick)

qt_add_executable(ic_ch4
    main.cpp
)

set_target_properties(ic_ch4 PROPERTIES RUNTIME_OUTPUT_DIRECTORY ../)
set_target_properties(ic_ch4 PROPERTIES
    WIN32_EXECUTABLE TRUE
    MACOSX_BUNDLE FALSE
    QT_QML_IMPORT_PATH "${CMAKE_CURRENT_BINARY_DIR}/../imports"
)
set_property(TARGET ic_ch4 APPEND PROPERTY QT_ANDROID_EXTRA_PLUGINS
    "${CMAKE_CURRENT_BINARY_DIR}/../interfaceframework")

target_link_libraries(ic_ch4 PUBLIC
    Qt::Core
    Qt::Gui
    Qt::Qml
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

qt_add_resources(ic_ch4 "app"
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

qt_add_resources(ic_ch4 "images"
    PREFIX
        "/images"
    BASE
        "../../images"
    FILES
        ${images_resource_files}
)

install(TARGETS ic_ch4
    RUNTIME DESTINATION "${INSTALL_EXAMPLEDIR}"
    BUNDLE DESTINATION "${INSTALL_EXAMPLEDIR}"
    LIBRARY DESTINATION "${INSTALL_EXAMPLEDIR}"
)
