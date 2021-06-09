TEMPLATE = subdirs

SUBDIRS = frontend \
          backend_simulator \
          demo

CONFIG += ordered

OTHER_FILES +=

DISTFILES += \
    example-if-addressbook.qface \
    example-if-addressbook.yaml
