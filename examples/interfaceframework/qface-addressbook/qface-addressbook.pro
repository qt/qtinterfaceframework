TEMPLATE = subdirs

SUBDIRS = frontend \
          backend_simulator \
          demo

CONFIG += ordered

OTHER_FILES +=

DISTFILES += \
    example-addressbook.qface \
    example-addressbook.yaml
