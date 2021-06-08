TEMPLATE = subdirs

QT_FOR_CONFIG += ivicore

SUBDIRS = org-example-echo \
          org-example-echo-noprivate \
          org-example-echo-noanno \
          include-test \
          custom-template \

qtConfig(remoteobjects) {
    SUBDIRS += org-example-echo-qtro
}

OTHER_FILES = org.example.echo.qface \
              org.example.echo.yaml \
              org.example.echo.noannotation.qface \
              no-private.yaml \
              include-test.qface \
