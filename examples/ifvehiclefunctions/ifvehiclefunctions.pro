TEMPLATE = subdirs

SUBDIRS += \
    climate-qml \
    window-qml

qtHaveModule(widgets): SUBDIRS += climate-widget
