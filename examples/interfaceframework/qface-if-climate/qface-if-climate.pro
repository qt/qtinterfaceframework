TEMPLATE = subdirs

SUBDIRS = frontend \
          backend_simulator \
          demo

CONFIG += ordered

OTHER_FILES += \
    example-if-climate.qface \
    example-if-climate.yaml
