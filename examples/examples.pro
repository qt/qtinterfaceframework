TEMPLATE = subdirs

qtHaveModule(gui): {
    qtHaveModule(interfaceframework): SUBDIRS += interfaceframework
}
