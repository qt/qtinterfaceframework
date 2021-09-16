# Generated from ifvehiclefunctions.pro.

add_subdirectory(climate-qml)
add_subdirectory(window-qml)
if(TARGET Qt::Widgets)
    add_subdirectory(climate-widget)
endif()
