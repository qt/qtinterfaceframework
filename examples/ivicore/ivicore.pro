TEMPLATE = subdirs

QT_FOR_CONFIG += interfaceframework
qtConfig(ifcodegen) {
    SUBDIRS += qface-if-climate \
               qface-if-addressbook
}
qtConfig(ifcodegen): qtConfig(remoteobjects): SUBDIRS += qface-if-remote

qtConfig(ifcodegen): qtConfig(remoteobjects): SUBDIRS += qface-tutorial
