TEMPLATE = subdirs

SUBDIRS = frontend \
          backend_simulator \
          demo

CONFIG += ordered

OTHER_FILES += \
    example-climate.qface \
    example-climate.yaml
