QFACE_SOURCE_DIR = $$QTIVI_ROOT/src/3rdparty/qface

# only create the environment once
debug_and_release:build_pass:CONFIG(release, debug|release) {
    return();
}

!exists($$QFACE_SOURCE_DIR/setup.py): error("Couldn't find $$QFACE_SOURCE_DIR: Please make sure all submodules are initialized")

include($$shadowed($$PWD/../../ivicore/qtivicore-config.pri))

VIRTUALENV_EXE = $$QMAKE_PYTHON3_LOCATION -m virtualenv
# virtualenv is always using the default interpreter, which is python2 on many systems"
# by adding -p we enforce that the python3 interpreter is used and make sure python3 is installed in the virtualenv
VIRTUALENV_EXE += " -p $$QMAKE_PYTHON3_LOCATION"

# Use a Python virtualenv for installing qface, so we don't pollute the user environment
# On some systems virtualenv --always-copy doesn't work (https://github.com/pypa/virtualenv/issues/565).
# To workaround the problem, we need to manually create the folder and create the virtualenv from
# inside
win32: qtivi_qface_virtualenv.target = qtivi_qface_virtualenv/Scripts/python.exe
else:  qtivi_qface_virtualenv.target = qtivi_qface_virtualenv/bin/python
qtivi_qface_virtualenv.commands = \
    $(MKDIR) qtivi_qface_virtualenv $$escape_expand(\n\t) \
    cd qtivi_qface_virtualenv && $$VIRTUALENV_EXE --always-copy . $$escape_expand(\n\t) \
    cd qtivi_qface_virtualenv && $$VIRTUALENV_EXE --relocatable . $$escape_expand(\n\t) \
    @echo "Set up virtualenv for qface, name: qtivi_qface_virtualenv"
QMAKE_EXTRA_TARGETS += qtivi_qface_virtualenv

win32: VIRTUALENV_ACTIVATION = qtivi_qface_virtualenv\Scripts\activate &&
else: VIRTUALENV_ACTIVATION = . qtivi_qface_virtualenv/bin/activate &&

# Create the forceRebuild file during the qmake run.
# This file is used as a dependency in other Makefiles.
# Once the virtualenv is created and setup the file will
# be touched to recreate theses Makefiles.
write_file($$OUT_PWD/forceRebuild)

PYTHON3_SHORT_VERSION_SPLITTED = $$split(QMAKE_PYTHON3_VERSION, .)
PYTHON3_SHORT_VERSION = $$member(PYTHON3_SHORT_VERSION_SPLITTED, 0).$$member(PYTHON3_SHORT_VERSION_SPLITTED, 1)
# Always run this target
win32: qtivi_qface_install.target = qtivi_qface_virtualenv/Lib/site-packages/qface
else: qtivi_qface_install.target = qtivi_qface_virtualenv/lib/python$${PYTHON3_SHORT_VERSION}/site-packages/qface
qtivi_qface_install.depends = $${qtivi_qface_virtualenv.target}
qtivi_qface_install.depends += $$QFACE_SOURCE_DIR/setup.py
qtivi_qface_install.depends += $$QFACE_SOURCE_DIR/requirements.txt
qtivi_qface_install.depends += $$QFACE_SOURCE_DIR/qface/__about__.py
qtivi_qface_install.commands = $$VIRTUALENV_ACTIVATION \
        pip3 install --upgrade $$shell_path($$QFACE_SOURCE_DIR) $$escape_expand(\n\t) \
        @echo "Installed qface development version into qtivi_qface_virtualenv" $$escape_expand(\n\t)
win32: qtivi_qface_install.commands += @COPY /B $$shell_path($$OUT_PWD/forceRebuild)+,, $$shell_path($$OUT_PWD/forceRebuild) >NUL
else: qtivi_qface_install.commands += @touch $$OUT_PWD/forceRebuild
QMAKE_EXTRA_TARGETS += qtivi_qface_install

# We need to make the virtualenv first deployable
# Otherwise it still needs some modules from the system
deploy_virtualenv.target = .stamp-deploy_virtualenv
win32 {
    deploy_virtualenv.commands = $$PWD/deploy-virtualenv.bat qtivi_qface_virtualenv $$escape_expand(\n\t)
    deploy_virtualenv.commands += @type nul > $$shell_path($$OUT_PWD/.stamp-deploy_virtualenv)
} else {
    deploy_virtualenv.commands = $$PWD/deploy-virtualenv.sh qtivi_qface_virtualenv $$escape_expand(\n\t)
    deploy_virtualenv.commands += @touch $$OUT_PWD/.stamp-deploy_virtualenv
}
deploy_virtualenv.depends = $${qtivi_qface_install.target}
QMAKE_EXTRA_TARGETS += deploy_virtualenv
PRE_TARGETDEPS += $${deploy_virtualenv.target}

virtualenv.files = $$OUT_PWD/qtivi_qface_virtualenv
virtualenv.path = $$[QT_HOST_BINS]/ivigenerator
virtualenv.depends = deploy_virtualenv
virtualenv.CONFIG = no_check_exist directory no_build

INSTALLS += virtualenv

# On osx the hidden .Python file is needed in the installed virtualenv as well
# this file is not installed by the directory config, so we need to install it manually
osx {
    virtualenv-python.files = $$OUT_PWD/qtivi_qface_virtualenv/.Python
    virtualenv-python.path = $$[QT_HOST_BINS]/ivigenerator/qtivi_qface_virtualenv
    # In case the file doesn't exist, touch creates it and fixes the install target
    virtualenv-python.extra = @touch $$OUT_PWD/qtivi_qface_virtualenv/.Python
    virtualenv-python.CONFIG = no_check_exist
    INSTALLS += virtualenv-python
}
