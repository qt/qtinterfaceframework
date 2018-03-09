TEMPLATE = aux

QT_FOR_CONFIG += ivicore

!contains(CONFIG, no_internal_qface): include(qface_internal_build.pri)

# Make sure to only build this once in a debug_and_release config
# This needs to be the last step as it unsets other configs and may have side effects
CONFIG -= debug_and_release

templates_frontend.files +=  \
    templates_frontend/backendinterface.cpp.tpl \
    templates_frontend/backendinterface.h.tpl \
    templates_frontend/generated_comment.cpp.tpl \
    templates_frontend/global.h.tpl \
    templates_frontend/interface.cpp.tpl \
    templates_frontend/interface.h.tpl \
    templates_frontend/interface_p.h.tpl \
    templates_frontend/module.cpp.tpl \
    templates_frontend/module.h.tpl \
    templates_frontend/module.pri.tpl \
    templates_frontend/modulefactory.cpp.tpl \
    templates_frontend/modulefactory.h.tpl \
    templates_frontend/struct.cpp.tpl \
    templates_frontend/struct.h.tpl \
    templates_frontend/structmodel.cpp.tpl \
    templates_frontend/structmodel.h.tpl \
    templates_frontend/structmodel_p.h.tpl \
    templates_frontend/utils.tpl
templates_frontend.path = $$[QT_HOST_BINS]/ivigenerator/templates_frontend

templates_backend_simulator.files += \
    templates_backend_simulator/backend.cpp.tpl \
    templates_backend_simulator/backend.h.tpl \
    templates_backend_simulator/backend_range.cpp.tpl \
    templates_backend_simulator/generated_comment.cpp.tpl \
    templates_backend_simulator/plugin.cpp.tpl \
    templates_backend_simulator/plugin.h.tpl \
    templates_backend_simulator/plugin.json \
    templates_backend_simulator/plugin.pri.tpl \
    templates_backend_simulator/plugin.pro \
    templates_backend_simulator/utils.tpl
templates_backend_simulator.path = $$[QT_HOST_BINS]/ivigenerator/templates_backend_simulator

templates_generation_validator.files += \
    templates_generation_validator/generated_comment.cpp.tpl \
    templates_generation_validator/generationstatusitem.cpp.tpl \
    templates_generation_validator/generationstatusitem.h.tpl \
    templates_generation_validator/main.cpp.tpl \
    templates_generation_validator/main.qml.tpl \
    templates_generation_validator/qml.qrc.tpl \
    templates_generation_validator/ui.pri.tpl \
    templates_generation_validator/validationstatus.cpp.tpl \
    templates_generation_validator/validationstatus.h.tpl
templates_generation_validator.path = $$[QT_HOST_BINS]/ivigenerator/templates_generation_validator

templates_control_panel.files += \
    templates_control_panel/EnumControl.qml.tpl \
    templates_control_panel/FlagControl.qml.tpl \
    templates_control_panel/generated_comment.cpp.tpl \
    templates_control_panel/global.h.tpl \
    templates_control_panel/interface.cpp.tpl \
    templates_control_panel/interface.h.tpl \
    templates_control_panel/interface.qml.tpl \
    templates_control_panel/main.cpp.tpl \
    templates_control_panel/main.qml.tpl \
    templates_control_panel/module.cpp.tpl \
    templates_control_panel/module.h.tpl \
    templates_control_panel/qml.qrc.tpl \
    templates_control_panel/ui.pri.tpl \
    templates_control_panel/utils.tpl
templates_control_panel.path = $$[QT_HOST_BINS]/ivigenerator/templates_control_panel

templates_backend_qtro.files += \
    templates_backend_qtro/generated_comment.cpp.tpl \
    templates_backend_qtro/backend.cpp.tpl \
    templates_backend_qtro/backend.h.tpl \
    templates_backend_qtro/plugin.cpp.tpl \
    templates_backend_qtro/plugin.h.tpl \
    templates_backend_qtro/plugin.json \
    templates_backend_qtro/plugin.pri.tpl \
    templates_backend_qtro/interface.rep.tpl
templates_backend_qtro.path = $$[QT_HOST_BINS]/ivigenerator/templates_backend_qtro

templates_server_qtro.files += \
    templates_server_qtro/generated_comment.cpp.tpl \
    templates_server_qtro/core.cpp.tpl \
    templates_server_qtro/core.h.tpl \
    templates_server_qtro/server.pri.tpl \
    templates_server_qtro/interface.rep.tpl
templates_server_qtro.path = $$[QT_HOST_BINS]/ivigenerator/templates_server_qtro

templates_test.files += \
    templates_test/generated_comment.cpp.tpl \
    templates_test/tst_test.h.tpl \
    templates_test/tst_test.cpp.tpl \
    templates_test/module.pri.tpl \
    templates_test/main.cpp.tpl
templates_test.path = $$[QT_HOST_BINS]/ivigenerator/templates_test

generator.files += \
    generate.py \
    $$OUT_PWD/.config \
    templates_frontend.yaml \
    templates_backend_simulator.yaml \
    templates_generation_validator.yaml \
    templates_backend_qtro.yaml \
    templates_server_qtro.yaml \
    templates_test.yaml \

generator.path = $$[QT_HOST_BINS]/ivigenerator

qtConfig(simulator) {
    generator.files += templates_control_panel.yaml
    INSTALLS += templates_control_panel
}

INSTALLS += generator \
    templates_frontend \
    templates_backend_simulator \
    templates_generation_validator \
    templates_backend_qtro \
    templates_server_qtro \
    templates_test \

# Ensure files are installed to qtbase for non-prefixed builds
!force_independent:if(!debug_and_release|!build_all|CONFIG(release, debug|release)) {
    for (install_target, INSTALLS) {
        path = $$eval($${install_target}.path)
        depends = $$eval($${install_target}.depends)
        $${install_target}_copy.input = $${install_target}.files
        $${install_target}_copy.output = $$path/${QMAKE_FILE_IN_BASE}${QMAKE_FILE_EXT}
        !isEmpty(depends) {
            $${install_target}_copy.depends = $$eval($${depends}.target)
        }
        $${install_target}_copy.commands = $$sprintf($$QMAKE_MKDIR_CMD, "$$path") $$escape_expand(\n\t)
        contains($${install_target}.CONFIG, directory): $${install_target}_copy.commands += $$QMAKE_COPY_DIR ${QMAKE_FILE_IN} ${QMAKE_FILE_OUT}
        else: $${install_target}_copy.commands += $$QMAKE_COPY ${QMAKE_FILE_IN} ${QMAKE_FILE_OUT}
        $${install_target}_copy.name = COPY ${QMAKE_FILE_IN}
        $${install_target}_copy.CONFIG = no_link target_predeps no_clean
        QMAKE_EXTRA_COMPILERS += $${install_target}_copy
    }
}

defineTest(createConfig) {
    write_file($$OUT_PWD/.config, $$list("---"))
    for(var, ARGS) {
        isEmpty($$var):out = "$$var: ~"
        else:count($$var, 1):out = "$$var: \"$$first($$var)\""
        else {
            out = "$$var:"
            for(val, $$var):out += "  - \"$$val\""
            out=$$join(out, "$$escape_expand(\\n)")
        }
        write_file($$OUT_PWD/.config, out, append)
    }
}

VERSION = $$MODULE_VERSION
FEATURES = $${QT.ivicore.enabled_features}
createConfig(VERSION, FEATURES)
