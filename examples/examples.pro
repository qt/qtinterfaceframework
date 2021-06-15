TEMPLATE = subdirs

qtHaveModule(gui): {
    qtHaveModule(interfaceframework): SUBDIRS += interfaceframework
    qtHaveModule(ifvehiclefunctions): SUBDIRS += ifvehiclefunctions
    qtHaveModule(ifmedia): SUBDIRS += ifmedia
}
