TEMPLATE = subdirs

SUBDIRS = frontend \
          backend_qtro \
          server_qtro \
          demo

CONFIG += ordered

OTHER_FILES += \
    example-if-remote.qface
