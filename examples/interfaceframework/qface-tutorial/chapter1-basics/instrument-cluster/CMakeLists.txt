set(CMAKE_AUTOMOC ON)

if(NOT DEFINED INSTALL_EXAMPLESDIR)
    set(INSTALL_EXAMPLESDIR "examples")
endif()

set(INSTALL_EXAMPLEDIR "${INSTALL_EXAMPLESDIR}/interfaceframework/qface-tutorial/chapter1-basics")

find_package(Qt6 REQUIRED COMPONENTS Core Gui Qml Quick)

qt_add_executable(ic_chapter1
    main.cpp
)

set_target_properties(ic_chapter1 PROPERTIES OUTPUT_NAME "chapter1-basics")
set_target_properties(ic_chapter1 PROPERTIES RUNTIME_OUTPUT_DIRECTORY ../)
set_target_properties(ic_chapter1 PROPERTIES
    WIN32_EXECUTABLE TRUE
    MACOSX_BUNDLE FALSE
)

target_link_libraries(ic_chapter1 PUBLIC
    libIc_chapter1
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

qt_add_resources(ic_chapter1 "app"
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

qt_add_resources(ic_chapter1 "images"
    PREFIX
        "/images"
    BASE
        "../../images"
    FILES
        ${images_resource_files}
)

install(TARGETS ic_chapter1
    RUNTIME DESTINATION "${INSTALL_EXAMPLEDIR}"
    BUNDLE DESTINATION "${INSTALL_EXAMPLEDIR}"
    LIBRARY DESTINATION "${INSTALL_EXAMPLEDIR}"
)
