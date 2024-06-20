TEMPLATE = subdirs

QT_FOR_CONFIG += interfaceframework
qtConfig(ifcodegen) {
    SUBDIRS += qface-climate
}
qtConfig(ifcodegen): qtConfig(remoteobjects): SUBDIRS += qface-remote

qtConfig(ifcodegen): qtConfig(remoteobjects): SUBDIRS += qface-tutorial
