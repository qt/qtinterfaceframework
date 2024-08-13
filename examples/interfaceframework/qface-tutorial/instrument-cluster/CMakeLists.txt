cmake_minimum_required(VERSION 3.16)
project(instrument-cluster LANGUAGES CXX)

find_package(Qt6 REQUIRED COMPONENTS Core)

qt_standard_project_setup(REQUIRES 6.7)

if(NOT DEFINED INSTALL_EXAMPLESDIR)
    set(INSTALL_EXAMPLESDIR "examples")
endif()

set(INSTALL_EXAMPLEDIR "${INSTALL_EXAMPLESDIR}/interfaceframework/qface-tutorial/instrument-cluster")

find_package(Qt6 REQUIRED COMPONENTS Core Gui Qml Quick)

qt_add_executable(instrument-cluster WIN32
    main.cpp
)

target_link_libraries(instrument-cluster PUBLIC
    Qt::Core
    Qt::Gui
    Qt::Qml
    Qt::Quick
)

qt_add_resources(instrument-cluster "app"
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

qt_add_resources(instrument-cluster "images"
    PREFIX
        "/images"
    BASE
        "../images"
    FILES
        "../images/+--.png"
        "../images/P-R-N-D.png"
        "../images/dial_cursor.png"
        "../images/dial_cursor_right.png"
        "../images/dial_fill_color.png"
        "../images/dial_fill_color_left.png"
        "../images/dial_pattern.png"
        "../images/fuel.png"
        "../images/fuel_level.png"
        "../images/fuelsymbol_orange.png"
        "../images/left_dial.png"
        "../images/mask_overlay.png"
        "../images/middle-bkg.png"
        "../images/middle-circle.png"
        "../images/right_dial.png"
        "../images/top_bar.png"
)

install(TARGETS instrument-cluster
    RUNTIME DESTINATION "${INSTALL_EXAMPLEDIR}"
    BUNDLE DESTINATION "${INSTALL_EXAMPLEDIR}"
    LIBRARY DESTINATION "${INSTALL_EXAMPLEDIR}"
)
