TEMPLATE = aux

QMAKE_DOCS = $$PWD/qtautomotivesuite.qdocconf
QMAKE_DOCS_OUTPUTDIR = $$OUT_PWD/qtautomotivesuite

OTHER_FILES += $$QMAKE_DOCS \
    $$PWD/src/*.qdoc
