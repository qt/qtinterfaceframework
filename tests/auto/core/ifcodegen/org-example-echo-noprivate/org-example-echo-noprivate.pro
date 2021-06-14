TEMPLATE = subdirs

QT_FOR_CONFIG += interfaceframework

SUBDIRS = frontend \
          qmlplugin \
          backend_simulator \
          test

backend_simulator.depends = frontend
test.depends = frontend
qmlplugin.depends = frontend
