TEMPLATE = subdirs

SUBDIRS = common \
          frontend \
          qmlplugin \
          backend_simulator \
          test \

frontend.depends = common
backend_simulator.depends = frontend
test.depends = frontend
qmlplugin.depends = frontend

QT_FOR_CONFIG += interfaceframework

qtConfig(remoteobjects) {
    SUBDIRS += backend_qtro \
               simulation_server_qtro

    backend_qtro.depends = frontend
    simulation_server_qtro.depends = frontend
}
